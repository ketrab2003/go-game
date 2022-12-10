#pragma once

#include "go_game.h"
#include "gui_utils.h"
#include "user_input.h"

class CompleteGame {
	GoGame game;
	Cursor cursor;
	Canvas mainBuffer;
	Canvas boardBuffer;
	Canvas legendBuffer;
	const char* gameStatusMessage;
	bool gameIsRunning;

	void setGameStatusMessage(MoveResult result);
	void clearGameStatusMessage();

	// drawing logic
	int getBoardviewWidth() const;	// width of piece of board that can be printed on screen
	int getBoardviewHeight() const;	// height of piece of board that can be printed on screen
	void drawBoard();
	static Pixel getBoardSign(const BoardSpace& space);	// helper function returning Pixel representation of provided space on board
	void drawLegend();
	void drawAll();		// calls all other draw functions and draws all pieces on mainBuffer
	void printGame();

	// user dialog logic
	void showAlert(const char* alert_message);	// show message that user can dismiss by clicking any button
	void getFilenameFromUser(char* dest, const char* header_message);
	int getBoardSizeFromUser();	// get board size in number form
	int askBoardSize();		// give user choice with default board sizes, or to provide their own
	void showGameResult();

	void createNewGame();
	void saveGame();
	void loadGame();

	void gameInit();	// should be run exactly once at the beggining of the program
	void gameLoop();
	void gameExit();	// should be run exactly once at the end of the program

public:
	CompleteGame();
	void start();
};