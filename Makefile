#Alexander Nguyen
# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
##LIB = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
all: lab1

lab1: lab1.cpp
	g++ $(CFLAGS) lab1.cpp log.cpp libggfonts.a  -Wall -Wextra $(LFLAGS) -olab1 

clean:
	rm -f lab1
	rm -f *.o

