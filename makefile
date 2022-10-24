.PHONY = all clean

CC = winegcc

LINKERFLAG = -lstdc++

all: main.exe

main.exe: main.o conio2.o gui_utils.o
	${CC} ${LINKERFLAG} main.o conio2.o gui_utils.o -o main

main.o: main.cpp constants.h
	${CC} -c main.cpp

conio2.o: conio2.cpp
	${CC} -c conio2.cpp

gui_utils.o: gui_utils.cpp
	${CC} -c gui_utils.cpp

clean:
	@echo "Cleaning up..."
	rm -rvf *.o
