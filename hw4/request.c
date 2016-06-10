#include <dserv.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <magic.h>

void request_handler(struct evhttp_request *request, void *args)
{
	struct event_base *base = (struct event_base*)args;
	struct evbuffer *buffer;
	struct evkeyvalq *headers = evhttp_request_get_output_headers(request);
	char *query_str = NULL;
	int fd;
	struct stat file_stat;
	char *filename = request->uri + 1;
	int len = 0;
	char *tmp;
	char *buf;
	const char *mime;
	magic_t magic;
	DEBUG("request_handler\n");
	DEBUG("uri: %s\n", request->uri);

	/*
	 * TODO parse uri
	 * from request->uri
	 */
	query_str = strchr(request->uri, '?');
	if(query_str){
		query_str[0] = '\0';
		query_str++;
		DEBUG("query_str: %s\n", query_str);
	}

	len = strlen(filename) + 20;
	tmp = filename;
	buf = malloc(len * sizeof(char));
	filename = buf;
	strncpy(filename, tmp, len);

	// specs root
	if(filename[0] == '\0'){
		strncpy(filename, "./", len);
	}

	// check directory
	stat(filename, &file_stat);
	if(S_ISDIR(file_stat.st_mode)){
		DEBUG("filename = %s is dir\n", filename);
		// check for slash
		if(filename[strlen(filename) - 1] != '/'){
			filename[strlen(filename)] = '/';
			filename[strlen(filename) + 1] = '\0';

			evhttp_add_header(headers, "Location", filename);
			evhttp_send_reply(request, HTTP_MOVEPERM, NULL, NULL);
			goto end;
		}

		// search index.html
		strcat(filename, "index.html");
		DEBUG("filename = %s\n", filename);
		if(stat(filename, &file_stat)){
			evhttp_send_error(request, HTTP_NOTFOUND, NULL);

			/*
			 * TODO autogen index
			 */

			goto end;
		}

	}

	fd = open(filename, O_RDONLY);

	// check permission
	if(fd < 0){
		DEBUG("Open Failed with errno = %d\n", errno);
		DEBUG("Errno: %s\n", strerror(errno));
		if(errno == EACCES){
			evhttp_send_error(request, 403, NULL);
			goto end;
		}
		else if(errno == ENOENT){
			evhttp_send_error(request, HTTP_NOTFOUND, NULL);
			goto end;
		}
	}

	/*
	 * TODO file handler
	 * using libevent to handle file and nonblocking
	 */
	// check MIME
	if(file_stat.st_size){
		magic = magic_open(MAGIC_MIME_TYPE);
		magic_load(magic, NULL);
		magic_compile(magic, NULL);
		mime = magic_file(magic, filename);
		evhttp_add_header(headers, "Content-Type", mime);
		struct event *read_file = event_new(base, fd, EV_READ | EV_PERSIST, file_handler, request);
		event_add(read_file, NULL);
	}
	else{
		evhttp_send_reply(request, HTTP_OK, "OK", NULL);
	}

	/*
	 * Empty Response
	 */
	//evhttp_send_reply(request, HTTP_OK, "OK", NULL);
	DEBUG("request_handler\n\n");

end:
	free(buf);
}
