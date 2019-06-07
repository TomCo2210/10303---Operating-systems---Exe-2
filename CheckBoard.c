#include "CheckBoard.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

bool checkBoard(int taskNum, int* board) {
	int i, j;
	unsigned int answer = 0;

	for (i = 0; i < 9; i++) {
		if (0 <= taskNum && taskNum <= 8) {
			answer = checkRows(board, i);
		} else if (9 <= taskNum && taskNum <= 17) {
			answer = checkColumns(board, i);
		} else if (18 <= taskNum && taskNum <= 26) {
			answer = checkSquare(board, i);
		}

		if (!answer)
			return FALSE;
	}
	return TRUE;
}

int checkRows(int* arr, int row) {
	int numbers[ROWS] = { 0 };
	int i, neededIndex = 0;
	for (i = 0; i < ROWS; i++) {
		neededIndex = i + row * ROWS;
		numbers[arr[neededIndex] - 1]++;
		if (numbers[arr[neededIndex] - 1] > 1)
			return FALSE;
	}

	return TRUE;
}
int checkColumns(int* arr, int col) {
	int numbers[COLUMNS] = { 0 };
	int i, neededIndex = 0;
	for (i = 0; i < COLUMNS; i++) {
		neededIndex = col + i * COLUMNS;
		numbers[arr[neededIndex] - 1]++;
		if (numbers[arr[neededIndex] - 1] > 1)
			return FALSE;
	}

	return TRUE;
}
int checkSquare(int* arr, int box) {
	int numbers[SQUARE] = { 0 };
	int rows, cols, neededIndex = 0;
	for (rows = 0; rows < SQUARE_DIM; ++rows) {
		for (cols = 0; cols < SQUARE_DIM; ++cols) {
			neededIndex = ((cols / 3) + (rows / 3) * 3) * SQUARE + (cols % 3)
					+ (rows % 3) * SQUARE_DIM;
			numbers[arr[neededIndex] - 1]++;
			if (numbers[arr[neededIndex] - 1] > 1) {
				return FALSE;
			}
		}
	}
	return TRUE;
}
