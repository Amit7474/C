#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdio.h>
#define SIZE 9
#define NUMBER_OF_CHILDS 3
void checkAnswers(char *buffer, int i);
void openPipes(int* fd);
int myAtoi(char str);
void createSudukuMatrix(FILE *file, char *suduku);
void printMat(char * mat);
int checkRows(int * isExist, char * suduku);
int checkColumns(int * isExist, char * suduku);
int checkSmallMat(int * isExist, char * suduku);
int checkIsExist(int *isExist);
void initBooleanArray(int *isExist, int size);
#endif /* FUNCTIONS_H_ */
