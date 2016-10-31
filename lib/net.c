// Copyright [2016] [hoenir]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <string.h>
#include <sys/types.h>

#include "info.h"
#include "mem.h"
#include "util.h"

#include "net.h"

conn_t *conn_new(void)
{
	conn_t *conn = xzmalloc(sizeof(conn_t));
	conn->addr   = xzmalloc(sizeof(conn->addr));

	return conn;
}

void conn_free(conn_t *conn)
{
	if (!conn)
		INFOEE("Can't free a connection pointer that is NULL");
	if ((!conn->addr))
		INFOEE("Can't free a connection members that are NULL");

	xfree(conn->addr);
	xfree(conn);
}

void conn_buff_new(conn_t *conn, uint16_t sz)
{
	if (!(conn))
		INFOEE("Could alloc underlying buff of conn NULL");
	
	conn->bufflen = sz;
	conn->buff = xmalloc(sizeof(conn->buff) * conn->bufflen);
}

void conn_buff_free(conn_t *conn)
{
	if (!(conn))
		INFOEE("Could not free underlying buff of conn NULL");
	xfree(conn->buff);
}
