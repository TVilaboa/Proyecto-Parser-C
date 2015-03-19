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
#include <assert.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "config.h"
#include "intl.h"
#include "object.h"
#include "element.h"
#include "render.h"
#include "attributes.h"
#include "text.h"

#include "pixmaps/case.xpm"

typedef struct _UsecasePropertiesDialog UsecasePropertiesDialog;
typedef struct _Usecase Usecase;
typedef struct _UsecaseState UsecaseState;

struct _UsecaseState {
  ObjectState obj_state;
  
  int text_outside;
  int collaboration;
};

struct _Usecase {
  Element element;

  ConnectionPoint connections[8];

  Text *text;
  int text_outside;
  int collaboration;
};


struct _UsecasePropertiesDialog {
  GtkWidget *dialog;
  
  GtkToggleButton *text_out;
  GtkToggleButton *collaboration;
};


#define USECASE_WIDTH 3.25
#define USECASE_HEIGHT 2
#define USECASE_MIN_RATIO 1.5
#define USECASE_MAX_RATIO 3
#define USECASE_LINEWIDTH 0.1
#define USECASE_MARGIN_Y 0.3

static UsecasePropertiesDialog *properties_dialog;

static real usecase_distance_from(Usecase *usecase, Point *point);
static void usecase_select(Usecase *usecase, Point *clicked_point,
			   Renderer *interactive_renderer);
static void usecase_move_handle(Usecase *usecase, Handle *handle,
				Point *to, HandleMoveReason reason, ModifierKeys modifiers);
static void usecase_move(Usecase *usecase, Point *to);
static void usecase_draw(Usecase *usecase, Renderer *renderer);
static Object *usecase_create(Point *startpoint,
			      void *user_data,
			      Handle **handle1,
			      Handle **handle2);
static void usecase_destroy(Usecase *usecase);
static Object *usecase_copy(Usecase *usecase);

static void usecase_save(Usecase *usecase, ObjectNode obj_node,
			 const char *filename);
static Object *usecase_load(ObjectNode obj_node, int version,
			    const char *filename);
static void usecase_update_data(Usecase *usecase);
static ObjectChange *usecase_apply_properties(Usecase *usecase);
static GtkWidget *usecase_get_properties(Usecase *dep);


static ObjectTypeOps usecase_type_ops =
{
  (CreateFunc) usecase_create,
  (LoadFunc)   usecase_load,
  (SaveFunc)   usecase_save
};

ObjectType usecase_type =
{
  "UML - Usecase",   /* name */
  0,                      /* version */
  (char **) case_xpm,  /* pixmap */
  
  &usecase_type_ops       /* ops */
};

static ObjectOps usecase_ops = {
  (DestroyFunc)         usecase_destroy,
  (DrawFunc)            usecase_draw,
  (DistanceFunc)        usecase_distance_from,
  (SelectFunc)          usecase_select,
  (CopyFunc)            usecase_copy,
  (MoveFunc)            usecase_move,
  (MoveHandleFunc)      usecase_move_handle,
  (GetPropertiesFunc)   usecase_get_properties,
  (ApplyPropertiesFunc) usecase_apply_properties,
  (ObjectMenuFunc)      NULL
};

static real
usecase_distance_from(Usecase *usecase, Point *point)
{
  Object *obj = &usecase->element.object;
  return distance_rectangle_point(&obj->bounding_box, point);
}

static void
usecase_select(Usecase *usecase, Point *clicked_point,
	       Renderer *interactive_renderer)
{
  text_set_cursor(usecase->text, clicked_point, interactive_renderer);
  text_grab_focus(usecase->text, (Object *)usecase);
  element_update_handles(&usecase->element);
}

static void
usecase_move_handle(Usecase *usecase, Handle *handle,
		 Point *to, HandleMoveReason reason, ModifierKeys modifiers)
{
  assert(usecase!=NULL);
  assert(handle!=NULL);
  assert(to!=NULL);

  assert(handle->id < 8);
}

