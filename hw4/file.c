#include <dserv.h>

void file_handler(int fd, short status, void *args)
{
	struct evhttp_request *request = (struct evhttp_request*)args;
	struct evbuffer *buffer = evbuffer_new();
	char buf[1024];
	int len;

	DEBUG("file handler\nstatus = %d\n", status);
	if(status & EV_READ){
		evhttp_send_reply_start(request, HTTP_OK, NULL);

		while((len = read(fd, buf, 1024)) > 0){
			DEBUG("read success\n");
			evbuffer_add(buffer, buf, len);
			evhttp_send_reply_chunk(request, buffer);
		}
	}
	evhttp_send_reply_end(request);
	evbuffer_free(buffer);
}
