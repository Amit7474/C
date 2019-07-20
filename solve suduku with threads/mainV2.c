#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include "functionsV2.h"
globalStruct global;		//Define global structure
pthread_mutex_t tasklock;		//Lock on task list
pthread_mutex_t threadDoneLock;		//Lock on done threads counter
pthread_mutex_t mainThreadLock;	//Lock for the 'wait' in the main thread
pthread_cond_t condFinished;	//Condition variable for finishing the tasks

int main(int argc, char * argv[]) {
	int i;
	FILE *file;
	pthread_t tid[N];
	if (argc < 2) {							//check if file name is given
		printf(
				"Please enter single suduku WITHOUT spaces. press enter when finished;\n");
		fgets(global.suduku, sizeof(global.suduku), stdin);
	} else {
		file = fopen(argv[1], "r");							//Open the file
		if (!file) {
			printf("Cannot open file!");
			exit(0);
		}
		createSudukuMatrix(file, &global.suduku);//Create suduku from the file
	}
	if (pthread_mutex_init(&tasklock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return EXIT_FAILURE;
	}
	if (pthread_mutex_init(&threadDoneLock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return EXIT_FAILURE;			//Initialize locks & condition variable
	}
	if (pthread_mutex_init(&mainThreadLock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return EXIT_FAILURE;
	}
	if (pthread_cond_init(&condFinished, NULL) != 0) {
		printf("\n condition init has failed\n");
		return EXIT_FAILURE;
	}
	initGlobalStruct();
	//Create threads
	for (i = 0; i < N; ++i) {
		pthread_create(&tid[i], NULL, getTask, NULL);
	}
	pthread_mutex_lock(&mainThreadLock);
	pthread_cond_wait(&condFinished, &mainThreadLock);
	pthread_mutex_unlock(&mainThreadLock);
	writeAnswer();			//Write if the solution is legal or not

	//Clean up and exit

	joinThreads(&tid);			//Join for all threads
	pthread_cond_destroy(&condFinished);
	pthread_mutex_destroy(&tasklock);
	pthread_mutex_destroy(&threadDoneLock);
	pthread_mutex_destroy(&mainThreadLock);
	return EXIT_SUCCESS;
}
