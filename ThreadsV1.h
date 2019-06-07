
#ifndef THREADSV1_H_
#define THREADSV1_H_

typedef struct {
	int board[81];
	char result[27];
} sudokuboard;

void* threadMission(void* arg);

#endif /* THREADSV1_H_ */
