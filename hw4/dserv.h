#ifndef __DSERV_H__
#define __DSERV_H__

#include <stdio.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <err.h>

#include <magic.h>

#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"

/* main.c */

/* request.c */
void request_handler(struct evhttp_request *request, void *args);

/* file.c */
void file_handler(int fd, short type, void *args);

#ifdef __DEBUG__
#define DEBUG(...) \
	fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...) \
	do {} while(0)
#endif

#endif
