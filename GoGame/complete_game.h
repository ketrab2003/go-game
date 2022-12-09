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
	char gameStatusMessage[20];
	bool gameIsRunning;

	void setGameStatusMessage(MoveResult result);
	void clearGameStatusMessage();

	// drawing logic
	int getBoardviewSize() const;	// size of piece of board that can be printed on screen
	void drawBoard();
	static Pixel getBoardSign(const BoardSpace& space);	// helper function returning Pixel representation of provided space on board
	void drawLegend();
	void drawAll();		// calls all other draw functions and draws all pieces on mainBuffer
	void printGame();   // print game onto user's screen

	// user dialog logic
	void showAlert(const char* alert_message);	// show message that user can dismiss by clicking any button
	void getFilenameFromUser(char* dest, const char* header_message);
	int getBoardSizeFromUser();	// get board size in number form
	int askBoardSize();		// give user choice with default board sizes, or to provide their own
	void showGameResult();

	void createNewGame();
	void saveGame();
	void loadGame();

	void gameInit();
	void gameLoop();
	void gameExit();

public:
	CompleteGame();
	void start();
};