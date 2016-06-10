#ifndef __DSERV_H__
#define __DSERV_H__

#include <stdio.h>

#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <err.h>

#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"

void request_handler(struct evhttp_request *request, void *args);

#ifdef __DEBUG__
#define DEBUG(...) \
	fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...) \
	do {} while(0)
#endif

#endif
