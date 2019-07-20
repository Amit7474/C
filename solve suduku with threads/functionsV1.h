#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdio.h>
#include <pthread.h>
#define SIZE 9
#define NUMBER_OF_THREADS 27
typedef struct {
	char suduku[SIZE][SIZE];			//Global suduku
	int threadsAnswer[NUMBER_OF_THREADS];		//Global answers array
} globalStruct;
extern globalStruct global;
int myAtoi(char str);
void createSudukuMatrix(FILE *file, char *suduku);
void printMat(char * mat);
void* checkRows(void *arg);
void* checkColumns(void *argv);
void* checkSmallMat(void* argv);
int checkIsExist(int *isExist, int size);
void initBooleanArray(int *isExist, int size);
void writeAnswer();
void joinThreads(pthread_t * tid);
#endif /* FUNCTIONS_H_ */
