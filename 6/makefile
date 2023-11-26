CC=cc
CFLAGS=-Wall -Wextra -g -std=c99

png2hex: png2hex.c
	$(CC) $(CFLAGS) -Ilibspng/spng -lz -lm libspng/spng/spng.c png2hex.c -o png2hex

task: task.c
	$(CC) $(CFLAGS) task.c -o task

