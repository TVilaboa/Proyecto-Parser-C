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
#ifndef CONNECTION_H
#define CONNECTION_H

#include "object.h"

typedef struct _Connection Connection;

/* This is a subclass of Object used to help implementing objects
 * of a line-like type. */
struct _Connection {
  /* Object must be first because this is a 'subclass' of it. */
  Object object;
  
  Point endpoints[2];
  Handle endpoint_handles[2];
};

extern void connection_update_handles(Connection *conn);
extern void connection_update_boundingbox(Connection *conn);
extern void connection_init(Connection *conn,
			    int num_handles, int num_connections);
extern void connection_destroy(Connection *conn);
extern void connection_copy(Connection *from, Connection *to);
extern void connection_save(Connection *conn, ObjectNode obj_node);
extern void connection_load(Connection *conn, ObjectNode obj_node);
extern void connection_move_handle(Connection *conn, HandleId id,
				   Point *to, HandleMoveReason reason);

#endif /* CONNECTION_H */
