CC=cc
CFLAGS=-Wall -Wextra -g  -lm
LIBPNGFLAGS=-Ilibspng/spng -lz
LIBPNGSRC=libspng/spng/spng.c

png2hex: png2hex.c
	$(CC) $(CFLAGS) $(LIBPNGFLAGS) $(LIBPNGSRC) png2hex.c -o png2hex

task: task.c
	$(CC) $(CFLAGS) task.c -o task

misha: misha.c
	$(CC) $(CFLAGS) $(LIBPNGFLAGS) $(LIBPNGSRC) misha.c -o misha

clean:
	rm png2hex task misha

