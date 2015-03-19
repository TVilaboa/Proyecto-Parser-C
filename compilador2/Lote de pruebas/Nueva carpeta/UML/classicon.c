/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998,1999 Alexander Larsson
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
#include <stdio.h>

#include "config.h"
#include "intl.h"
#include "object.h"
#include "element.h"
#include "render.h"
#include "attributes.h"
#include "text.h"

#include "uml.h"

#include "pixmaps/classicon.xpm"

typedef struct _Classicon Classicon;
typedef struct _ClassiconState ClassiconState;
typedef struct _ClassiconPropertiesDialog ClassiconPropertiesDialog;

struct _ClassiconState {
  ObjectState obj_state;
  
  int stereotype;
  int is_object;
};

struct _Classicon {
  Element element;

  ConnectionPoint connections[8];
  
  int stereotype;
  int is_object;
  Text *text;
};

struct _ClassiconPropertiesDialog {
  GtkWidget *dialog;
  GtkWidget *m_control;
  GtkWidget *m_bound;
  GtkWidget *m_entity;  
  GtkWidget *is_object;
};


enum CLassIconStereotype {
    CLASSICON_CONTROL,
    CLASSICON_BOUNDARY,
    CLASSICON_ENTITY
};


#define CLASSICON_LINEWIDTH 0.1
#define CLASSICON_RADIOUS 1
#define CLASSICON_FONTHEIGHT 0.8
#define CLASSICON_MARGIN 0.5
#define CLASSICON_AIR 0.25
#define CLASSICON_ARROW 0.4

static ClassiconPropertiesDialog* properties_dialog = NULL;

static real classicon_distance_from(Classicon *cicon, Point *point);
static void classicon_select(Classicon *cicon, Point *clicked_point,
			     Renderer *interactive_renderer);
static void classicon_move_handle(Classicon *cicon, Handle *handle,
				  Point *to, HandleMoveReason reason, ModifierKeys modifiers);
static void classicon_move(Classicon *cicon, Point *to);
static void classicon_draw(Classicon *cicon, Renderer *renderer);
static Object *classicon_create(Point *startpoint,
				void *user_data,
				Handle **handle1,
				Handle **handle2);
static void classicon_destroy(Classicon *cicon);
static Object *classicon_copy(Classicon *cicon);
static void classicon_save(Classicon *cicon, ObjectNode obj_node,
			   const char *filename);
static Object *classicon_load(ObjectNode obj_node, int version,
			      const char *filename);
static void classicon_update_data(Classicon *cicon);
static GtkWidget *classicon_get_properties(Classicon *cicon);
static ObjectChange *classicon_apply_properties(Classicon *cicon);
static ClassiconState *classicon_get_state(Classicon *cicon);
static void classicon_set_state(Classicon *cicon,
				ClassiconState *state);


static ObjectTypeOps classicon_type_ops =
{
  (CreateFunc) classicon_create,
  (LoadFunc)   classicon_load,
  (SaveFunc)   classicon_save
};

ObjectType classicon_type =
{
  "UML - Classicon",   /* name */
  0,                      /* version */
  (char **) classicon_xpm,  /* pixmap */
  
  &classicon_type_ops       /* ops */
};

static ObjectOps classicon_ops = {
  (DestroyFunc)         classicon_destroy,
  (DrawFunc)            classicon_draw,
  (DistanceFunc)        classicon_distance_from,
  (SelectFunc)          classicon_select,
  (CopyFunc)            classicon_copy,
  (MoveFunc)            classicon_move,
  (MoveHandleFunc)      classicon_move_handle,
  (GetPropertiesFunc)   classicon_get_properties,
  (ApplyPropertiesFunc) classicon_apply_properties,
  (ObjectMenuFunc)      NULL
};

static real
classicon_distance_from(Classicon *cicon, Point *point)
{
  Object *obj = &cicon->element.object;
  return distance_rectangle_point(&obj->bounding_box, point);
}

