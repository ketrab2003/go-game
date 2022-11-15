#include "conio2.h"
#include "constants.h"
#include <cstring>
#include "go_game.h"
#include "gui_utils.h"

int getkey() {
	int input = getch();
	if(input == 0) {
		return getch();
	}
	return input;
}

class Cursor {
private:
	int _x;
	int _y;
	int _xLimit;
	int _yLimit;
public:
	Cursor(const int xLimit, const int yLimit, int x=0, int y=0) {
		_xLimit = xLimit;
		_yLimit = yLimit;
		_x = x;
		_y = y;
	}

	void setX(const int x) {
		if(x < 0) {
			_x = 0;
			return;
		}
		if(x >= _xLimit) {
			_x = _xLimit - 1;
			return;
		}
		_x = x;
	}

	int getX() const {
		return _x;
	}

	void setY(const int y) {
		if(y < 0) {
			_y = 0;
			return;
		}
		if(y >= _yLimit) {
			_y = _yLimit - 1;
			return;
		}
		_y = y;
	}

	int getY() const {
		return _y;
	}

	void moveRelative(const int x, const int y) {
		setX(getX() + x);
		setY(getY() + y);
	}
};

Pixel getBoardSign(BoardSpace space) {
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

Canvas drawBoard(const GoGame& game, const Cursor& cursor) {
	Canvas canvas(game.getBoardSize(), game.getBoardSize());
	canvas.fill({'.'});

	for(int x=0; x<game.getBoardSize(); ++x) {
		for(int y=0; y<game.getBoardSize(); ++y) {
			BoardSpace space = game.getSpace(x, y);
			canvas.setPixel(x, y, getBoardSign(space));
			if(x == game.getChosenX() && y == game.getChosenY()) {
				canvas.setPixelBackground(x, y, YELLOW);
				canvas.setPixelForeground(x, y, BLACK);
			}
		}
	}

	canvas.setPixelBackground(cursor.getX(), cursor.getY(), RED);

	return canvas;
}

const char* pickMessage(MoveResult result) {
	switch(result) {
		case done:
			return "";
		case occupied:
			return OCCUPIED_MESSAGE;
		case suicidal:
			return SUICIDE_MESSAGE;
		case already_placed:
			return ALREADY_PLACED_MESSAGE;
		case ko:
			return KO_MESSAGE;
		case handicap:
			return HANDICAP_MESSAGE;
	}
}

int main() {
#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle(PERSONAL_DATA);
	clrscr();
	_setcursortype(_NOCURSOR);

	Canvas buffer(TERMINAL_WIDTH, TERMINAL_HEIGHT);

	int input_key;
	char location_message[20] = ""; 
	char game_message[20] = "";

	GoGame game(7);
	Cursor cursor(game.getBoardSize(), game.getBoardSize(), 5, 5);

	do {
		buffer.clear();

		// prepare legend
		Canvas legend(LEGEND_WIDTH, LEGEND_HEIGHT);
		legend.drawText(INSTRUCTIONS, 0, 0);
		legend.drawText(PERSONAL_DATA, 23, 0);
		legend.drawText(DONE_LIST, 23, 2);

		sprintf(location_message, "X: %d Y: %d", cursor.getX(), cursor.getY());
		legend.drawText(location_message, 23, 5);
		legend.drawText(game_message, 23, 6, RED);

		// prepare board
		Canvas board = drawBoard(game, cursor);

		Canvas spacer(1, TERMINAL_HEIGHT);

		// draw everything in buffer
		const Canvas canvases[] = {legend, spacer, board};
		int offset = 0;
		for(const auto canvas: canvases) {
			buffer.drawCanvas(canvas, offset, 0);
			offset += canvas.getWidth();
		}

		buffer.print();

		input_key = getkey();

		switch(input_key) {
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
				strcpy(game_message, pickMessage(result));
			}	break;
			case KEY_CONFIRM:
				game.confirmPlacement();
				strcpy(game_message, "");
			break;
			case KEY_CANCEL:
				game.cancelPlacement();
				strcpy(game_message, "");
			break;
			case KEY_NEW_GAME:
				game = GoGame(7);
				strcpy(game_message, "");
			break;
		}
	} while(input_key != KEY_QUIT);

	clrscr();
	gotoxy(1, 1);
	_setcursortype(_NORMALCURSOR);
	return 0;
}
