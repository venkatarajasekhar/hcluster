CC=g++
DEBUG=-g

CFLAGS= -Wall -c -std=gnu++11 ${DEBUG}
OBJS=tools.o clust.o bitset_util.o distance.o fhash.o

all: hclustering

hclustering: ${OBJS}
	${CC} ${OBJS} -o hclustering

clust.o: clust.cpp tools.h
	${CC} ${CFLAGS} clust.cpp

tools.o: tools.cpp tools.h
	${CC} ${CFLAGS} tools.cpp

bitset_util.o: bitset_util.cpp bitset_util.h
	${CC} ${CFLAGS} bitset_util.cpp

distance.o: distance.cpp distance.h
	${CC} ${CFLAGS} distance.cpp

fhash.o: fhash.cpp fhash.h
	${CC} ${CFLAGS} fhash.cpp

clean:
	rm -rf *.o hclustering