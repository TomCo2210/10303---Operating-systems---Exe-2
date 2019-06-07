
#include "ThreadsV2.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#include "CheckBoard.h"
#include "Utilities.h"

sudokuMissions* mSudokuMissions;

int board[81];
int resultOfBoard;

pthread_mutex_t tasksMutex;
pthread_mutex_t resultMutex;
pthread_cond_t condition;

int main(int argc, char **argv) {
	bool goodAnswer = TRUE;
	int i, returnValue, tempNumOfCompletedTasks = 0;
	char boardAsString[STRING_BOARD_SIZE];
	int fd = 0;
	char* nameOfFile = "matrix";
	pthread_t allThreads[N];

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

	mSudokuMissions = (sudokuMissions*) calloc(1, sizeof(sudokuMissions));

	resultOfBoard = 1;

	mSudokuMissions->completedMissionsCounter = 0;
	mSudokuMissions->currentMission = 0;
	for (i = 0; i < 27; i++) {
		mSudokuMissions->missions[i] = i;
	}

	boardAtoI(boardAsString, STRING_BOARD_SIZE, board);

	pthread_mutex_init(&tasksMutex, NULL);
	pthread_mutex_init(&resultMutex, NULL);
	pthread_cond_init(&condition, NULL);

	pthread_mutex_lock(&tasksMutex);

	for (i = 0; i < N; i++) {
		returnValue = pthread_create(&allThreads[i], NULL, threadMission,
				(void*) NULL);
		if (returnValue) {
			free(mSudokuMissions);
			exit(EXIT_FAILURE);
		}

	}

	while (tempNumOfCompletedTasks < 27) {
		pthread_cond_wait(&condition, &tasksMutex);
		tempNumOfCompletedTasks = mSudokuMissions->completedMissionsCounter;
	}
	pthread_mutex_unlock(&tasksMutex);

	if (resultOfBoard == 0) {
		goodAnswer = FALSE;
	}

	free(mSudokuMissions);

	if (goodAnswer)
		fprintf(stdout, "%s is legal\n", nameOfFile);
	else
		fprintf(stdout, "%s is not legal\n", nameOfFile);

	for (i = 0; i < N; i++) {
		returnValue = pthread_join(allThreads[i], NULL);
		if (returnValue) {
			free(mSudokuMissions);
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_destroy(&tasksMutex);
	pthread_mutex_destroy(&resultMutex);
	pthread_cond_destroy(&condition);

	return 0;
}

void* threadMission(void* arg) {
	int taskNum = 0;
	bool resultOfTask = FALSE;

	for (;;) {
		pthread_mutex_lock(&tasksMutex);
		if (mSudokuMissions->currentMission >= 27) {
			pthread_cond_signal(&condition);
			pthread_mutex_unlock(&tasksMutex);
			break;
		}
		taskNum = mSudokuMissions->missions[mSudokuMissions->currentMission++];
		pthread_mutex_unlock(&tasksMutex);

		resultOfTask = checkBoard(taskNum, board);

		if (resultOfBoard == 1 && resultOfTask == FALSE) {
			pthread_mutex_lock(&resultMutex);
			resultOfBoard = 0;
			pthread_mutex_unlock(&resultMutex);
		}
		pthread_mutex_lock(&tasksMutex);
		mSudokuMissions->completedMissionsCounter++;
		pthread_cond_signal(&condition);
		pthread_mutex_unlock(&tasksMutex);
	}

	return NULL;
}

