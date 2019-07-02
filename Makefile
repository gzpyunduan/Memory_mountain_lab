CC = gcc
CFLAGS = -Wall -O2 -D__i386__

DATAFILE=data.txt

mountain: mountain.c fcyc2.c clock.c
	$(CC) $(CFLAGS) -o mountain mountain.c fcyc2.c clock.c

run: ${DATAFILE}
	./mountain >${DATAFILE}

plot: ${DATAFILE}
	./plot.py ${DATAFILE}

clean:
	rm -f mountain *.o *~
