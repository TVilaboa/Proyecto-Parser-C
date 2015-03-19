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

#include "undo.h"
#include "object_ops.h"
#include "properties.h"
#include "connectionpoint_ops.h"
#include "focus.h"
#include "group.h"
#include "preferences.h"

#if 0
#define DEBUG_PRINTF(args) printf args
#else
#define DEBUG_PRINTF(args)
#endif

static void
transaction_point_pointer(Change *change, Diagram *dia)
{
  /* Empty function used to track transactionpoints. */
}

static int
is_transactionpoint(Change *change)
{
  return change->apply == transaction_point_pointer;
}

static Change *
new_transactionpoint(void)
{
  Change *transaction = g_new(Change, 1);

  if (transaction) {
    transaction->apply = transaction_point_pointer;
    transaction->revert = transaction_point_pointer;
    transaction->free = NULL;
  }
  
  return transaction;
}

UndoStack *
new_undo_stack(Diagram *dia)
{
  UndoStack *stack;
  Change *transaction;
  
  stack = g_new(UndoStack, 1);
  if (stack!=NULL){
    stack->dia = dia;
    transaction = new_transactionpoint();
    transaction->next = transaction->prev = NULL;
    stack->last_change = transaction;
    stack->current_change = transaction;
    stack->depth = 0;
  }
  return stack;
}

void
undo_destroy(UndoStack *stack)
{
  undo_clear(stack);
  g_free(stack->current_change); /* Free first transaction point. */
  g_free(stack);
}



static void
undo_remove_redo_info(UndoStack *stack)
{
  Change *change;
  Change *next_change;

  DEBUG_PRINTF(("UNDO: Removing redo info\n"));
  
  change = stack->current_change->next;
  stack->current_change->next = NULL;
  stack->last_change = stack->current_change;
    
  while (change != NULL) {
    next_change = change->next;
    if (change->free)
      (change->free)(change);
    g_free(change);
    change = next_change;
  }
}

static int
depth(UndoStack *stack)
{
  int i;
  Change *change;
  change = stack->current_change;

  i = 0;
  while (change->prev != NULL) {
    change = change->prev;
    i++;
  }
  return i;
}

void
undo_push_change(UndoStack *stack, Change *change)
{
  if (stack->current_change != stack->last_change)
    undo_remove_redo_info(stack);

  DEBUG_PRINTF(("UNDO: Push new change at %d\n", depth(stack)));
  
  change->prev = stack->last_change;
  change->next = NULL;
  stack->last_change->next = change;
  stack->last_change = change;
  stack->current_change = change;
}

void
undo_delete_lowest_transaction(UndoStack *stack)
{
  Change *change;
  Change *next_change;

  /* Find the lowest change: */
  change = stack->current_change;
  while (change->prev != NULL) {
    change = change->prev;
  }

  /* Remove changes from the bottom until (and including)
   * the first transactionpoint.
   * Stop if we reach current_change or NULL.
   */
  do {
    if ( (change == NULL) || (change == stack->current_change))
      break;

    next_change = change->next;
    DEBUG_PRINTF(("freeing one change from the bottom.\n"));
    if (change->free)
      (change->free)(change);
    g_free(change);
    change = next_change;
  } while (!is_transactionpoint(change));
  
  if (is_transactionpoint(change)) {
    stack->depth--;
    DEBUG_PRINTF(("Decreasing stack depth to: %d\n", stack->depth));
  }
  change->prev = NULL;
}

void
undo_set_transactionpoint(UndoStack *stack)
{
  Change *transaction;

  if (is_transactionpoint(stack->current_change))
    return;

  DEBUG_PRINTF(("UNDO: Push new transactionpoint at %d\n", depth(stack)));

  transaction = new_transactionpoint();

  undo_push_change(stack, transaction);
  stack->depth++;
  DEBUG_PRINTF(("Increasing stack depth to: %d\n", stack->depth));

  if (prefs.undo_depth > 0) {
    while (stack->depth > prefs.undo_depth){
      undo_delete_lowest_transaction(stack);
    }
  }
}

