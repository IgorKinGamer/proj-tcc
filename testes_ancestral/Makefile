CC=g++
OPTS=-O3 -std=c++11
LIBS=-lhwloc
EXEC=tmp

all: ${EXEC}

tmp: tst_tmp/tst_tmp.cpp
	${CC} tst_tmp/tst_tmp.cpp -o ${EXEC} ${LIBS} ${OPTS} 

clear:
	rm ${EXEC}