static void
classicon_select(Classicon *cicon, Point *clicked_point,
		 Renderer *interactive_renderer)
{
  text_set_cursor(cicon->text, clicked_point, interactive_renderer);
  text_grab_focus(cicon->text, (Object *)cicon);
  element_update_handles(&cicon->element);
}

static void
classicon_move_handle(Classicon *cicon, Handle *handle,
		      Point *to, HandleMoveReason reason, ModifierKeys modifiers)
{
  assert(cicon!=NULL);
  assert(handle!=NULL);
  assert(to!=NULL);

  assert(handle->id < 8);
}

static void
classicon_move(Classicon *cicon, Point *to)
{
  Element *elem = &cicon->element;
  
  elem->corner = *to;
  elem->corner.x -= elem->width/2.0; 
  elem->corner.y -= CLASSICON_RADIOUS + CLASSICON_ARROW;

  if (cicon->stereotype==CLASSICON_BOUNDARY)
    elem->corner.x -= CLASSICON_RADIOUS/2.0;

  classicon_update_data(cicon);
}

static void
classicon_draw(Classicon *icon, Renderer *renderer)
{
  Element *elem;
  real r, x, y, w, h;
  Point center, p1, p2;
  int i;
  
  assert(icon != NULL);
  assert(renderer != NULL);

  elem = &icon->element;

  x = elem->corner.x;
  y = elem->corner.y;
  w = elem->width;
  h = elem->height;
  
  r = CLASSICON_RADIOUS;
  center.x = x + elem->width/2;
  center.y = y + r + CLASSICON_ARROW;

  if (icon->stereotype==CLASSICON_BOUNDARY)
      center.x += r/2.0;

  renderer->ops->set_fillstyle(renderer, FILLSTYLE_SOLID);

  renderer->ops->fill_ellipse(renderer,
			      &center,
			      2*r, 2*r,
			      &color_white);

  renderer->ops->set_linewidth(renderer, CLASSICON_LINEWIDTH);
  renderer->ops->set_linestyle(renderer, LINESTYLE_SOLID);

  renderer->ops->draw_ellipse(renderer,
			      &center,
			      2*r, 2*r,
			      &color_black);


  switch (icon->stereotype) {
  case CLASSICON_CONTROL:
      p1.x = center.x - r*0.258819045102521;
      p1.y = center.y-r*0.965925826289068;

      p2.x = p1.x + CLASSICON_ARROW;
      p2.y = p1.y + CLASSICON_ARROW/1.5;
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      p2.x = p1.x + CLASSICON_ARROW;
      p2.y = p1.y - CLASSICON_ARROW/1.5;
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      break;

  case CLASSICON_BOUNDARY:
      p1.x = center.x - r;
      p2.x = p1.x - r;
      p1.y = p2.y = center.y;
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      p1.x = p2.x;
      p1.y = center.y - r;
      p2.y = center.y + r;
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      break;
  case CLASSICON_ENTITY:
      p1.x = center.x - r;
      p2.x = center.x + r;
      p1.y = p2.y = center.y + r;
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      break;
  }
  
  text_draw(icon->text, renderer);

  if (icon->is_object) {
    renderer->ops->set_linewidth(renderer, 0.01);
    if (icon->stereotype==CLASSICON_BOUNDARY)
      x += r/2.0;
    p1.y = p2.y = icon->text->position.y + icon->text->descent;
    for (i=0; i<icon->text->numlines; i++) { 
      p1.x = x + (w - icon->text->row_width[i])/2;
      p2.x = p1.x + icon->text->row_width[i];
      renderer->ops->draw_line(renderer,
			       &p1, &p2,
			       &color_black);
      p1.y = p2.y += icon->text->height;
    }
  }
}

