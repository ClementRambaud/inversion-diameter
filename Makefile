all: graph_lib.o
	gcc -Wall -march=native -Ofast -o compute_diam_I.native compute_diam_I.c graph_lib.o

dicoloration.o: graph_lib.c graph_lib.h
	gcc -Wall -march=native -Ofast -c graph_lib.c

clean:
	rm *.native
	rm *.o



