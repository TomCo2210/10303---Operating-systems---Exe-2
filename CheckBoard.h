#ifndef CHECKBOARD_H_
#define CHECKBOARD_H_

#include "Utilities.h"

bool checkBoard(int taskNum, int* boardAsArray);
int checkRows(int* arr, int row) ;
int checkColumns(int* arr, int col);
int checkSquare(int* arr, int box);

#endif /* CHECKBOARD_H_ */
