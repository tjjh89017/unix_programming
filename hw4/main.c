#include <stdio.h>
#include <stdlib.h>

#include <dserv.h>
#include <evhttp.h>

int main(int argc, char *argv[]){

	if(argc < 3){
		puts("Usage: dserv <port> <webroot>");
		return 0;
	}

	int port = atoi(argv[1]);
	DEBUG("port: %d\n", port);

	struct event_base *base;
	struct evhttp *httpd;

	base = event_base_new();
	httpd = evhttp_new(base);

	//evhttp_set_allowed_methods(httpd, EVHTTP_REQ_GET);
	evhttp_set_gencb(httpd, request_handler, 0);

	if(evhttp_bind_socket(httpd, "127.0.0.1", port) != 0)
		errx(1, "Could not bind to 0.0.0.0:%d", port);

	event_base_dispatch(base);
	evhttp_free(httpd);
	event_base_free(base);

	return 0;
}
