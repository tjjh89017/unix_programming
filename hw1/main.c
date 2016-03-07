#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DELAY_TIME 100000

extern void random_length_task();

int main(int argc, char* argv[]) {
	int n = 10;
	struct timeval origin;
	struct timeval current;
	
	if(argc == 2){
		n = atoi(argv[1]);
	}

	srand(time(0));
	while(n--){
		gettimeofday(&origin, NULL);
		random_length_task();
		gettimeofday(&current, NULL);

		while((1000000 + current.tv_usec - origin.tv_usec) % 1000000 < DELAY_TIME){
			gettimeofday(&current, NULL);
		}

	}

	return 0;
}

