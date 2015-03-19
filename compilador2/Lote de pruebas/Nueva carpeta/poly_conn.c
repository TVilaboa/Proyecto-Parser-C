/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1999 Alexander Larsson
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
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "poly_conn.h"
#include "message.h"

enum change_type {
  TYPE_ADD_POINT,
  TYPE_REMOVE_POINT
};

struct PointChange {
  ObjectChange obj_change;

  enum change_type type;
  int applied;
  
  Point point;
  int pos;

  Handle *handle; /* owning ref when not applied for ADD_POINT
		     owning ref when applied for REMOVE_POINT */
  ConnectionPoint *connected_to; /* NULL if not connected */
};

static ObjectChange *
polyconn_create_change(PolyConn *poly, enum change_type type,
		       Point *point, int segment, Handle *handle,
		       ConnectionPoint *connected_to);

static void setup_corner_handle(Handle *handle)
{
  handle->id = HANDLE_CORNER;
  handle->type = HANDLE_MINOR_CONTROL;
  handle->connect_type = HANDLE_CONNECTABLE;
  handle->connected_to = NULL;
}


static int get_handle_nr(PolyConn *poly, Handle *handle)
{
  int i = 0;
  for (i=0;i<poly->numpoints;i++) {
    if (poly->object.handles[i] == handle)
      return i;
  }
  return -1;
}

void
polyconn_move_handle(PolyConn *poly, Handle *handle,
		     Point *to, HandleMoveReason reason)
{
  int handle_nr;
  
  handle_nr = get_handle_nr(poly, handle);
  switch(handle->id) {
  case HANDLE_MOVE_STARTPOINT:
    poly->points[0] = *to;
    break;
  case HANDLE_MOVE_ENDPOINT:
    poly->points[poly->numpoints-1] = *to;
    break;
  case HANDLE_CORNER:
    poly->points[handle_nr] = *to;
    break;
  default:
    message_error("Internal error in polyconn_move_handle.\n");
    break;
  }
}

void
polyconn_move(PolyConn *poly, Point *to)
{
  Point p;
  int i;
  
  p = *to;
  point_sub(&p, &poly->points[0]);

  poly->points[0] = *to;
  for (i=1;i<poly->numpoints;i++) {
    point_add(&poly->points[i], &p);
  }
}

int
polyconn_closest_segment(PolyConn *poly, Point *point, real line_width)
{
  int i;
  real dist;
  int closest;

  dist = distance_line_point( &poly->points[0], &poly->points[1],
			      line_width, point);
  closest = 0;
  for (i=1;i<poly->numpoints-1;i++) {
    real new_dist = 
      distance_line_point( &poly->points[i], &poly->points[i+1],
			   line_width, point);
    if (new_dist < dist) {
      dist = new_dist;
      closest = i;
    }
}
  return closest;
}

Handle *
polyconn_closest_handle(PolyConn *poly, Point *point)
{
  int i;
  real dist;
  Handle *closest;
  
  closest = poly->object.handles[0];
  dist = distance_point_point( point, &closest->pos);
  for (i=1;i<poly->numpoints;i++) {
    real new_dist;
    new_dist = distance_point_point( point, &poly->points[i]);
    if (new_dist < dist) {
      dist = new_dist;
      closest = poly->object.handles[i];
    }
  }
  return closest;
}

real
polyconn_distance_from(PolyConn *poly, Point *point, real line_width)
{
  int i;
  real dist;
  
  dist = distance_line_point( &poly->points[0], &poly->points[1],
			      line_width, point);
  for (i=1;i<poly->numpoints-1;i++) {
    dist = MIN(dist,
	       distance_line_point( &poly->points[i], &poly->points[i+1],
				    line_width, point));
  }
  return dist;
}

static void
add_handle(PolyConn *poly, int pos, Point *point, Handle *handle)
{
  int i;
  Object *obj;
  
  poly->numpoints++;
  poly->points = realloc(poly->points, poly->numpoints*sizeof(Point));

  for (i=poly->numpoints-1; i > pos; i--) {
    poly->points[i] = poly->points[i-1];
  }
  poly->points[pos] = *point;
  object_add_handle_at((Object*)poly, handle, pos);

  obj = (Object *)poly;
  if (pos==0) {
    obj->handles[1]->type = HANDLE_MINOR_CONTROL;
    obj->handles[1]->id = HANDLE_CORNER;
  }
  if (pos==obj->num_handles-1) {
    obj->handles[obj->num_handles-2]->type = HANDLE_MINOR_CONTROL;
    obj->handles[obj->num_handles-2]->id = HANDLE_CORNER;
  }
}

