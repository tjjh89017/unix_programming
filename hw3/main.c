#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/wait.h>
#include <sys/types.h>

#ifdef __DEBUG__
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...) do {} while(0)
#endif

#define prompt(user, hostname, path) printf("\033[92m[%s@%s %s]$\033[0m ", user, hostname, path)
#define MAX_SIZE 256
#define IN 0
#define OUT 1

// get command
// sep pipe operator
// sep redirect operator
// parse command
// exec command
int run(char *cmd, int input, int first, int last);
void split(char *cmd);
char *skipwhite(char *s);
void cleanup(int n);
int command(int input, int first, int last);

char* args[512];
int n = 0;
char user[128];
char hostname[128];
char path[128];

void init()
{
	// init user
	strncpy(user, getenv("USER"), 128);
	// init hostname
	strncpy(hostname, getenv("HOSTNAME"), 128);
	*strchr(hostname, '.') = '\0';
	// init path
	strncpy(path, strrchr(getenv("PWD"), '/') + 1, 128);

	puts("Welcome to Yoda shell!");
	puts("Finish my homework, have I?");
	puts("");
	printf("\033[93m");
	puts("      8888888888  888    88888             ");
	puts("     88     88   88 88   88  88            ");
	puts("      8888  88  88   88  88888             ");
	puts("         88 88 888888888 88   88           ");
	puts("  88888888  88 88     88 88    888888      ");
	puts("                                           ");
	puts("  88  88  88   888    88888    888888      ");
	puts("  88  88  88  88 88   88  88  88           ");
	puts("  88 8888 88 88   88  88888    8888        ");
	puts("   888  888 888888888 88   88     88       ");
	puts("    88  88  88     88 88    8888888        ");
	puts("\033[0m");

	printf("\033[32m");
	puts("           .--.                  Try not.    ");
	puts(" ::\\`--._,'.::.`._.--'/::     Do or do not. ");
	puts(" ::::.  ` __::__ '  .::::    There is no try.");
	puts(" ::::::-:.`'..`'.:-::::::                    ");
	puts(" ::::::::\\ `--' /::::::::              -Yoda");
	puts("\033[0m");
 
}

int main()
{

	init();
	char input_line[MAX_SIZE];

	while(1){
		prompt(user, hostname, path);
		if(!fgets(input_line, MAX_SIZE, stdin))
			return 0;

		int input = 0;
		int first = 1;
		char *cmd = input_line;
		char *next = strchr(cmd, '|');
		while(next != NULL){
			*next = '\0';
			input = run(cmd, input, first, 0);

			// next command
			cmd = next + 1;
			next = strchr(cmd, '|');
			first = 0;
		}
		input = run(cmd, input, first, 1);
		cleanup(n);
		n = 0;
	}
	
	return 0;
}

int run(char *cmd, int input, int first, int last)
{
	split(cmd);
	
	n++;
	return command(input, first, last);
}

char *skipwhite(char *s)
{
	while(isspace(*s))
		s++;
	return s;
}

void split(char *cmd)
{
	cmd = skipwhite(cmd);
	char *next = strchr(cmd, ' ');
	int i = 0;
	while(next != NULL){
		next[0] = '\0';
		args[i] = cmd;
		i++;
		cmd = skipwhite(next + 1);
		next = strchr(cmd, ' ');
	}
	if(cmd[0] != '\0'){
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		i++;
	}
	args[i] = NULL;
}

void cleanup(int n)
{
	int t = n;
	while(t--)
		wait(NULL);
}

int command(int input, int first, int last)
{
	DEBUG("input: %d, first: %d, last: %d\n", input, first, last);
	DEBUG("cmd: %s\n", args[0]);

	int pipes[2];
	pipe(pipes);
	pid_t pid = fork();

	if(pid == 0){
		// child
		if(first == 1 && last == 0 && input == 0){
			dup2(pipes[OUT], STDOUT_FILENO);
		}
		else if(first == 0 && last == 0 && input != 0){
			dup2(input, STDIN_FILENO);
			dup2(pipes[OUT], STDOUT_FILENO);
		}
		else{
			dup2(input, STDIN_FILENO);
		}

		if(execvp(args[0], args) == -1)
			_exit(EXIT_FAILURE);
	}

	if(input != 0)
		close(input);
	close(pipes[OUT]);

	if(last == 1)
		close(pipes[IN]);

	return pipes[IN];
	
	return 0;
}