static void
classicon_update_data(Classicon *cicon)
{
  Element *elem = &cicon->element;
  Object *obj = (Object *) cicon;
  Font *font;
  Point p1;
  real h, wt, w = 0;
  int is_boundary = (cicon->stereotype==CLASSICON_BOUNDARY);
	
  font = cicon->text->font;
  h = CLASSICON_AIR + CLASSICON_MARGIN + CLASSICON_ARROW + 2*CLASSICON_RADIOUS;

  w = 2*CLASSICON_RADIOUS;
  wt = cicon->text->max_width;

  if (cicon->stereotype==CLASSICON_BOUNDARY) {
      w += 2*CLASSICON_RADIOUS;
      wt += CLASSICON_RADIOUS;
  }

  w = MAX(w, wt) + CLASSICON_AIR;

  p1.y = h + elem->corner.y; 
  h += cicon->text->height*cicon->text->numlines + CLASSICON_AIR;
  
  p1.y += cicon->text->ascent;
  p1.x = elem->corner.x + w/2.0;
  if (cicon->stereotype==CLASSICON_BOUNDARY)
      p1.x += CLASSICON_RADIOUS/2.0;
  text_set_position(cicon->text, &p1);
    
  elem->width = w;
  elem->height = h;
	
  p1.x = elem->corner.x + elem->width / 2.0;
  p1.y = elem->corner.y + CLASSICON_RADIOUS + CLASSICON_ARROW;
  w = CLASSICON_RADIOUS + CLASSICON_ARROW;
  h = (CLASSICON_RADIOUS + CLASSICON_ARROW) * M_SQRT1_2;

  if (is_boundary)
    p1.x += CLASSICON_RADIOUS/2.0;
	
  /* Update connections: */
  cicon->connections[0].pos.x = (is_boundary) ? p1.x-2*w: p1.x - h;
  cicon->connections[0].pos.y = (is_boundary) ? elem->corner.y: p1.y - h;
  cicon->connections[1].pos.x = p1.x;
  cicon->connections[1].pos.y = p1.y - w;
  cicon->connections[2].pos.x = p1.x + h;
  cicon->connections[2].pos.y = p1.y - h; 
	
  cicon->connections[3].pos.x = (is_boundary) ? p1.x-2*w: p1.x - w;
  cicon->connections[3].pos.y = p1.y;
  cicon->connections[4].pos.x = p1.x + w;
  cicon->connections[4].pos.y = p1.y;
  cicon->connections[5].pos.x = elem->corner.x;
  cicon->connections[5].pos.y = elem->corner.y + elem->height;
  cicon->connections[6].pos.x = p1.x;
  cicon->connections[6].pos.y = elem->corner.y + elem->height;
  cicon->connections[7].pos.x = elem->corner.x + elem->width;
  cicon->connections[7].pos.y = elem->corner.y + elem->height;
  
  element_update_boundingbox(elem);

  /* fix boundingclassicon for line width and top rectangle: */
  /*  obj->bounding_box.top -= CLASSICON_BORDERWIDTH/2.0;
  obj->bounding_box.left -= CLASSICON_BORDERWIDTH/2.0;
  obj->bounding_box.bottom += CLASSICON_BORDERWIDTH/2.0;
  obj->bounding_box.right += CLASSICON_BORDERWIDTH/2.0;
  */
  obj->position = elem->corner;
  obj->position.x += (elem->width + ((is_boundary)?CLASSICON_RADIOUS:0))/2.0;
  obj->position.y += CLASSICON_RADIOUS + CLASSICON_ARROW;

  element_update_handles(elem);
}

static Object *
classicon_create(Point *startpoint,
		    void *user_data,
		    Handle **handle1,
		    Handle **handle2)
{
  Classicon *cicon;
  Element *elem;
  Object *obj;
  Point p;
  Font *font;
  int i;
  
  cicon = g_malloc(sizeof(Classicon));
  elem = &cicon->element;
  obj = (Object *) cicon;
  
  obj->type = &classicon_type;

  obj->ops = &classicon_ops;

  elem->corner = *startpoint;

  font = font_getfont("Helvetica");
  
  cicon->stereotype = 0;
  cicon->is_object = 0;

  /* The text position is recalculated later */
  p.x = 0.0;
  p.y = 0.0;
  cicon->text = new_text("", font, 0.8, &p, &color_black, ALIGN_CENTER);
  
  element_init(elem, 8, 8);
  
  for (i=0;i<8;i++) {
    obj->connections[i] = &cicon->connections[i];
    cicon->connections[i].object = obj;
    cicon->connections[i].connected = NULL;
  }
  classicon_update_data(cicon);

  for (i=0;i<8;i++) {
    obj->handles[i]->type = HANDLE_NON_MOVABLE;
  }

  *handle1 = NULL;
  *handle2 = NULL;

  return (Object *)cicon;
}

