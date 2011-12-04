CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

all: process

processing.o : processing.cpp blockclass.cpp blockclass.h imgclasses.cpp imgclasses.h lablib.cpp lablib.h
	${CC} ${CFLAGS} -c processing.cpp

lablib.o : lablib.cpp lablib.h
	${CC} ${CFLAGS} -c lablib.cpp

imgclasses.o : imgclasses.cpp imgclasses.h lablib.cpp lablib.h
	$(CC) $(CFLAGS) -c imgclasses.cpp

blockclass.o : blockclass.cpp blockclass.h imgclasses.cpp imgclasses.h lablib.cpp lablib.h
	$(CC) $(CFLAGS) -c blockclass.cpp

process : processing.o lablib.o imgclasses.o blockclass.o
	${CC} ${CFLAGS} imgclasses.o lablib.o processing.o blockclass.o ${LDFLAGS} -o process

clean:
	rm -rf *o process

