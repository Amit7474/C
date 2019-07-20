#include "functionsV1.h"
#include <pthread.h>
globalStruct global;

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
			global.threadsAnswer[j] = 0;//Write answer in the specific place of this thread in the answer array
			return 0;
		}
		isExist[n - 1] = 1;
	}
	if (!checkIsExist(isExist, SIZE)) {
		global.threadsAnswer[j] = 0;//Write answer in the specific place of this thread in the answer array
		return 0;
	}
	global.threadsAnswer[j] = 1;//Write answer in the specific place of this thread in the answer array
	return 1;
}

void* checkColumns(void *argv) {
	int i, n, j, isExist[SIZE];
	j = (int) argv;			//The index of the thread
	initBooleanArray(&isExist, SIZE);			//Zero the boolean array
	for (i = 0; i < SIZE; i++) {
		n = myAtoi(global.suduku[i][j]);
		if (isExist[n - 1] == 1) {
			global.threadsAnswer[j + SIZE] = 0;	//Write answer in the specific place of this thread in the answer array
			return 0;
		}
		isExist[n - 1] = 1;
	}
	if (!checkIsExist(isExist, SIZE)) {
		global.threadsAnswer[j + SIZE] = 0;	//Write answer in the specific place of this thread in the answer array
		return 0;
	}
	global.threadsAnswer[j + SIZE] = 1;	//Write answer in the specific place of this thread in the answer array
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
				global.threadsAnswer[smallSqure + SIZE * 2] = 0;//Write answer in the specific place of this thread in the answer array
				return 0;
			}
			isExist[n - 1] = 1;
		}
	}
	if (!checkIsExist(isExist, SIZE)) {
		global.threadsAnswer[smallSqure + SIZE * 2] = 0;//Write answer in the specific place of this thread in the answer array
		return 0;
	}
	global.threadsAnswer[smallSqure + SIZE * 2] = 1;//Write answer in the specific place of this thread in the answer array
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
void writeAnswer() {
	if (checkIsExist(&global.threadsAnswer, NUMBER_OF_THREADS)) {//Check the final answer array after all the threads finished the tasks) {
		printf("solution is legal\n");
	} else {
		printf("solution is not legal\n");
	}
}
void joinThreads(pthread_t * tid) {
	int i;
	for (i = 0; i < NUMBER_OF_THREADS; ++i) {
		pthread_join(tid[i], NULL);
	}
}
