#include <dserv.h>

void request_handler(struct evhttp_request *request, void *args)
{
	struct evbuffer *buffer;
	struct evkeyvalq headers;
	DEBUG("request_handler\n");
	DEBUG("uri: %s\n", request->uri);

	/*
	 * TODO parse uri
	 * from request->uri
	 *
	 * TODO file handler
	 * using libevent to handle file and nonblocking
	 * autogen indexes
	 */


	buffer = evbuffer_new();

	evhttp_send_reply(request, HTTP_OK, "OK", buffer);

	DEBUG("request_handler\n\n");

	evbuffer_free(buffer);
}
