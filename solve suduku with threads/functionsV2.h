#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdio.h>
#include <pthread.h>
#define SIZE 9
#define N 10
#define NUMBER_OF_TASKS 27
#define CHECK_ROWS 0
#define CHECK_COLUMNS 1
#define CHECK_SMALL_MAT 2
typedef struct {
	char suduku[SIZE][SIZE];			//Global suduku
	int tasksList[NUMBER_OF_TASKS];		//Global tasks array
	int done;			//done threads Counter
	int currentTasks;
	int faliure;	//Indicator for failure
} globalStruct;
extern globalStruct global;
extern pthread_mutex_t tasklock;
extern pthread_cond_t condFinished;
extern pthread_mutex_t threadDoneLock;
int myAtoi(char str);
void createSudukuMatrix(FILE *file, char *suduku);
void printMat(char * mat);
void* checkRows(void *arg);
void* checkColumns(void *argv);
void* checkSmallMat(void* argv);
int checkIsExist(int *isExist, int size);
void initBooleanArray(int *isExist, int size);
void signalFailure();
void* getTask();
void writeAnswer();
void initGlobalStruct();
void joinThreads(pthread_t * tid);
#endif /* FUNCTIONS_H_ */