static void
classicon_destroy(Classicon *cicon)
{
  text_destroy(cicon->text);
  element_destroy(&cicon->element);
}

static Object *
classicon_copy(Classicon *cicon)
{
  int i;
  Classicon *newcicon;
  Element *elem, *newelem;
  Object *newobj;
  
  elem = &cicon->element;
  
  newcicon = g_malloc(sizeof(Classicon));
  newelem = &newcicon->element;
  newobj = (Object *) newcicon;

  element_copy(elem, newelem);

  newcicon->text = text_copy(cicon->text);
  
  for (i=0;i<8;i++) {
    newobj->connections[i] = &newcicon->connections[i];
    newcicon->connections[i].object = newobj;
    newcicon->connections[i].connected = NULL;
    newcicon->connections[i].pos = cicon->connections[i].pos;
    newcicon->connections[i].last_pos = cicon->connections[i].last_pos;
  }

  newcicon->stereotype = cicon->stereotype;
  newcicon->is_object = cicon->is_object;

  classicon_update_data(newcicon);
  
  return (Object *)newcicon;
}

static ClassiconState *
classicon_get_state(Classicon *cicon)
{
  ClassiconState *state = g_new(ClassiconState, 1);

  state->obj_state.free = NULL;

  state->stereotype = cicon->stereotype;
  state->is_object = cicon->is_object;

  return state;
}

static void
classicon_set_state(Classicon *cicon, ClassiconState *state)
{
  cicon->stereotype = state->stereotype;
  cicon->is_object = state->is_object;
  
  g_free(state);
  
  classicon_update_data(cicon);
}

static void
classicon_save(Classicon *cicon, ObjectNode obj_node, const char *filename)
{
  element_save(&cicon->element, obj_node);

  data_add_text(new_attribute(obj_node, "text"),
		cicon->text);

  data_add_int(new_attribute(obj_node, "stereotype"),
		   cicon->stereotype);

  data_add_boolean(new_attribute(obj_node, "is_object"),
		   cicon->is_object);
}

static Object *
classicon_load(ObjectNode obj_node, int version, const char *filename)
{
  Classicon *cicon;
  AttributeNode attr;
  Element *elem;
  Object *obj;
  int i;
  
  cicon = g_malloc(sizeof(Classicon));
  elem = &cicon->element;
  obj = (Object *) cicon;
  
  obj->type = &classicon_type;
  obj->ops = &classicon_ops;

  element_load(elem, obj_node);
  
  cicon->text = NULL;
  attr = object_find_attribute(obj_node, "text");
  if (attr != NULL)
    cicon->text = data_text(attribute_first_data(attr));

  cicon->stereotype = 0;
  attr = object_find_attribute(obj_node, "stereotype");
  if (attr != NULL)
      cicon->stereotype = data_int(attribute_first_data(attr));

  cicon->is_object = 0;
  attr = object_find_attribute(obj_node, "is_object");
  if (attr != NULL)
      cicon->is_object = data_boolean(attribute_first_data(attr));

  element_init(elem, 8, 8);

  for (i=0;i<8;i++) {
    obj->connections[i] = &cicon->connections[i];
    cicon->connections[i].object = obj;
    cicon->connections[i].connected = NULL;
  }
  classicon_update_data(cicon);

  for (i=0;i<8;i++) {
    obj->handles[i]->type = HANDLE_NON_MOVABLE;
  }

  return (Object *) cicon;
}


