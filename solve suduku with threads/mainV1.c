#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include "functionsV1.h"
globalStruct global;		//Define global structure

int main(int argc, char * argv[]) {
	int i;
	FILE *file;
	pthread_t tid[NUMBER_OF_THREADS];
	if (argc < 2) {								//check if file name is given
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
	initBooleanArray(&global.threadsAnswer, NUMBER_OF_THREADS);
	//9 threads for checking the rows
	for (i = 0; i < SIZE; i++) {
		pthread_create(&tid[i], NULL, checkRows, i);//Create threads to check rows
	}
	//9 threads for checking the columns
	for (i = 0; i < SIZE; i++) {
		pthread_create(&tid[i + SIZE], NULL, checkColumns, i);//Create threads to check columns
	}
	//9 threads for checking the small matrix
	for (i = 0; i < SIZE; i++) {
		pthread_create(&tid[i + SIZE * 2], NULL, checkSmallMat, i);	//Create threads to check small matrix
	}
	joinThreads(&tid);		//Wait and Clean up the threads
	writeAnswer();			//Write the answer

	return EXIT_SUCCESS;
}