static void
remove_handle(PolyConn *poly, int pos)
{
  int i;
  Object *obj;
  Handle *old_handle;

  obj = (Object *)poly;

  if (pos==0) {
    obj->handles[1]->type = HANDLE_MAJOR_CONTROL;
    obj->handles[1]->id = HANDLE_MOVE_STARTPOINT;
  }
  if (pos==obj->num_handles-1) {
    obj->handles[obj->num_handles-2]->type = HANDLE_MAJOR_CONTROL;
    obj->handles[obj->num_handles-2]->id = HANDLE_MOVE_ENDPOINT;
  }

  /* delete the points */
  poly->numpoints--;
  for (i=pos; i < poly->numpoints; i++) {
    poly->points[i] = poly->points[i+1];
  }
  poly->points = realloc(poly->points, poly->numpoints*sizeof(Point));

  old_handle = obj->handles[pos];
  object_remove_handle(&poly->object, old_handle);
}


/* Add a point by splitting segment into two, putting the new point at
 'point' or, if NULL, in the middle */
ObjectChange *
polyconn_add_point(PolyConn *poly, int segment, Point *point)
{
  Point realpoint;
  Handle *new_handle;

  if (point == NULL) {
    realpoint.x = (poly->points[segment].x+poly->points[segment+1].x)/2;
    realpoint.y = (poly->points[segment].y+poly->points[segment+1].y)/2;
  } else {
    realpoint = *point;
  }

  new_handle = g_malloc(sizeof(Handle));
  setup_corner_handle(new_handle);
  add_handle(poly, segment+1, &realpoint, new_handle);
  return polyconn_create_change(poly, TYPE_ADD_POINT,
				&realpoint, segment+1, new_handle,
				NULL);
}

ObjectChange *
polyconn_remove_point(PolyConn *poly, int pos)
{
  Handle *old_handle;
  ConnectionPoint *connectionpoint;
  Point old_point;
  
  old_handle = poly->object.handles[pos];
  old_point = poly->points[pos];
  connectionpoint = old_handle->connected_to;
  
  object_unconnect((Object *)poly, old_handle);

  remove_handle(poly, pos);

  polyconn_update_data(poly);
  
  return polyconn_create_change(poly, TYPE_REMOVE_POINT,
				&old_point, pos, old_handle,
				connectionpoint);
}

void
polyconn_update_data(PolyConn *poly)
{
  int i;
  
  /* Update handles: */
  for (i=0;i<poly->numpoints;i++) {
    poly->object.handles[i]->pos = poly->points[i];
  }
}

void
polyconn_update_boundingbox(PolyConn *poly)
{
  Rectangle *bb;
  Point *points;
  int i;
  
  assert(poly != NULL);

  bb = &poly->object.bounding_box;
  points = &poly->points[0];

  bb->right = bb->left = points[0].x;
  bb->top = bb->bottom = points[0].y;
  for (i=1;i<poly->numpoints;i++) {
    if (points[i].x < bb->left)
      bb->left = points[i].x;
    if (points[i].x > bb->right)
      bb->right = points[i].x;
    if (points[i].y < bb->top)
      bb->top = points[i].y;
    if (points[i].y > bb->bottom)
      bb->bottom = points[i].y;
  }
}

void
polyconn_simple_draw(PolyConn *poly, Renderer *renderer, real width)
{
  Point *points;
  
  assert(poly != NULL);
  assert(renderer != NULL);

  points = &poly->points[0];
  
  renderer->ops->set_linewidth(renderer, width);
  renderer->ops->set_linestyle(renderer, LINESTYLE_SOLID);
  renderer->ops->set_linejoin(renderer, LINEJOIN_ROUND);
  renderer->ops->set_linecaps(renderer, LINECAPS_BUTT);

  renderer->ops->draw_polyline(renderer, points, poly->numpoints,
			       &color_black);
}


void
polyconn_init(PolyConn *poly)
{
  Object *obj;

  obj = &poly->object;

  object_init(obj, 2, 0);
  
  poly->numpoints = 2;

  poly->points = g_malloc(2*sizeof(Point));

  poly->object.handles[0] = g_malloc(sizeof(Handle));
  poly->object.handles[1] = g_malloc(sizeof(Handle));

  obj->handles[0]->connect_type = HANDLE_CONNECTABLE;
  obj->handles[0]->connected_to = NULL;
  obj->handles[0]->type = HANDLE_MAJOR_CONTROL;
  obj->handles[0]->id = HANDLE_MOVE_STARTPOINT;
  
  obj->handles[1]->connect_type = HANDLE_CONNECTABLE;
  obj->handles[1]->connected_to = NULL;
  obj->handles[1]->type = HANDLE_MAJOR_CONTROL;
  obj->handles[1]->id = HANDLE_MOVE_ENDPOINT;

  polyconn_update_data(poly);
}

void
polyconn_copy(PolyConn *from, PolyConn *to)
{
  int i;
  Object *toobj, *fromobj;

  toobj = &to->object;
  fromobj = &from->object;

  object_copy(fromobj, toobj);

  to->numpoints = from->numpoints;

  to->points = g_malloc((to->numpoints)*sizeof(Point));

  for (i=0;i<to->numpoints;i++) {
    to->points[i] = from->points[i];
  }

  to->object.handles[0] = g_new(Handle,1);
  *to->object.handles[0] = *from->object.handles[0];
  for (i=1;i<to->numpoints-1;i++) {
    to->object.handles[i] = g_malloc(sizeof(Handle));
    setup_corner_handle(to->object.handles[i]);
  }
  to->object.handles[to->numpoints-1] = g_new(Handle,1);
  *to->object.handles[to->numpoints-1] = *from->object.handles[to->numpoints-1];

  polyconn_update_data(to);
}

