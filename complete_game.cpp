#include "complete_game.h"

#include "conio2.h"
#include "constants.h"
#include "go_game.h"
#include "gui_utils.h"
#include "user_input.h"
#include <cstdio>
#include <cstring>

void CompleteGame::setGameStatusMessage(MoveResult result) {
  char message[20];
	switch(result) {
		case done:
			strcpy(message, "");
    break;
		case occupied:
			strcpy(message, OCCUPIED_MESSAGE);
    break;
		case suicidal:
			strcpy(message, SUICIDE_MESSAGE);
    break;
		case already_placed:
			strcpy(message, ALREADY_PLACED_MESSAGE);
    break;
		case ko:
			strcpy(message, KO_MESSAGE);
    break;
		case handicap:
			strcpy(message, HANDICAP_MESSAGE);
    break;
	}
  strcpy(gameStatusMessage, message);
}

void CompleteGame::clearGameStatusMessage() {
  strcpy(gameStatusMessage, "");
}

int CompleteGame::getBoardviewSize() const {
  const int buffer_limit = min(mainBuffer.getWidth() - LEGEND_WIDTH - 2, mainBuffer.getHeight() - 2);
  return min(game.getBoardSize() + 2, buffer_limit);
}

void CompleteGame::drawBoard() {
  if(boardBuffer.getWidth() != getBoardviewSize()) {
    boardBuffer = Canvas(getBoardviewSize(), getBoardviewSize());
  }
	boardBuffer.clear();

  const int offset_x = cursor.getViewboxPositionX();
  const int offset_y = cursor.getViewboxPositionY();

	for(int x=0; x<getBoardviewSize(); ++x) {
		for(int y=0; y<getBoardviewSize(); ++y) {
			BoardSpace space = game.getSpace(offset_x + x, offset_y + y);
			boardBuffer.setPixel(x, y, getBoardSign(space));
			if(game.getChosenX() >= 0 && offset_x + x == game.getChosenX() && offset_y + y == game.getChosenY()) {
				boardBuffer.setPixelBackground(x, y, YELLOW);
				boardBuffer.setPixelForeground(x, y, BLACK);
			}
		}
	}

	boardBuffer.setPixelBackground(cursor.getLocalX(), cursor.getLocalY(), RED);
}

Pixel CompleteGame::getBoardSign(BoardSpace space) const {
	switch (space.state) {
		case blackStone:
			return {SIGN_BLACK_STONE, WHITE, BLACK};
		case whiteStone:
			return {SIGN_WHITE_STONE, BLACK, WHITE};
		case edge:
			return {BORDER_SIGN};
		case empty:
		default:
			return {SIGN_EMPTY, BLACK, DARKGRAY};
	}
}

void CompleteGame::drawLegend() {
  legendBuffer.clear();
  legendBuffer.drawText(INSTRUCTIONS, 0, 0);
  legendBuffer.drawText(PERSONAL_DATA, 23, 0);
  legendBuffer.drawText(DONE_LIST, 23, 2);

  char score_message[30];
  sprintf(score_message, "B: %d W: %d (+%.1f)", game.getScoreBlack(), game.getScoreWhite(), game.getScoreWhiteBonus());
  legendBuffer.drawText(score_message, 23, 5);
  char location_message[20];
  sprintf(location_message, "X: %d Y: %d", cursor.getX()+1, cursor.getY()+1);
  legendBuffer.drawText(location_message, 23, 6);
  legendBuffer.drawText(gameStatusMessage, 23, 7, RED);
}

void CompleteGame::drawAll() {
  if(mainBuffer.getWidth() != GUIUtils::getTerminalWidth()
  || mainBuffer.getHeight() != GUIUtils::getTerminalHeight()) {
    mainBuffer = Canvas(GUIUtils::getTerminalWidth(), GUIUtils::getTerminalHeight());
  }

  drawBoard();
  drawLegend();

  mainBuffer.clear();
  const Canvas canvases[] = {legendBuffer, spacer, boardBuffer};
  int offset = 0;
  for(const auto canvas: canvases) {
    mainBuffer.drawCanvas(canvas, offset, 0);
    offset += canvas.getWidth();
  }
}

void CompleteGame::printGame() {
  drawAll();
  mainBuffer.print();
}

