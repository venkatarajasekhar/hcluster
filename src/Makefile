CC=g++
DEBUG=-g

CFLAGS= -Wall -c ${DEBUG}
OBJS=tools.o clust.o pkg_table.o

all: hclustering

hclustering: ${OBJS}
	${CC} ${OBJS} -o hclustering

clust.o: clust.cpp tools.h
	${CC} ${CFLAGS} clust.cpp

tools.o: tools.cpp tools.h
	${CC} ${CFLAGS} tools.cpp

pkg_table.o: pkg_table.cpp pkg_table.h
	${CC} ${CFLAGS} pkg_table.cpp

clean:
	rm -rf *.o hclustering