void
polyconn_destroy(PolyConn *poly)
{
  int i;
  Handle **temp_handles;

  /* Need to store these temporary since object.handles is
     freed by object_destroy() */
  temp_handles = g_new(Handle *, poly->numpoints);
  for (i=0;i<poly->numpoints;i++)
    temp_handles[i] = poly->object.handles[i];

  object_destroy(&poly->object);

  for (i=0;i<poly->numpoints;i++)
    g_free(temp_handles[i]);
  g_free(temp_handles);
  
  g_free(poly->points);
}


void
polyconn_save(PolyConn *poly, ObjectNode obj_node)
{
  int i;
  AttributeNode attr;

  object_save(&poly->object, obj_node);

  attr = new_attribute(obj_node, "poly_points");
  
  for (i=0;i<poly->numpoints;i++) {
    data_add_point(attr, &poly->points[i]);
  }
}

void
polyconn_load(PolyConn *poly, ObjectNode obj_node) /* NOTE: Does object_init() */
{
  int i;
  AttributeNode attr;
  DataNode data;
  
  Object *obj = &poly->object;

  object_load(obj, obj_node);

  attr = object_find_attribute(obj_node, "poly_points");

  if (attr != NULL)
    poly->numpoints = attribute_num_data(attr);
  else
    poly->numpoints = 0;

  object_init(obj, poly->numpoints, 0);

  data = attribute_first_data(attr);
  poly->points = g_malloc(poly->numpoints*sizeof(Point));
  for (i=0;i<poly->numpoints;i++) {
    data_point(data, &poly->points[i]);
    data = data_next(data);
  }

  obj->handles[0] = g_malloc(sizeof(Handle));
  obj->handles[0]->connect_type = HANDLE_CONNECTABLE;
  obj->handles[0]->connected_to = NULL;
  obj->handles[0]->type = HANDLE_MAJOR_CONTROL;
  obj->handles[0]->id = HANDLE_MOVE_STARTPOINT;
  
  obj->handles[poly->numpoints-1] = g_malloc(sizeof(Handle));
  obj->handles[poly->numpoints-1]->connect_type = HANDLE_CONNECTABLE;
  obj->handles[poly->numpoints-1]->connected_to = NULL;
  obj->handles[poly->numpoints-1]->type = HANDLE_MAJOR_CONTROL;
  obj->handles[poly->numpoints-1]->id = HANDLE_MOVE_ENDPOINT;

  for (i=1;i<poly->numpoints-1;i++) {
    obj->handles[i] = g_malloc(sizeof(Handle));
    setup_corner_handle(obj->handles[i]);
  }

  polyconn_update_data(poly);
}

static void
polyconn_change_free(struct PointChange *change)
{
  if ( (change->type==TYPE_ADD_POINT && !change->applied) ||
       (change->type==TYPE_REMOVE_POINT && change->applied) ){
    if (change->handle)
      g_free(change->handle);
    change->handle = NULL;
  }
}

static void
polyconn_change_apply(struct PointChange *change, Object *obj)
{
  change->applied = 1;
  switch (change->type) {
  case TYPE_ADD_POINT:
    add_handle((PolyConn *)obj, change->pos, &change->point,
	       change->handle);
    break;
  case TYPE_REMOVE_POINT:
    object_unconnect(obj, change->handle);
    remove_handle((PolyConn *)obj, change->pos);
    break;
  }
}

static void
polyconn_change_revert(struct PointChange *change, Object *obj)
{
  switch (change->type) {
  case TYPE_ADD_POINT:
    remove_handle((PolyConn *)obj, change->pos);
    break;
  case TYPE_REMOVE_POINT:
    add_handle((PolyConn *)obj, change->pos, &change->point,
	       change->handle);
    if (change->connected_to) {
      object_connect(obj, change->handle, change->connected_to);
    }
      
    break;
  }
  change->applied = 0;
}

static ObjectChange *
polyconn_create_change(PolyConn *poly, enum change_type type,
		       Point *point, int pos, Handle *handle,
		       ConnectionPoint *connected_to)
{
  struct PointChange *change;

  change = g_new(struct PointChange, 1);

  change->obj_change.apply = (ObjectChangeApplyFunc) polyconn_change_apply;
  change->obj_change.revert = (ObjectChangeRevertFunc) polyconn_change_revert;
  change->obj_change.free = (ObjectChangeFreeFunc) polyconn_change_free;

  change->type = type;
  change->applied = 1;
  change->point = *point;
  change->pos = pos;
  change->handle = handle;
  change->connected_to = connected_to;

  return (ObjectChange *)change;
}
