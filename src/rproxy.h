/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License. For a copy,
 * see http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _RPROXY_H
#define _RPROXY_H

#include "config.h"

#ifdef ENABLE_RPROXY

#include <stdbool.h>
#include <regex.h>
#ifdef ENABLE_SSL
#include "polarssl/ssl.h"
#endif
#include "ip.h"
#include "liblist.h"

typedef struct type_rproxy {
	regex_t   pattern;

#ifdef ENABLE_SSL
	bool      use_ssl;
#endif
	char      *hostname;
	size_t    hostname_len;
	t_ip_addr ip_addr;
	int       port;
	char      *path;
	size_t    path_len;
	int       timeout;

	struct type_rproxy *next;
} t_rproxy;

typedef struct {
	int           client_socket;
	t_ip_addr     *client_ip;
	char          *method;
	char          *uri;
	char          *vars;
	t_http_header *http_headers;
	char          *body;
	int           body_length;
	char          *remote_user;
#ifdef ENABLE_CACHE
	t_charlist    *cache_extensions;
#endif
} t_rproxy_options;

typedef struct {
	int socket;
#ifdef ENABLE_SSL
	bool use_ssl;
	ssl_context ssl;
#endif
} t_rproxy_webserver;

typedef struct {
	int bytes_sent;
} t_rproxy_result;

int init_rproxy_module(void);
t_rproxy *rproxy_setting(char *line);
bool rproxy_match(t_rproxy *rproxy, char *uri);
bool rproxy_loop_detected(t_http_header *http_headers);
void init_rproxy_options(t_rproxy_options *options, int socket, t_ip_addr *client_ip,
                         char *method, char *uri, t_http_header *http_headers,
                         char *body, int body_length, char *remote_user);
void init_rproxy_result(t_rproxy_result *result);
int  connect_to_server(t_ip_addr *ip_addr, int port);
int send_request_to_webserver(t_rproxy_webserver *webserver, t_rproxy_options *options,
                              t_rproxy *rproxy, t_rproxy_result *result);
int tunnel_ssh_connection(int client_sock);

#endif

#endif