void
undo_revert_to_last_tp(UndoStack *stack)
{
  Change *change;
  Change *prev_change;
  
  if (stack->current_change->prev == NULL)
    return; /* Can't revert first transactionpoint */

  change = stack->current_change;
  do {
    prev_change = change->prev;
    (change->revert)(change, stack->dia);
    change = prev_change;
  } while (!is_transactionpoint(change));
  stack->current_change  = change;
  stack->depth--;
  DEBUG_PRINTF(("Decreasing stack depth to: %d\n", stack->depth));
}

void
undo_apply_to_next_tp(UndoStack *stack)
{
  Change *change;
  Change *next_change;
  
  change = stack->current_change;

  if (change->next == NULL)
    return /* Already at top. */;
  
  do {
    next_change = change->next;
    (change->apply)(change, stack->dia);
    change = next_change;
  } while ( (change != NULL) &&
	    (!is_transactionpoint(change)) );
  if (change == NULL)
    change = stack->last_change;
  stack->current_change = change;
  stack->depth++;
  DEBUG_PRINTF(("Increasing stack depth to: %d\n", stack->depth));
}


void
undo_clear(UndoStack *stack)
{
  Change *change;

  DEBUG_PRINTF(("undo_clear()\n"));
  
  change = stack->current_change;
    
  while (change->prev != NULL) {
    change = change->prev;
  }
  
  stack->current_change = change;
  stack->depth = 0;
  undo_remove_redo_info(stack);
}


/****************************************************************/
/****************************************************************/
/*****************                          *********************/
/***************** Specific undo functions: *********************/
/*****************                          *********************/
/****************************************************************/
/****************************************************************/

/******** Move object list: */

struct MoveObjectsChange {
  Change change;

  Point *orig_pos;
  Point *dest_pos;
  GList *obj_list;
};

static void
move_objects_apply(struct MoveObjectsChange *change, Diagram *dia)
{
  GList *list;
  int i;
  Object *obj;

  object_add_updates_list(change->obj_list, dia);

  list = change->obj_list;
  i=0;
  while (list != NULL) {
    obj = (Object *)  list->data;
    
    obj->ops->move(obj, &change->dest_pos[i]);
    
    list = g_list_next(list); i++;
  }

  list = change->obj_list;
  while (list!=NULL) {
    obj = (Object *) list->data;
    
    diagram_update_connections_object(dia, obj, TRUE);
    
    list = g_list_next(list);
  }

  object_add_updates_list(change->obj_list, dia);
}

static void
move_objects_revert(struct MoveObjectsChange *change, Diagram *dia)
{
  GList *list;
  int i;
  Object *obj;

  object_add_updates_list(change->obj_list, dia);

  list = change->obj_list;
  i=0;
  while (list != NULL) {
    obj = (Object *)  list->data;
    
    obj->ops->move(obj, &change->orig_pos[i]);
    
    list = g_list_next(list); i++;
  }

  list = change->obj_list;
  while (list!=NULL) {
    obj = (Object *) list->data;
    
    diagram_update_connections_object(dia, obj, TRUE);
    
    list = g_list_next(list);
  }

  object_add_updates_list(change->obj_list, dia);
}

static void
move_objects_free(struct MoveObjectsChange *change)
{
  g_free(change->orig_pos);
  g_free(change->dest_pos);
  g_list_free(change->obj_list);
}

