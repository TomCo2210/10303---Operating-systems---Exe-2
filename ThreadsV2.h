
#ifndef THREADSV2_H_
#define THREADSV2_H_

typedef struct {
	int missions[27];
	int currentMission;
	int completedMissionsCounter;
} sudokuMissions;

#define N 10

void* threadMission(void* arg);

#endif /* THREADSV2_H_ */
