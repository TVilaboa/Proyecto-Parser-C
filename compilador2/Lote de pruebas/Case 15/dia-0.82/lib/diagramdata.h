/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
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
#ifndef DIAGRAMDATA_H
#define DIAGRAMDATA_H

#include <glib.h>

typedef struct _DiagramData DiagramData;
typedef struct _PaperInfo PaperInfo;
typedef struct _Layer Layer;

#include "object.h"

struct _PaperInfo {
  gchar *name;      /* name of the paper */
  gfloat tmargin, bmargin, lmargin, rmargin; /* margin widths in centimeters */
  gboolean is_portrait;   /* page is in portrait orientation? */
  gfloat scaling;         /* scaling factor for image on page */

  gfloat width, height;   /* usable width/height -- calculated from paper type,
			   * margin widths and paper orientation */
};

struct _DiagramData {
  Rectangle extents;      /* The extents of the diagram        */

  Color bg_color;

  PaperInfo paper;       /* info about the page info for the diagram */

  GPtrArray *layers;     /* Layers ordered by decreasing z-order */
  Layer *active_layer;

  guint selected_count;
  GList *selected;        /* List of objects that are selected,
			     all from the active layer! */
};

struct _Layer {
  char *name;
  Rectangle extents;      /* The extents of the layer        */

  GList *objects;         /* List of objects in the layer,
			     sorted by decreasing z-valued,
			     objects can ONLY be connected to objects
			     in the same layer! */

  int visible;
};

extern DiagramData *new_diagram_data(void);
extern void diagram_data_destroy(DiagramData *data);

extern Layer *new_layer(char *name);
extern void layer_destroy(Layer *layer);

extern void data_raise_layer(DiagramData *data, Layer *layer);
extern void data_lower_layer(DiagramData *data, Layer *layer);

extern void data_add_layer(DiagramData *data, Layer *layer);
extern void data_add_layer_at(DiagramData *data, Layer *layer, int pos);
extern void data_set_active_layer(DiagramData *data, Layer *layer);
extern void data_delete_layer(DiagramData *data, Layer *layer);
extern void data_select(DiagramData *data, Object *obj);
extern void data_unselect(DiagramData *data, Object *obj);
extern void data_remove_all_selected(DiagramData *data);
extern int data_update_extents(DiagramData *data); /* returns true if changed. */
extern GList *data_get_sorted_selected(DiagramData *data);
extern GList *data_get_sorted_selected_remove(DiagramData *data);

typedef void (*ObjectRenderer)(Object *obj, Renderer *renderer,
			       int active_layer,
			       gpointer data);
extern void data_render(DiagramData *data, Renderer *renderer, Rectangle *update,
			ObjectRenderer obj_renderer /* Can be NULL */,
			gpointer gdata);  
extern void layer_render(Layer *layer, Renderer *renderer, Rectangle *update,
			 ObjectRenderer obj_renderer /* Can be NULL */,
			 gpointer data,
			 int active_layer);

extern int layer_object_index(Layer *layer, Object *obj);
extern void layer_add_object(Layer *layer, Object *obj);
extern void layer_add_object_at(Layer *layer, Object *obj, int pos);
extern void layer_add_objects(Layer *layer, GList *obj_list);
extern void layer_add_objects_first(Layer *layer, GList *obj_list);
extern void layer_remove_object(Layer *layer, Object *obj);
extern void layer_remove_objects(Layer *layer, GList *obj_list);
extern GList *layer_find_objects_in_rectangle(Layer *layer, Rectangle *rect);
extern Object *layer_find_closest_object(Layer *layer, Point *pos,
					 real maxdist);
extern real layer_find_closest_connectionpoint(Layer *layer,
					       ConnectionPoint **closest,
					       Point *pos);
extern int layer_update_extents(Layer *layer); /* returns true if changed. */
extern void layer_replace_object_with_list(Layer *layer, Object *obj,
					   GList *list);
extern void layer_set_object_list(Layer *layer, GList *list);
/* Make sure all objects that are in the layer and not in the new
   list eventually gets destroyed. */
#endif /* DIAGRAMDATA_H */