extern Change *
undo_move_objects(Diagram *dia, Point *orig_pos, Point *dest_pos,
		  GList *obj_list)
{
  struct MoveObjectsChange *change;

  change = g_new(struct MoveObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) move_objects_apply;
  change->change.revert = (UndoRevertFunc) move_objects_revert;
  change->change.free = (UndoFreeFunc) move_objects_free;

  change->orig_pos = orig_pos;
  change->dest_pos = dest_pos;
  change->obj_list = obj_list;

  DEBUG_PRINTF(("UNDO: Push new move objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/********** Move handle: */

struct MoveHandleChange {
  Change change;

  Point orig_pos;
  Point dest_pos;
  Handle *handle;
  Object *obj;
};

static void
move_handle_apply(struct MoveHandleChange *change, Diagram *dia)
{
  object_add_updates(change->obj, dia);
  change->obj->ops->move_handle(change->obj, change->handle,
				 &change->dest_pos,
				 HANDLE_MOVE_USER_FINAL,0);
  object_add_updates(change->obj, dia);
  diagram_update_connections_object(dia, change->obj, TRUE);
}

static void
move_handle_revert(struct MoveHandleChange *change, Diagram *dia)
{
  object_add_updates(change->obj, dia);
  change->obj->ops->move_handle(change->obj, change->handle,
				 &change->orig_pos,
				 HANDLE_MOVE_USER_FINAL,0);
  object_add_updates(change->obj, dia);
  diagram_update_connections_object(dia, change->obj, TRUE);
}

static void
move_handle_free(struct MoveHandleChange *change)
{
}


Change *
undo_move_handle(Diagram *dia,
		 Handle *handle, Object *obj,
		 Point orig_pos, Point dest_pos)
{
  struct MoveHandleChange *change;

  change = g_new(struct MoveHandleChange, 1);
  
  change->change.apply = (UndoApplyFunc) move_handle_apply;
  change->change.revert = (UndoRevertFunc) move_handle_revert;
  change->change.free = (UndoFreeFunc) move_handle_free;

  change->orig_pos = orig_pos;
  change->dest_pos = dest_pos;
  change->handle = handle;
  change->obj = obj;

  DEBUG_PRINTF(("UNDO: Push new move handle at %d\n", depth(dia->undo)));

  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/***************** Connect object: */

struct ConnectChange {
  Change change;

  Object *obj;
  Handle *handle;
  ConnectionPoint *connectionpoint;
  Point handle_pos;
};

static void
connect_apply(struct ConnectChange *change, Diagram *dia)
{
  object_connect(change->obj, change->handle, change->connectionpoint);
  
  object_add_updates(change->obj, dia);
  change->obj->ops->move_handle(change->obj, change->handle ,
				&change->connectionpoint->pos,
				HANDLE_MOVE_CONNECTED, 0);
  
  object_add_updates(change->obj, dia);
}

static void
connect_revert(struct ConnectChange *change, Diagram *dia)
{
  object_unconnect(change->obj, change->handle);
  
  object_add_updates(change->obj, dia);
  change->obj->ops->move_handle(change->obj, change->handle ,
				&change->handle_pos,
				HANDLE_MOVE_CONNECTED, 0);
  
  object_add_updates(change->obj, dia);
}

static void
connect_free(struct ConnectChange *change)
{
}

extern Change *
undo_connect(Diagram *dia, Object *obj, Handle *handle,
	     ConnectionPoint *connectionpoint)
{
  struct ConnectChange *change;

  change = g_new(struct ConnectChange, 1);
  
  change->change.apply = (UndoApplyFunc) connect_apply;
  change->change.revert = (UndoRevertFunc) connect_revert;
  change->change.free = (UndoFreeFunc) connect_free;

  change->obj = obj;
  change->handle = handle;
  change->handle_pos = handle->pos;
  change->connectionpoint = connectionpoint;

  DEBUG_PRINTF(("UNDO: Push new connect at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/*************** Unconnect object: */

struct UnconnectChange {
  Change change;

  Object *obj;
  Handle *handle;
  ConnectionPoint *connectionpoint;
};

static void
unconnect_apply(struct UnconnectChange *change, Diagram *dia)
{
  object_unconnect(change->obj, change->handle);
  
  object_add_updates(change->obj, dia);
}

static void
unconnect_revert(struct UnconnectChange *change, Diagram *dia)
{
  object_connect(change->obj, change->handle, change->connectionpoint);
  
  object_add_updates(change->obj, dia);
}

static void
unconnect_free(struct UnconnectChange *change)
{
}

extern Change *
undo_unconnect(Diagram *dia, Object *obj, Handle *handle)
{
  struct UnconnectChange *change;

  change = g_new(struct UnconnectChange, 1);
  
  change->change.apply = (UndoApplyFunc) unconnect_apply;
  change->change.revert = (UndoRevertFunc) unconnect_revert;
  change->change.free = (UndoFreeFunc) unconnect_free;

  change->obj = obj;
  change->handle = handle;
  change->connectionpoint = handle->connected_to;

  DEBUG_PRINTF(("UNDO: Push new unconnect at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}


/******** Delete object list: */

struct DeleteObjectsChange {
  Change change;

  Layer *layer;
  GList *obj_list; /* Owning reference when applied */
  GList *original_objects;
  int applied;
};

static void
delete_objects_apply(struct DeleteObjectsChange *change, Diagram *dia)
{
  GList *list;
  
  DEBUG_PRINTF(("delete_objects_apply()\n"));
  change->applied = 1;
  diagram_unselect_objects(dia, change->obj_list);
  layer_remove_objects(change->layer, change->obj_list);
  object_add_updates_list(change->obj_list, dia);
  
  list = change->obj_list;
  while (list != NULL) {
    Object *obj = (Object *)list->data;

  /* Have to hide any open properties dialog
     if it contains some object in cut_list */
    properties_hide_if_shown(dia, obj);

    /* Remove focus if active */
    if ((active_focus()!=NULL) && (active_focus()->obj == obj)) {
      remove_focus();
    }
    
    list = g_list_next(list);
  }
  
}

static void
delete_objects_revert(struct DeleteObjectsChange *change, Diagram *dia)
{
  DEBUG_PRINTF(("delete_objects_revert()\n"));
  change->applied = 0;
  layer_set_object_list(change->layer,
			g_list_copy(change->original_objects));
  object_add_updates_list(change->obj_list, dia);
}

static void
delete_objects_free(struct DeleteObjectsChange *change)
{
  DEBUG_PRINTF(("delete_objects_free()\n"));
  if (change->applied)
    destroy_object_list(change->obj_list);
  else
    g_list_free(change->obj_list);
  g_list_free(change->original_objects);
}

Change *
undo_delete_objects(Diagram *dia, GList *obj_list)
{
  struct DeleteObjectsChange *change;

  change = g_new(struct DeleteObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) delete_objects_apply;
  change->change.revert = (UndoRevertFunc) delete_objects_revert;
  change->change.free = (UndoFreeFunc) delete_objects_free;

  change->layer = dia->data->active_layer;
  change->obj_list = obj_list;
  change->original_objects = g_list_copy(dia->data->active_layer->objects);
  change->applied = 0;

  DEBUG_PRINTF(("UNDO: Push new delete objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/******** Insert object list: */

struct InsertObjectsChange {
  Change change;

  Layer *layer;
  GList *obj_list; /* Owning reference when not applied */
  int applied;
};

static void
insert_objects_apply(struct InsertObjectsChange *change, Diagram *dia)
{
  DEBUG_PRINTF(("insert_objects_apply()\n"));
  change->applied = 1;
  layer_add_objects(change->layer, g_list_copy(change->obj_list));
  object_add_updates_list(change->obj_list, dia);
}

static void
insert_objects_revert(struct InsertObjectsChange *change, Diagram *dia)
{
  GList *list;
  
  DEBUG_PRINTF(("insert_objects_revert()\n"));
  change->applied = 0;
  diagram_unselect_objects(dia, change->obj_list);
  layer_remove_objects(change->layer, change->obj_list);
  object_add_updates_list(change->obj_list, dia);
  
  list = change->obj_list;
  while (list != NULL) {
    Object *obj = (Object *)list->data;

  /* Have to hide any open properties dialog
     if it contains some object in cut_list */
    properties_hide_if_shown(dia, obj);

    /* Remove focus if active */
    if ((active_focus()!=NULL) && (active_focus()->obj == obj)) {
      remove_focus();
    }
    
    list = g_list_next(list);
  }
}

static void
insert_objects_free(struct InsertObjectsChange *change)
{
  DEBUG_PRINTF(("insert_objects_free()\n"));
  if (!change->applied)
    destroy_object_list(change->obj_list);
  else
    g_list_free(change->obj_list);
}

Change *
undo_insert_objects(Diagram *dia, GList *obj_list, int applied)
{
  struct InsertObjectsChange *change;

  change = g_new(struct InsertObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) insert_objects_apply;
  change->change.revert = (UndoRevertFunc) insert_objects_revert;
  change->change.free = (UndoFreeFunc) insert_objects_free;

  change->layer = dia->data->active_layer;
  change->obj_list = obj_list;
  change->applied = applied;

  DEBUG_PRINTF(("UNDO: Push new insert objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/******** Reorder object list: */

struct ReorderObjectsChange {
  Change change;

  Layer *layer;
  GList *changed_list; /* Owning reference when applied */
  GList *original_objects;
  GList *reordered_objects;
};

static void
reorder_objects_apply(struct ReorderObjectsChange *change, Diagram *dia)
{
  DEBUG_PRINTF(("reorder_objects_apply()\n"));
  layer_set_object_list(change->layer,
			g_list_copy(change->reordered_objects));
  object_add_updates_list(change->changed_list, dia);
}

static void
reorder_objects_revert(struct ReorderObjectsChange *change, Diagram *dia)
{
  DEBUG_PRINTF(("reorder_objects_revert()\n"));
  layer_set_object_list(change->layer,
			g_list_copy(change->original_objects));
  object_add_updates_list(change->changed_list, dia);
}

static void		
reorder_objects_free(struct ReorderObjectsChange *change)
{			    
  DEBUG_PRINTF(("reorder_objects_free()\n"));
  g_list_free(change->changed_list);
  g_list_free(change->original_objects);
  g_list_free(change->reordered_objects);
}

Change *
undo_reorder_objects(Diagram *dia, GList *changed_list, GList *orig_list)
{
  struct ReorderObjectsChange *change;

  change = g_new(struct ReorderObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) reorder_objects_apply;
  change->change.revert = (UndoRevertFunc) reorder_objects_revert;
  change->change.free = (UndoFreeFunc) reorder_objects_free;

  change->layer = dia->data->active_layer;
  change->changed_list = changed_list;
  change->original_objects = orig_list;
  change->reordered_objects = g_list_copy(dia->data->active_layer->objects);

  DEBUG_PRINTF(("UNDO: Push new reorder objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/******** ObjectChange: */

struct ObjectChangeChange {
  Change change;

  Object *obj;
  ObjectChange *obj_change;
};


static void
object_change_apply(struct ObjectChangeChange *change,
		    Diagram *dia)
{
  object_add_updates(change->obj, dia);
  change->obj_change->apply(change->obj_change, change->obj);
  { /* Make sure object updates its data: */
    Point p = change->obj->position;
    (change->obj->ops->move)(change->obj,&p);
  }
  object_add_updates(change->obj, dia);
  
  diagram_update_connections_object(dia, change->obj, TRUE);

  properties_update_if_shown(dia, change->obj);
}

static void
object_change_revert(struct ObjectChangeChange *change,
		     Diagram *dia)
{
  object_add_updates(change->obj, dia);
  change->obj_change->revert(change->obj_change, change->obj);
  { /* Make sure object updates its data: */
    Point p = change->obj->position;
    (change->obj->ops->move)(change->obj,&p);
  }
  object_add_updates(change->obj, dia);
  
  diagram_update_connections_object(dia, change->obj, TRUE);

  properties_update_if_shown(dia, change->obj);
}

static void
object_change_free(struct ObjectChangeChange *change)
{
  DEBUG_PRINTF(("state_change_free()\n"));
  if (change->obj_change->free)
    (*change->obj_change->free)(change->obj_change);
  g_free(change->obj_change);
}

Change *
undo_object_change(Diagram *dia, Object *obj,
		   ObjectChange *obj_change)
{
  struct ObjectChangeChange *change;

  change = g_new(struct ObjectChangeChange, 1);
  
  change->change.apply = (UndoApplyFunc) object_change_apply;
  change->change.revert = (UndoRevertFunc) object_change_revert;
  change->change.free = (UndoFreeFunc) object_change_free;

  change->obj = obj;
  change->obj_change = obj_change;

  DEBUG_PRINTF(("UNDO: Push new obj_change at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/******** Group object list: */

struct GroupObjectsChange {
  Change change;

  Layer *layer;
  Object *group;   /* owning reference if not applied */
  GList *obj_list; /* This list is owned by the group. */
  GList *orig_list; /* This list (not the object) is owned */
  int applied;
};

static void
group_objects_apply(struct GroupObjectsChange *change, Diagram *dia)
{
  GList *list;

  DEBUG_PRINTF(("group_objects_apply()\n"));
  
  change->applied = 1;
  
  diagram_unselect_objects(dia, change->obj_list);
  layer_remove_objects(change->layer, change->obj_list);
  layer_add_object(change->layer, change->group);
  object_add_updates(change->group, dia);

  list = change->obj_list;
  while (list != NULL) {
    Object *obj = (Object *)list->data;
    
  /* Have to hide any open properties dialog
     if it contains some object in cut_list */
    properties_hide_if_shown(dia, obj);

    /* Remove focus if active */
    if ((active_focus()!=NULL) && (active_focus()->obj == obj)) {
      remove_focus();
    }
    
    list = g_list_next(list);
  }
}

static void
group_objects_revert(struct GroupObjectsChange *change, Diagram *dia)
{
  GList *old_list;
  
  DEBUG_PRINTF(("group_objects_revert()\n"));
  change->applied = 0;
  
  diagram_unselect_object(dia, change->group);
  object_add_updates(change->group, dia);

  old_list = change->layer->objects;
  change->layer->objects = g_list_copy(change->orig_list);
  g_list_free(old_list);
  
  object_add_updates_list(change->obj_list, dia);

  properties_hide_if_shown(dia, change->group);
}

static void
group_objects_free(struct GroupObjectsChange *change)
{
  DEBUG_PRINTF(("group_objects_free()\n"));
  if (!change->applied) {
    group_destroy_shallow(change->group);
    change->group = NULL;
    change->obj_list = NULL;
  }
  g_list_free(change->orig_list);
}

Change *
undo_group_objects(Diagram *dia, GList *obj_list, Object *group,
		   GList *orig_list)
{
  struct GroupObjectsChange *change;

  change = g_new(struct GroupObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) group_objects_apply;
  change->change.revert = (UndoRevertFunc) group_objects_revert;
  change->change.free = (UndoFreeFunc) group_objects_free;

  change->layer = dia->data->active_layer;
  change->group = group;
  change->obj_list = obj_list;
  change->orig_list = orig_list;
  change->applied = 1;

  DEBUG_PRINTF(("UNDO: Push new group objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

/******** Ungroup object list: */

struct UngroupObjectsChange {
  Change change;

  Layer *layer;
  Object *group;   /* owning reference if applied */
  GList *obj_list; /* This list is owned by the ungroup. */
  int group_index;
  int applied;
};

static void
ungroup_objects_apply(struct UngroupObjectsChange *change, Diagram *dia)
{
  DEBUG_PRINTF(("ungroup_objects_apply()\n"));
  
  change->applied = 1;
  
  diagram_unselect_object(dia, change->group);
  object_add_updates(change->group, dia);
  layer_replace_object_with_list(change->layer, change->group,
				 g_list_copy(change->obj_list));
  object_add_updates_list(change->obj_list, dia);
  
  properties_hide_if_shown(dia, change->group);
}

static void
ungroup_objects_revert(struct UngroupObjectsChange *change, Diagram *dia)
{
  GList *list;
  
  DEBUG_PRINTF(("ungroup_objects_revert()\n"));
  change->applied = 0;
  

  diagram_unselect_objects(dia, change->obj_list);
  layer_remove_objects(change->layer, change->obj_list);
  layer_add_object_at(change->layer, change->group, change->group_index);
  object_add_updates(change->group, dia);

  list = change->obj_list;
  while (list != NULL) {
    Object *obj = (Object *)list->data;
    
  /* Have to hide any open properties dialog
     if it contains some object in cut_list */
    properties_hide_if_shown(dia, obj);

    /* Remove focus if active */
    if ((active_focus()!=NULL) && (active_focus()->obj == obj)) {
      remove_focus();
    }
    
    list = g_list_next(list);
  }
}

static void
ungroup_objects_free(struct UngroupObjectsChange *change)
{
  DEBUG_PRINTF(("ungroup_objects_free()\n"));
  if (change->applied) {
    group_destroy_shallow(change->group);
    change->group = NULL;
    change->obj_list = NULL;
  }
}

Change *
undo_ungroup_objects(Diagram *dia, GList *obj_list, Object *group,
		     int group_index)
{
  struct UngroupObjectsChange *change;

  change = g_new(struct UngroupObjectsChange, 1);
  
  change->change.apply = (UndoApplyFunc) ungroup_objects_apply;
  change->change.revert = (UndoRevertFunc) ungroup_objects_revert;
  change->change.free = (UndoFreeFunc) ungroup_objects_free;

  change->layer = dia->data->active_layer;
  change->group = group;
  change->obj_list = obj_list;
  change->group_index = group_index;
  change->applied = 1;

  DEBUG_PRINTF(("UNDO: Push new ungroup objects at %d\n", depth(dia->undo)));
  undo_push_change(dia->undo, (Change *) change);
  return (Change *)change;
}

