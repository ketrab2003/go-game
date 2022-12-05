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
  const Canvas spacer;
  char gameStatusMessage[20];
  bool gameIsRunning;

	void setGameStatusMessage(MoveResult result);
	void clearGameStatusMessage();

  int getBoardviewSize() const;

  void drawBoard();
  Pixel getBoardSign(BoardSpace space) const;
  void drawLegend();
  void drawAll();
  void printGame();   // print game onto user's screen

  void showAlert(const char* alert_message);
  int getNumberFromUser(const int maxLimit);
  void getFilenameFromUser(char *dest, const int lengthLimit, const char* header_message);

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