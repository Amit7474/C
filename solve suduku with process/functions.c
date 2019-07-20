#include "functions.h"

void checkAnswers(char *buffer, int i) {
	if (myAtoi(buffer[0]) && myAtoi(buffer[1]) && myAtoi(buffer[2]) == 1) {
		printf("Suduku No.%d is Correct!\n", i);
	} else {
		printf("Suduku No.%d is NOT Correct!\n", i);
	}
}

void openPipes(int* fd) {
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(0);
	}
}

int myAtoi(char str) {
	int res = 0;
	res = res * 10 + str - '0';
	return res;
}

void createSudukuMatrix(FILE *file, char *suduku) {
	int i, j;
	char c;
	for (i = 0; i < SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
			c = fgetc(file);
			while (!isspace(c)) {
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

int checkRows(int * isExist, char * suduku) {
	int i, j, n;
	char c;
	for (i = 0; i < SIZE; ++i) {
		initBooleanArray(isExist, SIZE);
		for (j = 0; j < SIZE; ++j) {
			c = suduku[i * SIZE + j];
			n = myAtoi(c);
			if (isExist[n - 1] == 1)
				return 0;
			isExist[n - 1] = 1;
		}
		if (!checkIsExist(isExist)) {
			return 0;
		}
	}
	return 1;
}

int checkColumns(int * isExist, char * suduku) {
	int i, j, n;
	char c;
	for (i = 0; i < SIZE; i++) {
		initBooleanArray(isExist, SIZE);
		for (j = 0; j < SIZE; ++j) {
			c = suduku[j * SIZE + i];
			n = myAtoi(c);
			if (isExist[n - 1] == 1)
				return 0;
			isExist[n - 1] = 1;
		}
	}
	if (!checkIsExist(isExist)) {
		return 0;
	}
	return 1;
}

int checkSmallMat(int * isExist, char * suduku) {
	int i, j, k, l, n;
	char c;
	for (l = 0; l < SIZE * SIZE; l = l + 27) {
		for (k = 0; k < SIZE / 3; ++k) {
			initBooleanArray(isExist, SIZE);
			for (i = 0; i < SIZE / 3; ++i) {
				for (j = 0; j < SIZE / 3; ++j) {
					c = suduku[i * SIZE + 3 * k + j + l];
					n = myAtoi(c);
					if (isExist[n - 1] == 1)
						return 0;
					isExist[n - 1] = 1;
				}
			}
			if (!checkIsExist(isExist)) {
				return 0;
			}
		}
	}
	return 1;
}

int checkIsExist(int *isExist) {
	int j;
	for (j = 0; j < SIZE; ++j) {
		if (isExist[j] == 0) {
			return 0;
		}
	}
	return 1;
}

void initBooleanArray(int *arr, int size) {
	int i;
	for (i = 0; i < size; ++i) {
		arr[i] = 0;
	}
}