static void
usecase_move(Usecase *usecase, Point *to)
{
  real h;
  Point p;
  
  usecase->element.corner = *to;
  h = usecase->text->height*usecase->text->numlines;
  
  p = *to;
  p.x += usecase->element.width/2.0;
  if (usecase->text_outside) {
      p.y += usecase->element.height - h + usecase->text->ascent;
  } else {
      p.y += (usecase->element.height - h)/2.0 + usecase->text->ascent;
  }
  text_set_position(usecase->text, &p);
  usecase_update_data(usecase);
}

static void
usecase_draw(Usecase *usecase, Renderer *renderer)
{
  Element *elem;
  real x, y, w, h;
  Point c;

  assert(usecase != NULL);
  assert(renderer != NULL);

  elem = &usecase->element;

  x = elem->corner.x;
  y = elem->corner.y;

  if (usecase->text_outside) {
      w = USECASE_WIDTH;
      h = USECASE_HEIGHT;
      c.x = x + elem->width/2.0;
      c.y = y + USECASE_HEIGHT / 2.0;     
   } else {
      w = elem->width;
      h = elem->height;
      c.x = x + w/2.0;
      c.y = y + h/2.0;    
  }


  renderer->ops->set_fillstyle(renderer, FILLSTYLE_SOLID);
  renderer->ops->set_linewidth(renderer, USECASE_LINEWIDTH);

  if (usecase->collaboration)
	  renderer->ops->set_linestyle(renderer, LINESTYLE_DASHED);
  else 
	  renderer->ops->set_linestyle(renderer, LINESTYLE_SOLID);

  renderer->ops->fill_ellipse(renderer, 
			     &c,
			     w, h,
			     &color_white);
  renderer->ops->draw_ellipse(renderer, 
			     &c,
			     w, h,
			     &color_black);  
  
  text_draw(usecase->text, renderer);
}


static void
usecase_update_data(Usecase *usecase)
{
  real w, h, ratio;
  Point c, half, r;
  
  Element *elem = &usecase->element;
  Object *obj = (Object *) usecase;
  
  w = usecase->text->max_width;
  h = usecase->text->height*usecase->text->numlines;

  if (!usecase->text_outside) { 
      ratio = w/h;

      if (ratio > USECASE_MAX_RATIO) 
	  ratio = USECASE_MAX_RATIO;
  
      if (ratio < USECASE_MIN_RATIO) {
	  ratio = USECASE_MIN_RATIO;
	  r.y = w / ratio + h;
	  r.x = r.y * ratio;
      } else {
	  r.x = ratio*h + w;
	  r.y = r.x / ratio;
      }
      if (r.x < USECASE_WIDTH) 
	      r.x = USECASE_WIDTH;
      if (r.y < USECASE_HEIGHT)
	      r.y = USECASE_HEIGHT;
  } else {
      r.x = USECASE_WIDTH;
      r.y = USECASE_HEIGHT;
  }

  elem->width = r.x;
  elem->height = r.y;

  if (usecase->text_outside) { 
	  elem->width = MAX(elem->width, w);
	  elem->height += h + USECASE_MARGIN_Y;
  }

  r.x /= 2.0;
  r.y /= 2.0;
  c.x = elem->corner.x + elem->width / 2.0;
  c.y = elem->corner.y + r.y;
  half.x = r.x * M_SQRT1_2;
  half.y = r.y * M_SQRT1_2;

  /* Update connections: */
  usecase->connections[0].pos.x = c.x - half.x;
  usecase->connections[0].pos.y = c.y - half.y;
  usecase->connections[1].pos.x = c.x;
  usecase->connections[1].pos.y = elem->corner.y;
  usecase->connections[2].pos.x = c.x + half.x;
  usecase->connections[2].pos.y = c.y - half.y;
  usecase->connections[3].pos.x = c.x - r.x;
  usecase->connections[3].pos.y = c.y;
  usecase->connections[4].pos.x = c.x + r.x;
  usecase->connections[4].pos.y = c.y;
  
  if (usecase->text_outside) { 
      usecase->connections[5].pos.x = elem->corner.x;
      usecase->connections[5].pos.y = elem->corner.y + elem->height;
      usecase->connections[6].pos.x = c.x;
      usecase->connections[6].pos.y = elem->corner.y + elem->height;
      usecase->connections[7].pos.x = elem->corner.x + elem->width;
      usecase->connections[7].pos.y = elem->corner.y + elem->height;
  } else {
      usecase->connections[5].pos.x = c.x - half.x;
      usecase->connections[5].pos.y = c.y + half.y;
      usecase->connections[6].pos.x = c.x;
      usecase->connections[6].pos.y = elem->corner.y + elem->height;
      usecase->connections[7].pos.x = c.x + half.x;
      usecase->connections[7].pos.y = c.y + half.y;
  }

  element_update_boundingbox(elem);

  obj->position = elem->corner;

  element_update_handles(elem);
}