void CompleteGame::showAlert(const char* alert_message) {
  mainBuffer.clear();
  mainBuffer.drawText(alert_message, 0, 0);
  mainBuffer.print();
  UserInput::getKey();
}

int CompleteGame::getNumberFromUser(const int maxLimit) {
  mainBuffer.clear();
  const int xEndPos = mainBuffer.drawText("Provide board size: ", 0, 0);
  mainBuffer.print();
  gotoxy(xEndPos + 1, 1);
  return UserInput::getNumber(2, maxLimit);
}

void CompleteGame::getFilenameFromUser(char *dest, const int lengthLimit, const char* header_message) {
  mainBuffer.clear();
  mainBuffer.drawText(header_message, 0, 0);
  const int xEndPos = mainBuffer.drawText("Provide filename: ", 0, 1);
  mainBuffer.print();
  gotoxy(xEndPos + 1, 2);
  return UserInput::getFilename(dest, lengthLimit);
}

void CompleteGame::createNewGame() {
  const int board_size = getNumberFromUser(MAX_GAMEBOARD_SIZE);
  game = GoGame(board_size);
  cursor = Cursor(game.getBoardSize(), game.getBoardSize(), getBoardviewSize(), getBoardviewSize());
  clearGameStatusMessage();
}

void CompleteGame::saveGame() {
  char filename[MAX_FILENAME_LENGTH + 1];
  getFilenameFromUser(filename, MAX_FILENAME_LENGTH, "### SAVING GAME ###");
  FILE *savefile = fopen(filename, "w+");
  game.save(savefile);
  fclose(savefile);
  showAlert("Saved game!");
}

void CompleteGame::loadGame() {
  char filename[MAX_FILENAME_LENGTH + 1];
  getFilenameFromUser(filename, MAX_FILENAME_LENGTH, "### LOADING GaME ###");
  FILE *loadfile = fopen(filename, "r");
  if(loadfile == NULL) {
    showAlert("Failed to load!");
    return;
  }
  game.load(loadfile);
  cursor = Cursor(game.getBoardSize(), game.getBoardSize(), getBoardviewSize(), getBoardviewSize());
  fclose(loadfile);
}

void CompleteGame::gameInit() {
  GUIUtils::initializeGui(PERSONAL_DATA);
  createNewGame();
  printGame();
}

void CompleteGame::gameLoop() {
  int input_key = UserInput::getKey();
  switch(input_key) {
    case KEY_QUIT:
      gameIsRunning = false;
    break;
    case KEY_UP:
      cursor.moveRelative(0, -1);
    break;
    case KEY_DOWN:
      cursor.moveRelative(0, 1);
    break;
    case KEY_LEFT:
      cursor.moveRelative(-1, 0);
    break;
    case KEY_RIGHT:
      cursor.moveRelative(1, 0);
    break;
    case KEY_PLACE_STONE:	{
      const MoveResult result = game.placeStone(cursor.getX(), cursor.getY());
      setGameStatusMessage(result);
    }	break;
    case KEY_CONFIRM:
      game.confirmPlacement();
      clearGameStatusMessage();
    break;
    case KEY_CANCEL:
      game.cancelPlacement();
      clearGameStatusMessage();
    break;
    case KEY_NEW_GAME:
      createNewGame();
    break;
    case KEY_SAVE:
      saveGame();
    break;
    case KEY_LOAD:
      loadGame();
    break;
    case KEY_FINISH_GAME:
      game.finishGame();
    break;
  }
  printGame();
}

void CompleteGame::gameExit() {
  GUIUtils::deinitializeGui();
}

CompleteGame::CompleteGame()
: game(0),
  cursor(0, 0, 0, 0),
  mainBuffer(GUIUtils::getTerminalWidth(), GUIUtils::getTerminalHeight()),
  boardBuffer(getBoardviewSize(), getBoardviewSize()),
  legendBuffer(LEGEND_WIDTH, LEGEND_HEIGHT),
  spacer(1, GUIUtils::getTerminalHeight()) {
  gameIsRunning = true;
}

void CompleteGame::start() {
  gameInit();
  while(gameIsRunning) {
    gameLoop();
  }
  gameExit();
}