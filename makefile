.PHONY = all clean

CC = g++

LINKERFLAG = -lstdc++

all: main.exe
test: tests.out

main.exe: main.o conio2.o gui_utils.o go_game.o user_input.o complete_game.o
	${CC} ${LINKERFLAG} main.o conio2.o gui_utils.o go_game.o user_input.o complete_game.o -o main

tests.out: go_game_tests.cpp go_game.o
	${CC} go_game_tests.cpp go_game.o -o tests.out && ./tests.out

main.o: main.cpp constants.h
	${CC} -c main.cpp

conio2.o: conio2.cpp
	${CC} -c conio2.cpp

gui_utils.o: gui_utils.cpp
	${CC} -c gui_utils.cpp

go_game.o: go_game.cpp
	${CC} -c go_game.cpp

user_input.o: user_input.cpp
	${CC} -c user_input.cpp

complete_game.o: complete_game.cpp
	${CC} -c complete_game.cpp

clean:
	@echo "Cleaning up..."
	rm -rvf *.o
