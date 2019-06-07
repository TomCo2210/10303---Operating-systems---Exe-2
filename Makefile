all: v1 v2

v1:
	gcc -pthread -o v1 ThreadsV1.c CheckBoard.c Utilities.c
v2:
	gcc -pthread -o v2 ThreadsV2.c CheckBoard.c Utilities.c
clean:
	rm -f v1 v2
