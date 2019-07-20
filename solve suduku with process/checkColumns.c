#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char * argv) {
	int isExist[SIZE], answer = 0;
	char suduku[SIZE][SIZE], buffer;
	while (read(STDIN_FILENO, suduku, SIZE * SIZE) != 0) { //Keep reading while there is suduku to read
		answer = checkColumns(&isExist, &suduku);
		buffer = answer + '0';
		write(STDOUT_FILENO, &buffer, sizeof(buffer));
		sleep(0.5);
	}
	return EXIT_SUCCESS;
}
