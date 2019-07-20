#include "functionsV2.h"
globalStruct global;
pthread_mutex_t tasklock;
pthread_mutex_t threadDoneLock;
pthread_mutex_t mainThreadLock;
pthread_cond_t condFinished;

int myAtoi(char str) {
	int res = 0;
	res = res * 10 + str - '0';
	return res;
}

void createSudukuMatrix(FILE *file, char *suduku) {	//Reads from the file into 2d array
	int i, j;
	char c;
	for (i = 0; i < SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
			c = fgetc(file);
			while (!isspace(c)) {		//Skip whitespace
				suduku[i * SIZE + j] = c;
				c = fgetc(file);
			}
		}
	}
}

void printMat(char * mat) {
	int i, j;
	for (i = 0; i < SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
			printf("%c ", *((mat + i * SIZE) + j));
		}
		printf("\n");
	}
}

void* checkRows(void *argv) {
	int i, j, n, isExist[SIZE];
	j = (int) argv;			//The index of the thread
	initBooleanArray(&isExist, SIZE);		//Zero the boolean array
	for (i = 0; i < SIZE; ++i) {
		n = myAtoi(global.suduku[j][i]);
		if (isExist[n - 1] == 1) {
			return 0;
		}
		isExist[n - 1] = 1;
	}
	if (!checkIsExist(isExist, SIZE)) {
		return 0;
	}
	return 1;
}

void* checkColumns(void *argv) {
	int i, n, j, isExist[SIZE];
	j = (int) argv;			//The index of the thread
	initBooleanArray(&isExist, SIZE);			//Zero the boolean array
	for (i = 0; i < SIZE; i++) {
		n = myAtoi(global.suduku[i][j]);
		if (isExist[n - 1] == 1) {
			return 0;
		}
		isExist[n - 1] = 1;
	}
	if (!checkIsExist(isExist, SIZE)) {
		return 0;
	}
	return 1;
}

void* checkSmallMat(void* argv) {
	int i, j, si, sj, n, smallSqure, isExist[SIZE];
	smallSqure = (int) argv;
	initBooleanArray(&isExist, SIZE);			//Zero the boolean array
	switch (smallSqure) {	//Switch case for choosing the small matrix to check
	case 0:
		si = 0;
		sj = 0;
	case 1:
		si = 0;
		sj = 1;
	case 2:
		si = 0;
		sj = 2;
	case 3:
		si = 1;
		sj = 0;
	case 4:
		si = 1;
		sj = 1;
	case 5:
		si = 1;
		sj = 2;
	case 6:
		si = 2;
		sj = 0;
	case 7:
		si = 2;
		sj = 1;
	case 8:
		si = 2;
		sj = 2;
	}
	for (i = 0; i < SIZE / 3; ++i) {
		for (j = 0; j < SIZE / 3; ++j) {
			n = myAtoi(global.suduku[si * 3 + i][sj * 3 + j]);
			if (isExist[n - 1] == 1) {
				return 0;
			}
			isExist[n - 1] = 1;
		}
	}
	if (!checkIsExist(isExist, SIZE)) {
		return 0;
	}
	return 1;
}

int checkIsExist(int *isExist, int size) {//Check if there is 0 in the boolean array
	int j;
	for (j = 0; j < size; ++j) {
		if (isExist[j] == 0) {
			return 0;
		}
	}
	return 1;
}

void initBooleanArray(int *arr, int size) {		//Zero the boolean array
	int i;
	for (i = 0; i < size; ++i) {
		arr[i] = 0;
	}
}
void signalFailure() {
	global.faliure = 1;		//Indicates that the check has failed
	global.currentTasks = 27;	//Force all the threads finish the tasks
	pthread_cond_signal(&condFinished);
}
void* getTask() {
	int i, task, location;
	while (global.currentTasks < NUMBER_OF_TASKS) {
		pthread_mutex_lock(&tasklock);	//Lock while the thread pulls task
		global.tasksList[global.currentTasks] += 1;	//+1 to check if task has been taken more then 1 time
		i = global.currentTasks;
		global.currentTasks++;
		pthread_mutex_unlock(&tasklock);		//Release the lock
		task = i / SIZE;//Determine the task type: 0-check rows, 1-check columns, 2-check small suduku
		location = i % SIZE;//Determine the row/column/small suduku location in the suduku
		switch (task) {
		case CHECK_ROWS:
			if (!checkRows(location)) {
				signalFailure();
			}
			break;
		case CHECK_COLUMNS:
			if (!checkColumns(location)) {
				signalFailure();
			}
			break;
		case CHECK_SMALL_MAT:
			if (!checkSmallMat(location)) {
				signalFailure();
			}
			break;
		}
	}
	pthread_mutex_lock(&threadDoneLock);
	global.done++;		//Increase the number of completed threads
	pthread_mutex_unlock(&threadDoneLock);
	//Only the last thread that finishes tasks Signals
	if (global.done == N) {
		pthread_cond_signal(&condFinished);
	}
	return NULL;
}
void writeAnswer() {
	if (global.faliure) {
		printf("solution is not legal\n");
	} else {
		global.currentTasks = 27;	//Force all the threads finish the tasks
		printf("solution is legal\n");
	}
}
void initGlobalStruct() {
	global.done = 0;
	global.faliure = 0;
	global.currentTasks = 0;
	initBooleanArray(global.tasksList, NUMBER_OF_TASKS);
}
void joinThreads(pthread_t * tid) {
	int i;
	for (i = 0; i < N; ++i) {
		pthread_join(tid[i], NULL);
	}
}
