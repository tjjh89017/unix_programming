#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include <dserv.h>
#include <evhttp.h>

int main(int argc, char *argv[]){

	char path[PATH_MAX + 1];

	if(argc < 3){
		puts("Usage: dserv <port> <webroot>");
		return 0;
	}

	int port = atoi(argv[1]);
	DEBUG("port: %d\n", port);

	// chdir
	if(chdir(argv[2]))
		errx(1, "Cloud not chdir to %s\n", argv[2]);
	// chroot
	//if(realpath(argv[2], path) && chroot(path))
	//	errx(1, "Cloud not chroot to %s\nerrno = %d\nstrerror: %s\n", path, errno, strerror(errno));

	struct event_base *base;
	struct evhttp *httpd;

	base = event_base_new();
	httpd = evhttp_new(base);
	evhttp_set_gencb(httpd, request_handler, base);

	if(evhttp_bind_socket(httpd, "127.0.0.1", port) != 0)
		errx(1, "Could not bind to 0.0.0.0:%d", port);

	event_base_dispatch(base);
	evhttp_free(httpd);
	event_base_free(base);

	return 0;
}
