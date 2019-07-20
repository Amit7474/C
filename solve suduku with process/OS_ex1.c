#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"

int main(int argc, char* argv[]) {
	int i, j, fd1[2], fd2[2], fd3[2], fd4[2];
	FILE *file;
	char suduku[SIZE][SIZE];
	char buffer[NUMBER_OF_CHILDS];
	//Open 4 pipes
	openPipes(&fd1);		//Send the suduku matrix
	openPipes(&fd2);		//Receive answer
	openPipes(&fd3);		//Receive answer
	openPipes(&fd4);		//Receive answer
	switch (fork()) {				//Fork 1st child
	case -1:
		perror("fork error");
		exit(EXIT_FAILURE);
	case 0:						//1st Child process
		//Close useless file descriptors
		close(fd2[0]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd4[0]);
		close(fd4[1]);
		close(fd1[1]);
		//Dup2 to the read side of the child's pipe for reading suduku
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) == -1)
				perror("dup2");
			if (close(fd1[0]) == -1)
				perror("close 2");
		}
		////Dup2 to the write side of the child's pipe for writing answer
		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) == -1)
				perror("dup2");
			if (close(fd2[1]) == -1)
				perror("close 2");
		}
		//Execlp to check rows
		execlp("./checkRow", "./checkRow", (char*) NULL);
		printf("cannot execlp..");

	default:
		//Parent procces
		break;
	}
	switch (fork()) {				//Fork 2nd child
	case -1:
		perror("fork error");
		exit(EXIT_FAILURE);
	case 0:					///2nd Child process
		//Close useless file descriptors
		close(fd2[0]);
		close(fd2[1]);
		close(fd3[0]);
		close(fd4[0]);
		close(fd4[1]);
		close(fd1[1]);
		//Dup2 to the read side of the child's pipe for reading suduku
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) == -1)
				perror("dup2");
			if (close(fd1[0]) == -1)
				perror("close 2");
		}
		////Dup2 to the write side of the child's pipe for writing answer
		if (fd3[1] != STDOUT_FILENO) {
			if (dup2(fd3[1], STDOUT_FILENO) == -1)
				perror("dup2");
			if (close(fd3[1]) == -1)
				perror("close 2");
		}
		//Execlp to check columns
		execlp("./checkCol", "./checkCol", (char*) NULL);
		printf("cannot execlp..");

	default:		//Parent process
		break;
	}
	switch (fork()) {				//Fork 3rd child
	case -1:
		perror("fork error");
		exit(EXIT_FAILURE);
	case 0:				///3rd Child process
		//Close useless file descriptors
		close(fd2[0]);
		close(fd2[1]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd4[0]);
		close(fd1[1]);
		//Dup2 to the read side of the child's pipe for reading suduku
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) == -1)
				perror("dup2");
			if (close(fd1[0]) == -1)
				perror("close 2");
		}
		////Dup2 to the write side of the child's pipe for writing answer
		if (fd4[1] != STDOUT_FILENO) {
			if (dup2(fd4[1], STDOUT_FILENO) == -1)
				perror("dup2");
			if (close(fd4[1]) == -1)
				perror("close 2");
		}
		//Execlp to check small martix
		execlp("./checkSmall", "./checkSmall", (char*) NULL);
		printf("cannot execlp..");

	default:		//Parent process
		break;
	}

	if (argc < 2) {
		printf("Please enter file name as input...\n");
	} else {
		//Close useless file descriptors
		close(fd3[1]);
		close(fd4[1]);
		close(fd2[1]);
		close(fd1[0]);
		//Open the suduku file
		for (i = 1; i < argc; i++) {
			file = fopen(argv[i], "r");
			if (!file) {
				printf("Cannot open file!\n");
				exit(0);
			}
			//Create matrix from suduku file
			createSudukuMatrix(file, &suduku);
			//Write the suduku to the children in pipe
			for (j = 0; j < NUMBER_OF_CHILDS; ++j) {
				write(fd1[1], suduku, SIZE * SIZE);
			}
			//Read result from pipe
			read(fd2[0], &buffer[0], sizeof(char));
			read(fd3[0], &buffer[1], sizeof(char));
			read(fd4[0], &buffer[2], sizeof(char));
			//Give the final result
			checkAnswers(&buffer, i);
			//Close the file
			fclose(file);
		}
		//Close all the file descriptors
		close(fd2[0]);
		close(fd1[1]);
		close(fd3[0]);
		close(fd4[0]);
		//Waiting for the process to finish
		wait(NULL);
		wait(NULL);
		wait(NULL);
	}

	return EXIT_SUCCESS;
}
