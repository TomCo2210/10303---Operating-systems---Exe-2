#include "ThreadsV1.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

#include "CheckBoard.h"

sudokuboard* sudokuBoard;

int main(int argc, char **argv) {
	bool goodAnswer = TRUE;
	int i, taskNum[27], returnValue;
	char boardAsString[STRING_BOARD_SIZE];
	int fd = 0;
	char* nameOfFile = "matrix";
	pthread_t allThreads[27];

	if (argc == 1) {
		readBoard(boardAsString, 0);
	} else {
		nameOfFile = argv[1];
		fd = open(nameOfFile, O_RDONLY);
		if (fd == -1) {
			exit(EXIT_FAILURE);
		}

		readBoard(boardAsString, fd);
		close(fd);
	}

	sudokuBoard = (sudokuboard*) calloc(1, sizeof(sudokuboard));

	boardAtoI(boardAsString, STRING_BOARD_SIZE,
			sudokuBoard->board);

	for (i = 0; i < 27; i++) {
		taskNum[i] = i;
		returnValue = pthread_create(&allThreads[i], NULL, threadMission,
				(void*) &taskNum[i]);
		if (returnValue) {
			free(sudokuBoard);
			exit(EXIT_FAILURE);
		}

	}

	for (i = 0; i < 27; i++) {
		returnValue = pthread_join(allThreads[i], NULL);
		if (returnValue) {
			free(sudokuBoard);
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < 27; i++) {
		if (sudokuBoard->result[i] == 0) {
			goodAnswer = FALSE;
			break;
		}
	}

	free(sudokuBoard);
	if (goodAnswer)
		fprintf(stdout, "%s is legal\n", nameOfFile);
	else
		fprintf(stdout, "%s is not legal\n", nameOfFile);

	return 0;
}

void* threadMission(void* arg) {
	int taskNum = *(int*) arg;
	if (checkBoard(taskNum, sudokuBoard->board) == TRUE)
		sudokuBoard->result[taskNum] = 1;
	else
		sudokuBoard->result[taskNum] = 0;

	return NULL;
}