static ObjectChange *
classicon_apply_properties(Classicon *cicon)
{
  ClassiconPropertiesDialog *prop_dialog;
  ObjectState *old_state;

  prop_dialog = properties_dialog;

  old_state = (ObjectState *)classicon_get_state(cicon);

  cicon->is_object = GTK_TOGGLE_BUTTON(prop_dialog->is_object)->active;

  /* Read from dialog and put in object: */
  if (GTK_TOGGLE_BUTTON(prop_dialog->m_control)->active) {
      cicon->stereotype = CLASSICON_CONTROL;
  } else if (GTK_TOGGLE_BUTTON(prop_dialog->m_bound)->active) {
    cicon->stereotype = CLASSICON_BOUNDARY;
  } else if (GTK_TOGGLE_BUTTON(prop_dialog->m_entity)->active) {
    cicon->stereotype = CLASSICON_ENTITY;
  }

  classicon_update_data(cicon);

  return new_object_state_change((Object *)cicon, old_state, 
				 (GetStateFunc)classicon_get_state,
				 (SetStateFunc)classicon_set_state);
}

static void
fill_in_dialog(Classicon *cicon)
{
  ClassiconPropertiesDialog *prop_dialog;
  GtkToggleButton *button = NULL;

  prop_dialog = properties_dialog;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(prop_dialog->is_object), 
			       cicon->is_object);

  switch (cicon->stereotype) {
  case CLASSICON_CONTROL:
      button = GTK_TOGGLE_BUTTON(prop_dialog->m_control);
      break;
  case CLASSICON_BOUNDARY:
      button = GTK_TOGGLE_BUTTON(prop_dialog->m_bound);
      break;
  case CLASSICON_ENTITY:
      button = GTK_TOGGLE_BUTTON(prop_dialog->m_entity);
      break;
  }
  gtk_toggle_button_set_active(button, TRUE);
}

static GtkWidget *
classicon_get_properties(Classicon *cicon)
{
  ClassiconPropertiesDialog *prop_dialog;
  GtkWidget *dialog;
  GtkWidget *label;
  GSList *group;

  if (properties_dialog == NULL) {

    prop_dialog = g_new(ClassiconPropertiesDialog, 1);
    properties_dialog = prop_dialog;

    dialog = gtk_vbox_new(FALSE, 0);
    prop_dialog->dialog = dialog;
    
    prop_dialog->m_control = gtk_radio_button_new_with_label (NULL, _("Control"));
    gtk_box_pack_start (GTK_BOX (dialog), prop_dialog->m_control, TRUE, TRUE, 0);
    gtk_widget_show (prop_dialog->m_control);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prop_dialog->m_control), TRUE);

    group = gtk_radio_button_group (GTK_RADIO_BUTTON (prop_dialog->m_control));
    prop_dialog->m_bound = gtk_radio_button_new_with_label(group, _("Boundary"));
    gtk_box_pack_start (GTK_BOX (dialog), prop_dialog->m_bound, TRUE, TRUE, 0);
    gtk_widget_show (prop_dialog->m_bound);

    group = gtk_radio_button_group (GTK_RADIO_BUTTON (prop_dialog->m_bound));
    prop_dialog->m_entity = gtk_radio_button_new_with_label(group, _("Entity"));
    gtk_box_pack_start (GTK_BOX (dialog), prop_dialog->m_entity, TRUE, TRUE, 0);
    gtk_widget_show (prop_dialog->m_entity);

    label = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (dialog), label, FALSE, TRUE, 0);
    gtk_widget_show (label);

    prop_dialog->is_object = gtk_check_button_new_with_label(_("Is an object"));
    gtk_widget_show(prop_dialog->is_object);
    gtk_box_pack_start (GTK_BOX (dialog), prop_dialog->is_object, TRUE, TRUE, 0);
  }
  
  fill_in_dialog(cicon);
  gtk_widget_show (properties_dialog->dialog);

  return properties_dialog->dialog;
}