static Object *
usecase_create(Point *startpoint,
	       void *user_data,
  	       Handle **handle1,
	       Handle **handle2)
{
  Usecase *usecase;
  Element *elem;
  Object *obj;
  Point p;
  Font *font;
  int i;
  
  usecase = g_malloc(sizeof(Usecase));
  elem = &usecase->element;
  obj = (Object *) usecase;
  
  obj->type = &usecase_type;
  obj->ops = &usecase_ops;
  elem->corner = *startpoint;
  elem->width = USECASE_WIDTH;
  elem->height = USECASE_HEIGHT;

  font = font_getfont("Helvetica");
  p = *startpoint;
  p.x += USECASE_WIDTH/2.0;
  p.y += USECASE_HEIGHT/2.0;
  
  usecase->text = new_text("", font, 0.8, &p, &color_black, ALIGN_CENTER);
  usecase->text_outside = 0;
  usecase->collaboration = 0;
  element_init(elem, 8, 8);
  
  for (i=0;i<8;i++) {
    obj->connections[i] = &usecase->connections[i];
    usecase->connections[i].object = obj;
    usecase->connections[i].connected = NULL;
  }
  usecase_update_data(usecase);

  for (i=0;i<8;i++) {
    obj->handles[i]->type = HANDLE_NON_MOVABLE;
  }

  *handle1 = NULL;
  *handle2 = NULL;
  return (Object *)usecase;
}

static void
usecase_destroy(Usecase *usecase)
{
  text_destroy(usecase->text);

  element_destroy(&usecase->element);
}

static Object *
usecase_copy(Usecase *usecase)
{
  int i;
  Usecase *newusecase;
  Element *elem, *newelem;
  Object *newobj;
  
  elem = &usecase->element;
  
  newusecase = g_malloc(sizeof(Usecase));
  newelem = &newusecase->element;
  newobj = (Object *) newusecase;

  element_copy(elem, newelem);

  newusecase->text = text_copy(usecase->text);
  newusecase->text_outside = usecase->text_outside;
  newusecase->collaboration = usecase->collaboration;
  
  for (i=0;i<8;i++) {
    newobj->connections[i] = &newusecase->connections[i];
    newusecase->connections[i].object = newobj;
    newusecase->connections[i].connected = NULL;
    newusecase->connections[i].pos = usecase->connections[i].pos;
    newusecase->connections[i].last_pos = usecase->connections[i].last_pos;
  }

  newusecase->text_outside = usecase->text_outside;
  newusecase->collaboration = usecase->collaboration;
  usecase_update_data(newusecase);
  
  return (Object *)newusecase;
}

static UsecaseState *
usecase_get_state(Usecase *usecase)
{
  UsecaseState *state = g_new(UsecaseState, 1);

  state->obj_state.free = NULL;

  state->text_outside = usecase->text_outside;
  state->collaboration = usecase->collaboration;

  return state;
}

static void
usecase_set_state(Usecase *usecase, UsecaseState *state)
{
  usecase->text_outside = state->text_outside;
  usecase->collaboration = state->collaboration;
  
  g_free(state);
  
  usecase_update_data(usecase);
}

static void
usecase_save(Usecase *usecase, ObjectNode obj_node, const char *filename)
{
  element_save(&usecase->element, obj_node);

  data_add_text(new_attribute(obj_node, "text"),
		usecase->text);

  data_add_boolean(new_attribute(obj_node, "textout"),
		   usecase->text_outside);

  data_add_boolean(new_attribute(obj_node, "collaboration"),
		   usecase->collaboration);
}

