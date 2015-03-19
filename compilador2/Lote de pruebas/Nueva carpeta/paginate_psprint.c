/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998, 1999 Alexander Larsson
 *
 * paginate_psprint.[ch] -- pagination code for the postscript backend
 * Copyright (C) 1999 James Henstridge
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include "intl.h"
#include "message.h"
#include "diagram.h"
#include "diagramdata.h"
#include "render_eps.h"
#include "paginate_psprint.h"
#include "diapagelayout.h"

#include <gtk/gtk.h>

/* keep track of print options between prints */
typedef struct _dia_print_options {
    int printer;
    char *command;
    char *output;
} dia_print_options;

static dia_print_options last_print_options = 
{
    1, NULL, NULL
};

static void
count_objs(Object *obj, Renderer *renderer, int active_layer, guint *nobjs)
{
  (*nobjs)++;
}

static guint
print_page(DiagramData *data, RendererEPS *rend, Rectangle *bounds)
{
  guint nobjs = 0;
  gfloat tmargin = data->paper.tmargin, bmargin = data->paper.bmargin;
  gfloat lmargin = data->paper.lmargin, rmargin = data->paper.rmargin;
  gfloat scale = data->paper.scaling;

  /* count the number of objects in this region */
  data_render(data, (Renderer *)rend, bounds,
              (ObjectRenderer) count_objs, &nobjs);

  if (nobjs == 0)
    return nobjs;

  /* output a page number comment */
  fprintf(rend->file, "%%%%Page: %d %d\n", rend->pagenum, rend->pagenum);
  rend->pagenum++;

  /* save print context */
  fprintf(rend->file, "gs\n");

  /* transform coordinate system */
  if (data->paper.is_portrait) {
    fprintf(rend->file, "%f %f scale\n", 28.346457*scale, -28.346457*scale);
    fprintf(rend->file, "%f %f translate\n", lmargin/scale - bounds->left,
	    -bmargin/scale - bounds->bottom);
  } else {
    fprintf(rend->file, "90 rotate\n");
    fprintf(rend->file, "%f %f scale\n", 28.346457*scale, -28.346457*scale);
    fprintf(rend->file, "%f %f translate\n", lmargin/scale - bounds->left,
	    tmargin/scale - bounds->top);
  }

  /* set up clip mask */
  fprintf(rend->file, "n %f %f m ", bounds->left, bounds->top);
  fprintf(rend->file, "%f %f l ", bounds->right, bounds->top);
  fprintf(rend->file, "%f %f l ", bounds->right, bounds->bottom);
  fprintf(rend->file, "%f %f l ", bounds->left, bounds->bottom);
  fprintf(rend->file, "%f %f l ", bounds->left, bounds->top);
  fprintf(rend->file, "clip\n");

  /* render the region */
  data_render(data, (Renderer *)rend, bounds, NULL, NULL);

  /* restore print context */
  fprintf(rend->file, "gr\n");

  /* print the page */
  fprintf(rend->file, "showpage\n\n");

  return nobjs;
}

void
paginate_psprint(Diagram *dia, FILE *file)
{
  RendererEPS *rend;
  Rectangle *extents;
  gfloat width, height;
  gfloat x, y;
  guint nobjs = 0;

  rend = new_psprint_renderer(dia, file);

  /* the usable area of the page */
  width = dia->data->paper.width;
  height = dia->data->paper.height;

  /* get extents, and make them multiples of width / height */
  extents = &dia->data->extents;
  /*extents->left -= extents->left % width;
    extents->right += width - extents->right % width;
    extents->top -= extents->top % height;
    extents->bottom += height - extents->bottom % height;*/

  /* iterate through all the pages in the diagram */
  for (y = extents->top; y < extents->bottom; y += height)
    for (x = extents->left; x < extents->right; x += width) {
      Rectangle page_bounds;

      page_bounds.left = x;
      page_bounds.right = x + width;
      page_bounds.top = y;
      page_bounds.bottom = y + height;

      nobjs += print_page(dia->data,rend, &page_bounds);
    }

  destroy_eps_renderer(rend);
}

static void
change_entry_state(GtkToggleButton *radio, GtkWidget *entry)
{
  gtk_widget_set_sensitive(entry, gtk_toggle_button_get_active(radio));
}

static void
ok_pressed(GtkButton *button, gboolean *flag)
{
  *flag = TRUE;
  gtk_main_quit();
}

