/*
 * ProFTPD - FTP server daemon
 * Copyright (c) 1997, 1998 Public Flood Software
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 */

/* BSD socket manipulation tools.
 *
 * $Id: inet.h,v 1.2 1999-09-17 03:56:55 macgyver Exp $
 */

#ifndef __INET_H
#define __INET_H

/* connection modes */
#define CM_NONE		0
#define CM_LISTEN	1
#define CM_OPEN		2
#define CM_ACCEPT	3
#define CM_CONNECT	4
#define CM_CLOSED	5
#define CM_ERROR	6

#ifndef INADDR_ANY
#define INADDR_ANY	((unsigned long int) 0x00000000)
#endif

#ifndef INPORT_ANY
#define INPORT_ANY	0
#endif

#ifndef U32BITS
#define U32BITS		0xffffffff
#endif

/* connection structure */
typedef struct conn_struc {
  pool *pool;
  int mode;				/* Current connection mode */
  int listen_fd;			/* Listening file descriptor */
  int rcvbuf,sndbuf;			/* Socket recv and send sizes */

  int xerrno;				/* Set to error if mode == CM_ERROR */

  array_header *iplist;
  int niplist;				/* IPs we are listening to */
  
  int rfd,wfd;				/* Read and write fds */
  IOFILE *inf,*outf;			/* Input/Output streams */

  p_in_addr_t *remote_ipaddr;		/* Remote address of connection */
  int remote_port;			/* Remote port of connection */
  p_in_addr_t *local_ipaddr;		/* Local side of connection */
  int local_port;			/* Local port */
  char *remote_name;			/* Remote FQDN */
} conn_t;


/* Prototypes */
void init_inet();
void clear_inet_pool();
int inet_reverse_dns(pool*,int);
int inet_getservport(pool*,char *serv,char *proto);
char *inet_validate(char *buf);
char *inet_gethostname(pool*);
char *inet_fqdn(pool*,const char*);
p_in_addr_t *inet_getaddr(pool*,char*);
char *inet_ascii(pool*,p_in_addr_t*);
char *inet_getname(pool*,p_in_addr_t*);
conn_t *inet_copy_connection(pool*,conn_t*);
int inet_prebind_socket(pool*,p_in_addr_t*,int);
conn_t *inet_create_dup_connection(pool*,xaset_t*,int,p_in_addr_t*);
conn_t *inet_create_connection(pool*,xaset_t *servers,int fd,
                               p_in_addr_t *bind_addr,int port,int retry_bind);
void inet_close(pool*,conn_t*);
int inet_setnonblock(pool*,conn_t*);
int inet_setblock(pool*,conn_t*);
int inet_setoptions(pool*,conn_t*,int rcvbuf,int sndbuf);
int inet_set_proto_options(pool*,conn_t*,int,int,int,int);
int inet_setasync(pool*,conn_t*);
int inet_listen(pool*,conn_t*,int);
int inet_resetlisten(pool*,conn_t*);
int inet_accept_nowait(pool*,conn_t*);
int inet_connect(pool*,conn_t*,p_in_addr_t*,int);
int inet_connect_nowait(pool*,conn_t*,p_in_addr_t*,int);
int inet_get_conn_info(conn_t*,int);
conn_t *inet_accept(pool*,conn_t*,int rfd,int wfd,int resolve);
conn_t *inet_associate(pool*,conn_t*,p_in_addr_t *addr,
                       IOFILE *inf, IOFILE *outf,int resolve);
conn_t *inet_openrw(pool*,conn_t*,p_in_addr_t *addr,
                    int fd,int rfd,int wfd,int resolve);

#endif /* __INET_H */
