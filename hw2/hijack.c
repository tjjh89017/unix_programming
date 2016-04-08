#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define _TO_FILE_

#ifdef _TO_FILE_
#define RED
#define BLUE
#define CLEAR
#define INIT_FILE() outfile = o_fopen("hijack.log", "a")
#define DBG(...) o_fprintf(outfile, __VA_ARGS__)
#define INFO(...) o_fprintf(outfile, __VA_ARGS__)

FILE *outfile;
#else
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define CLEAR "\033[0m"
#define INIT_FILE() do {} while(0)
#define DBG(...) o_fprintf(stderr, __VA_ARGS__)
#define INFO(...) o_fprintf(stderr, __VA_ARGS__)
#endif

#define find_origin_func(x) o_##x = dlsym(RTLD_NEXT, #x)

static int (*o_fprintf)(FILE *, const char*, ...);
static FILE* (*o_fopen)(const char*, const char*);
static void* (*o_memset)(void*, int, size_t);
static int (*o___libc_start_main)(int (*main) (int, char **, char **), int argc, char ** ubp_av, void (*init) (void), void (*fini) (void), void (*rtld_fini) (void), void (*stack_end));
static void* (*o_calloc)(size_t num, size_t size);
static size_t (*o_strlen)(const char * str);
static void* (*o_malloc)(size_t size);
static void* (*o_memcpy)(void* destination, const void* source, size_t num);
static char* (*o_strcpy)( char* destination, const char* source);
static int (*o_memcmp)(const void* ptr1, const void* ptr2, size_t num);
static int (*o_fclose)(FILE* stream);
static int (*o_strncmp)(const char* str1, const char* str2, size_t num);
static char* (*o_strchr)(char* str, int character);
static void* (*o_memchr)(void* ptr, int value, size_t num);
static char* (*o_strrchr)(char* str, int character);
static char* (*o_strpbrk)(char* str1, const char* str2);
static int (*o_fflush)(FILE* stream);
static int (*o_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
static int (*o_fputs)(const char* str, FILE* stream);
static int (*o_socket)(int domain, int type, int protocol);
static int (*o_connect)(int socket, const struct sockaddr *address, socklen_t address_len);
static const char* (*o_inet_ntop)(int af, const void *src, char *dst, socklen_t size);
static ssize_t (*o_write)(int fd, const void *buf, size_t count);
static ssize_t (*o_read)(int fd, void *buf, size_t count);
static ssize_t (*o_recv)(int sockfd, void *buf, size_t len, int flags);
static size_t (*o_fwrite)(const void* ptr, size_t size, size_t count, FILE* stream);
static int (*o_SSL_write)(void *ssl, const void *buf, int num);
static int (*o_SSL_read)(void *ssl, void *buf, int num);

static void init() __attribute__((constructor));
static void init(){

	find_origin_func(fprintf);
	find_origin_func(fopen);
	find_origin_func(memset);
	find_origin_func(__libc_start_main);
	find_origin_func(calloc);
	find_origin_func(strlen);
	find_origin_func(malloc);
	find_origin_func(memcpy);
	find_origin_func(strcpy);
	find_origin_func(memcmp);
	find_origin_func(fclose);
	find_origin_func(strncmp);
	find_origin_func(strchr);
	find_origin_func(memchr);
	find_origin_func(strrchr);
	find_origin_func(strpbrk);
	find_origin_func(fflush);
	find_origin_func(getaddrinfo);
	find_origin_func(fputs);
	find_origin_func(socket);
	find_origin_func(connect);
	find_origin_func(inet_ntop);
	find_origin_func(write);
	find_origin_func(read);
	find_origin_func(recv);
	find_origin_func(fwrite);
	find_origin_func(SSL_write);
	find_origin_func(SSL_read);

	INIT_FILE();
}

FILE *fopen(const char* a, const char* b){

	DBG(RED"*** fopen(%s, %s) ***\n"CLEAR, a, b);
	return o_fopen(a, b);
}

void *memset(void *ptr, int value, size_t num){

	DBG(RED"*** memset(%p, %d, %d)***\n"CLEAR, ptr, value, num);
	return o_memset(ptr, value, num);
}

int __libc_start_main(int (*main) (int, char **, char **), int argc, char ** ubp_av, void (*init) (void), void (*fini) (void), void (*rtld_fini) (void), void (*stack_end)){

	DBG(RED"*** __libc_start_main(%p, %d, %p, %p, %p, %p, %p)***\n"CLEAR, main, argc, ubp_av, init, fini, rtld_fini, stack_end);
	return o___libc_start_main(main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}

/*
void *calloc(size_t num, size_t size){
	
	DBG(RED"*** calloc(%d, %d) ***\n"CLEAR, num, size);
	return o_calloc(num, size);
}
*/

size_t strlen(const char* str){

	DBG(RED"*** strlen(\"%s\") ***\n"CLEAR, str);
	return o_strlen(str);
}

/*
void* malloc(size_t size){

	DBG(RED"*** malloc(0x%x) ***\n"CLEAR, size);
	return o_malloc(size);
}
*/

void* memcpy (void* destination, const void* source, size_t num){

	DBG(RED"*** memcpy(%p, %p, %d) ***\n"CLEAR, destination, source, num);
	return o_memcpy(destination, source, num);
}

char* strcpy(char* destination, const char* source){

	DBG(RED"*** strcpy(%p, %p) ***\n"CLEAR, destination, source);
	return o_strcpy(destination, source);
}

int memcmp(const void* ptr1, const void* ptr2, size_t num){

	DBG(RED"*** memcmp(%p, %p, %d) ***\n"CLEAR, ptr1, ptr2, num);
	return o_memcmp(ptr1, ptr2, num);
}

int fclose(FILE* stream){

	DBG(RED"*** fclose(%p) ***\n"CLEAR, stream);
	return o_fclose(stream);
}

int strncmp(const char* str1, const char* str2, size_t num){

	DBG(RED"*** strncmp(\"%s\", \"%s\", %d) ***"CLEAR, str1, str2, num);
	return o_strncmp(str1, str2, num);
}

char* strchr(char* str, int character){

	DBG(RED"*** strchr(\"%s\", %c) ***\n"CLEAR, str, character);
	return o_strchr(str, character);
}

void* memchr(void* ptr, int value, size_t num){

	DBG(RED"*** memchr(%p, %d, %d) ***\n"CLEAR, ptr, value, num);
	return o_memchr(ptr, value, num);
}

char* strrchr(char* str, int character){

	DBG(RED"*** strrchr(\"%s\", %c) ***\n"CLEAR, str, character);
	return o_strrchr(str, character);
}

char* strpbrk(char* str1, const char* str2){

	DBG(RED"*** strpbrk(\"%s\", \"%s\") ***\n"CLEAR, str1, str2);
	return o_strpbrk(str1, str2);
}

int fflush(FILE* stream){

	DBG(RED"*** fflush(%p) ***\n"CLEAR, stream);
	return o_fflush(stream);
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res){

	DBG(RED"*** getaddrinfo(%p, %p, %p, %p) ***\n"CLEAR, node, service, hints, res);
	INFO(BLUE"!!! Host: %s !!!"CLEAR, node);
	return o_getaddrinfo(node, service, hints, res);
}

int fputs(const char* str, FILE* stream){

	DBG(RED"*** fputs(\"%s\", %p)***\n"CLEAR, str, stream);
	return o_fputs(str, stream);
}

int socket(int domain, int type, int protocol){

	DBG(RED"*** socket(%d, %d, %d) ***\n"CLEAR, domain, type, protocol);
	return o_socket(domain, type, protocol);
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len){

	DBG(RED"*** connect(%d, %p, %d) ***\n"CLEAR, socket, address, address_len);
	return o_connect(socket, address, address_len);
}

const char* inet_ntop(int af, const void *src, char *dst, socklen_t size){

	DBG(RED"*** inet_ntop(%d, %p, %p, %d) ***\n"CLEAR, af, src, dst, size);
	const char* ip = o_inet_ntop(af, src, dst, size);
	INFO(BLUE"!!! IP: %s !!!\n"CLEAR, ip);
	return ip;
}

ssize_t write(int fd, const void *buf, size_t count){

	DBG(RED"*** write(%d, \"%s\", %d) ***\n"CLEAR, fd, buf, count);
	ssize_t result = o_write(fd, buf, count);
	INFO(BLUE"!!! Write: \"%*s\" !!!\n"CLEAR, count, (const char*)buf);
	return result;
}

ssize_t read(int fd, void *buf, size_t count){

	DBG(RED"*** read(%d, \"%s\", %d) ***\n"CLEAR, fd, buf, count);
	ssize_t result = o_read(fd, buf, count);
	INFO(BLUE"!!! Read: \"%*s\" !!!\n"CLEAR, count, (char*)buf);
	return result;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags){

	DBG(RED"*** recv(%d, %p, %d, %d) ***\n"CLEAR, sockfd, buf, len, flags);
	ssize_t result = o_recv(sockfd, buf, len, flags);
	INFO(BLUE"!!! Recv: \"%*s\" !!!\n"CLEAR, len, (char*)buf);
	return result;
}

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream){

	DBG(RED"*** fwrite(\"%s\", %d, %d, %p) ***\n"CLEAR, ptr, size, count ,stream);
	INFO(BLUE"!!! Fwrite: \"%*s\" !!!\n"CLEAR, count * size, ptr);
	return o_fwrite(ptr, size, count, stream);
}

int SSL_write(void *ssl, const void *buf, int num){
	
	DBG(RED"*** SSL_write(%p, \"%s\", %d) ***\n"CLEAR, ssl, buf, num);
	INFO(BLUE"!!! SSL_write: \"%*s\" !!!\n"CLEAR, num, buf);
	return o_SSL_write(ssl, buf, num);
}

int SSL_read(void *ssl, void *buf, int num){

	DBG(RED"*** SSL_read(%p, \"%s\", %d) ***\n"CLEAR, ssl, buf, num);
	int result = o_SSL_read(ssl, buf, num);
	INFO(BLUE"!!! SSL_read: \"%*s\" !!!\n"CLEAR, num, buf);
	return result;
}