void
diagram_print_ps(Diagram *dia)
{
  GtkWidget *dialog;
  GtkWidget *vbox, *frame, *table, *box, *button;
  GtkWidget *iscmd, *isofile;
  GtkWidget *cmd, *ofile;
  gboolean cont = FALSE;
  
  FILE *file;
  gboolean is_pipe;

  /* create the dialog */
  dialog = gtk_dialog_new();
  gtk_signal_connect(GTK_OBJECT(dialog), "delete_event",
		     GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_signal_connect(GTK_OBJECT(dialog), "delete_event",
		     GTK_SIGNAL_FUNC(gtk_true), NULL);
  vbox = GTK_DIALOG(dialog)->vbox;

  frame = gtk_frame_new(_("Select Printer"));
  gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
  gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);
  gtk_widget_show(frame);

  table = gtk_table_new(2, 2, FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(table), 5);
  gtk_table_set_row_spacings(GTK_TABLE(table), 5);
  gtk_table_set_col_spacings(GTK_TABLE(table), 5);
  gtk_container_add(GTK_CONTAINER(frame), table);
  gtk_widget_show(table);

  iscmd = gtk_radio_button_new_with_label(NULL, _("Printer"));
  gtk_table_attach(GTK_TABLE(table), iscmd, 0,1, 0,1,
		   GTK_FILL, GTK_FILL|GTK_EXPAND, 0, 0);
  gtk_widget_show(iscmd);

  cmd = gtk_entry_new();
  gtk_table_attach(GTK_TABLE(table), cmd, 1,2, 0,1,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 0);
  gtk_widget_show(cmd);
  gtk_signal_connect(GTK_OBJECT(iscmd), "toggled",
		     GTK_SIGNAL_FUNC(change_entry_state), cmd);

  isofile = gtk_radio_button_new_with_label(GTK_RADIO_BUTTON(iscmd)->group,
					    _("File"));
  gtk_table_attach(GTK_TABLE(table), isofile, 0,1, 1,2,
		   GTK_FILL, GTK_FILL|GTK_EXPAND, 0, 0);
  gtk_widget_show(isofile);

  ofile = gtk_entry_new();
  gtk_widget_set_sensitive(ofile, FALSE);
  gtk_table_attach(GTK_TABLE(table), ofile, 1,2, 1,2,
		   GTK_FILL|GTK_EXPAND, GTK_FILL|GTK_EXPAND, 0, 0);
  gtk_widget_show(ofile);
  gtk_signal_connect(GTK_OBJECT(isofile), "toggled",
		     GTK_SIGNAL_FUNC(change_entry_state), ofile);

  box = GTK_DIALOG(dialog)->action_area;

  button = gtk_button_new_with_label(_("OK"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked", 
		     GTK_SIGNAL_FUNC(ok_pressed), &cont);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
  gtk_widget_grab_default(button);
  gtk_widget_show(button);

  button = gtk_button_new_with_label(_("Cancel"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked", 
		     GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  /* Set default or old dialog values: */
  gtk_entry_set_text(GTK_ENTRY(cmd), 
		     last_print_options.command 
		     ? last_print_options.command : "lpr");
  gtk_entry_set_text(GTK_ENTRY(ofile), 
		     last_print_options.output 
		     ? last_print_options.output : "output.ps");
  /* Scaling is already set at creation. */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(iscmd), last_print_options.printer);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(isofile), !last_print_options.printer);
  
  gtk_widget_show(dialog);
  gtk_main();

  if (!cont) {
    gtk_widget_destroy(dialog);
    return;
  }

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(iscmd))) {
    file = popen(gtk_entry_get_text(GTK_ENTRY(cmd)), "w");
    is_pipe = TRUE;
  } else {
    file = fopen(gtk_entry_get_text(GTK_ENTRY(ofile)), "w");
    is_pipe = FALSE;
  }

  /* Store dialog values */
  g_free( last_print_options.command );
  g_free( last_print_options.output );
  last_print_options.command = g_strdup( gtk_entry_get_text(GTK_ENTRY(cmd)) );
  last_print_options.output = g_strdup( gtk_entry_get_text(GTK_ENTRY(ofile)) );
  last_print_options.printer = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(iscmd));
  
  if (!file) {
    if (is_pipe)
      message_warning("Could not run command '%s'",
		      gtk_entry_get_text(GTK_ENTRY(cmd)));
    else
      message_warning("Could not open '%s' for writing",
		      gtk_entry_get_text(GTK_ENTRY(ofile)));
    return;
  }
  paginate_psprint(dia, file);
  gtk_widget_destroy(dialog);
  if (is_pipe)
    pclose(file);
  else
    fclose(file);
}
