#Alexander Nguyen
# cs335 hw1
# to compile your project, type make and press enter
#CFLAGS = -I ./include
##LIB = ./libggfonts.so
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
all: hw1

hw1: hw1.cpp
	g++ -I ./include hw1.cpp log.cpp libggfonts.a  -Wall -Wextra -lrt -lX11 -lGLU -lGL -pthread -lm -ohw1 

clean:
	rm -f hw1
	rm -f *.o

