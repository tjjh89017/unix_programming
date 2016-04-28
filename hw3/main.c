#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __DEBUG__
#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#else
#define DEBUG(...)
#endif

#define prompt(x) printf("[user@host path]$ ")
#define MAX_SIZE 256

// get command
// sep pipe operator
// sep redirect operator
// parse command
// exec command

int get_command(char *buf){
	fgets(buf, MAX_SIZE, stdin);
	return 0;
}

int main(){

	char command[MAX_SIZE];

	while(1){
		prompt(0);
		get_command(command);
		DEBUG("%s\n", command);
	}

}
