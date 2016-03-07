#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DELAY_TIME 100000

extern void random_length_task();

int main(int argc, char* argv[]) {
	int n = 10;
	
	if(argc == 2){
		n = atoi(argv[1]);
	}

	srand(time(0) ^ getpid());
	struct timeval origin, current;
	while(n-- > 0) {
		gettimeofday(&origin, NULL);
		random_length_task();
		gettimeofday(&current, NULL);

		while(current.tv_usec - origin.tv_usec > DELAY_TIME) {
			gettimeofday(&current, NULL);
		}
	}
	return 0;
}

