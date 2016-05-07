#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <glob.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifdef __DEBUG__
#define DEBUG(...) \
do { \
	fprintf(stderr, "\033[31m");\
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\033[0m");\
} while(0)
#else
#define DEBUG(...) do {} while(0)
#endif

#define prompt(user, hostname, path) printf("\033[92m[%s@%s %s(Yosh)]$\033[0m ", user, hostname, path)
#define MAX_SIZE 256
#define IN 0
#define OUT 1

// get command
// sep pipe operator
// sep redirect operator
// parse command
// exec command
int run(char *cmd, int input, int first, int last);
void split(char *cmd, glob_t *);
char *skipwhite(char *s);
void cleanup(int n);
int command(int input, int first, int last);
void interrupt_handler(int sig);
int cd();
void update_prompt();
int export();
int unset();

char* args[512];
int n = 0;
char user[128];
char hostname[128];
char path[128];
char *infile = NULL, *outfile = NULL;

void init()
{
	update_prompt();

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

	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGINT, interrupt_handler);
 
}

int main()
{

	init();
	char input_line[MAX_SIZE];

	while(1){
		prompt(user, hostname, path);
		if(!fgets(input_line, MAX_SIZE, stdin)){
			puts("");
			return 0;
		}

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
	glob_t results;
	split(cmd, &results);
	if(args[0] != NULL){
		if(strcmp(args[0], "exit") == 0)
			exit(0);
		else if(strcmp(args[0], "cd") == 0)
			return cd();
		else if(strcmp(args[0], "export") == 0)
			return export();
		else if(strcmp(args[0], "unset") == 0)
			return unset();
		n++;
		int ret = command(input, first, last);
		globfree(&results);
	}
	return 0;
}

char *skipwhite(char *s)
{
	while(isspace(*s))
		s++;
	return s;
}

static int globerr(const char *path, int eerrno)
{
	fprintf(stderr, "%s: %s\n", path, strerror(eerrno));
	return 0;
}

void split(char *cmd, glob_t *results)
{
	int flags = 0;
	cmd = skipwhite(cmd);
	char *next = strchr(cmd, ' ');
	int i = 0, j = 0;
	while(next != NULL){
		next[0] = '\0';
		args[i] = cmd;

		// expand * ?
		//glob_t results;
		int ret;
		if(ret = glob(cmd, flags, globerr, results)){
			// failed
			DEBUG("glob failed: %d\n", ret);
			if(ret == GLOB_NOMATCH)
				DEBUG("GLOB_NOMATCH\n");
			if(ret == GLOB_ABORTED)
				DEBUG("GLOB_ABORTED\n");
			if(ret == GLOB_NOSPACE)
				DEBUG("GLOB_NOSPACE\n");
			i++;
		}

		flags |= GLOB_APPEND;
		DEBUG("results.gl_pathc = %d\n", results->gl_pathc);
		for(; j < results->gl_pathc; j++){
			DEBUG("results.gl_pathv[%d] = %s\n", j, results->gl_pathv[j]);
			args[i] = results->gl_pathv[j];
			i++;
		}
		//globfree(&results);

		cmd = skipwhite(next + 1);
		next = strchr(cmd, ' ');
	}
	if(cmd[0] != '\0'){
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		// expand * ?
		//glob_t results;
		int ret;
		if(ret = glob(cmd, flags, globerr, results)){
			// failed
			DEBUG("glob failed: %d\n", ret);
			if(ret == GLOB_NOMATCH)
				DEBUG("GLOB_NOMATCH\n");
			if(ret == GLOB_ABORTED)
				DEBUG("GLOB_ABORTED\n");
			if(ret == GLOB_NOSPACE)
				DEBUG("GLOB_NOSPACE\n");
			i++;
		}
		DEBUG("results.gl_pathc = %d\n", results->gl_pathc);
		for(; j < results->gl_pathc; j++){
			DEBUG("results.gl_pathv[%d] = %s\n", j, results->gl_pathv[j]);
			args[i] = results->gl_pathv[j];
			i++;
		}
		//globfree(&results);
	}
	args[i] = NULL;
}

void cleanup(int n)
{
	while(n--)
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
	
		signal(SIGTTOU, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGCONT, SIG_DFL);

		//search for file
		infile = NULL, outfile = NULL;
		int i = 0, infile_no = -1, outfile_no = -1;
		for(i = 0; i < 512 && args[i]; i++){
			if(args[i][0] == '<'){
				infile_no = i;
			}
			if(args[i][0] == '>'){
				outfile_no = i;
			}
		}
		if(infile_no >= 0){
			args[i] = args[infile_no];
			args[i + 1] = args[infile_no + 1];
			args[infile_no] = NULL;
			infile = args[i + 1];
			if(args[i][1] != '\0'){
				infile = args[i] + 1;
			}
			i = i + 2;
		}
		if(outfile_no >= 0){
			args[i] = args[outfile_no];
			args[i + 1] = args[outfile_no + 1];
			args[outfile_no] = NULL;
			outfile = args[i + 1];
			if(args[i][1] != '\0'){
				infile = args[i] + 1;
			}
			i = i + 2;
		}


		// file
		if(infile){
			DEBUG("testing command infile: %s\n", infile);
			int infd = open(infile, O_RDONLY);
			if(infd < 0){
				fprintf(stderr, "Error!\n");
				_exit(EXIT_FAILURE);
			}
			dup2(infd, STDIN_FILENO);
			close(infd);
			infile = NULL;
			DEBUG("testing command infile\n");
		}

		if(outfile){
			DEBUG("testing command outfile: %s\n", outfile);
			int outfd = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			if(outfd < 0){
				fprintf(stderr, "Error!\n");
				_exit(EXIT_FAILURE);
			}
			dup2(outfd, STDOUT_FILENO);
			close(outfd);
			outfile = NULL;
			DEBUG("testing command outfile\n");
		}

		for(int x = 0; args[x] != NULL; x++){
			DEBUG("args[%d] = %s\n", x, args[x]);
		}

		DEBUG("testing execvp\n");
		if(execvp(args[0], args) == -1){
			fprintf(stderr, "-yosh: %s: command not found\n", args[0]);	
			_exit(EXIT_FAILURE);
		}
	}

	if(input != 0)
		close(input);
	close(pipes[OUT]);

	if(last == 1)
		close(pipes[IN]);

	return pipes[IN];
}

void interrupt_handler(int sig)
{
	if(n == 0){
		puts("");
		prompt(user, hostname, path);
		fflush(stdout);
	}
	return;
}

int cd()
{
	char path[1024];
	struct stat info;

	// cd to home
	DEBUG("path: %s\n", args[1]);
	if(args[1] == NULL){
		setenv("OLDPWD", getenv("PWD"), 1);
		chdir(getenv("HOME"));
		setenv("PWD", getenv("HOME"), 1);
		update_prompt();
		return 0;
	}
	else if(strcmp(args[1], "-") == 0){
		strncpy(path, getenv("PWD"), 1024);
		chdir(getenv("OLDPWD"));
		setenv("PWD", getenv("OLDPWD"), 1);
		setenv("OLDPWD", path, 1);
		update_prompt();
		return 0;
	}

	realpath(args[1], path);
	DEBUG("realpath: %s\n", path);

	if(chdir(args[1])){
		fprintf(stderr, "-yosh: cd: %s: No such file or directory\n", args[1]);
		return 1;
	}
	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", path, 1);
	update_prompt();
	return 0;
}

void update_prompt()
{
	// init user
	strncpy(user, getenv("USER"), 128);
	// init hostname
	strncpy(hostname, getenv("HOSTNAME"), 128);
	*strchr(hostname, '.') = '\0';
	// init path
	strncpy(path, strrchr(getenv("PWD"), '/') + 1, 128);
}

int export()
{
	char *variable = args[1];
	char value[1024] = "";
	int offset = 0;
	if(args[2] != NULL){
		offset += snprintf(value, 1024 - offset, "%s", args[2]);
		for(int i = 3; args[i] != NULL; i++){
			offset += snprintf(value + offset, 1024 - offset, " %s", args[i]);
		}
	}
	DEBUG("value: %s\n", value);

	setenv(variable, value, 1);
	return 0;
}

int unset()
{
	for(int i = 1; args[i] != NULL; i++){
		DEBUG("unset variable: %s\n", args[i]);
		unsetenv(args[i]);
	}
	return 0;
}