static Object *
usecase_load(ObjectNode obj_node, int version, const char *filename)
{
  Usecase *usecase;
  Element *elem;
  Object *obj;
  int i;
  AttributeNode attr;

  usecase = g_malloc(sizeof(Usecase));
  elem = &usecase->element;
  obj = (Object *) usecase;
  
  obj->type = &usecase_type;
  obj->ops = &usecase_ops;

  element_load(elem, obj_node);
  attr = object_find_attribute(obj_node, "text");
  if (attr != NULL)
      usecase->text = data_text(attribute_first_data(attr));


  attr = object_find_attribute(obj_node, "textout");
  if (attr != NULL)
    usecase->text_outside = data_boolean(attribute_first_data(attr));
  else
    usecase->text_outside = 0;

  attr = object_find_attribute(obj_node, "collaboration");
  if (attr != NULL)
    usecase->collaboration = data_boolean(attribute_first_data(attr));
  else
    usecase->collaboration = 0;
  
  element_init(elem, 8, 8);

  for (i=0;i<8;i++) {
    obj->connections[i] = &usecase->connections[i];
    usecase->connections[i].object = obj;
    usecase->connections[i].connected = NULL;
  }
  usecase_update_data(usecase);

  for (i=0;i<8;i++) {
    obj->handles[i]->type = HANDLE_NON_MOVABLE;
  }

  return (Object *)usecase;
}

static ObjectChange *
usecase_apply_properties(Usecase *usecase)
{
  UsecasePropertiesDialog *prop_dialog;
  ObjectState *old_state;
  real h;
  Point p;

  prop_dialog = properties_dialog;

  old_state = (ObjectState *)usecase_get_state(usecase);

  usecase->text_outside = prop_dialog->text_out->active;
  usecase->collaboration = prop_dialog->collaboration->active;
  usecase_update_data(usecase);

  h = usecase->text->height*usecase->text->numlines;
  p = usecase->element.corner;
  p.x += usecase->element.width/2.0;
  if (usecase->text_outside) {
      p.y += usecase->element.height - h + usecase->text->ascent;
  } else {
      p.y += (usecase->element.height - h)/2.0 + usecase->text->ascent;
  }
  text_set_position(usecase->text, &p);
  return new_object_state_change((Object *)usecase, old_state, 
				 (GetStateFunc)usecase_get_state,
				 (SetStateFunc)usecase_set_state);
}

static void
fill_in_dialog(Usecase *usecase)
{
  UsecasePropertiesDialog *prop_dialog;
  prop_dialog = properties_dialog;
  gtk_toggle_button_set_active(prop_dialog->text_out, usecase->text_outside);
  gtk_toggle_button_set_active(prop_dialog->collaboration, usecase->collaboration);


}

static GtkWidget *
usecase_get_properties(Usecase *dep)
{
  UsecasePropertiesDialog *prop_dialog;
  GtkWidget *dialog;
  GtkWidget *checkbox;

  if (properties_dialog == NULL) {

    prop_dialog = g_new(UsecasePropertiesDialog, 1);
    properties_dialog = prop_dialog;

    dialog = gtk_vbox_new(FALSE, 0);
    prop_dialog->dialog = dialog;
    
    checkbox = gtk_check_button_new_with_label(_("Text outside:"));
    prop_dialog->text_out = GTK_TOGGLE_BUTTON( checkbox );
    gtk_widget_show(checkbox);
    gtk_box_pack_start (GTK_BOX (dialog), checkbox, TRUE, TRUE, 0);

    checkbox = gtk_check_button_new_with_label(_("Collaboration"));
    prop_dialog->collaboration = GTK_TOGGLE_BUTTON( checkbox );
    gtk_widget_show(checkbox);
    gtk_box_pack_start (GTK_BOX (dialog), checkbox, TRUE, TRUE, 0);
  }
  
  fill_in_dialog(dep);
  gtk_widget_show (properties_dialog->dialog);

  return properties_dialog->dialog;
}
