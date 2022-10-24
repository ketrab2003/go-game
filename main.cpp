#include "conio2.h"
#include "constants.h"
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

	int getX() {
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

	int getY() {
		return _y;
	}

	void moveRelative(const int x, const int y) {
		setX(getX() + x);
		setY(getY() + y);
	}
};

int main() {
#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle(PERSONAL_DATA);

	Canvas buffer(TERMINAL_WIDTH, TERMINAL_HEIGHT);
	Cursor cursor(BOARD_VIEW_SIZE, BOARD_VIEW_SIZE, 5, 5);
	int input_key;

	_setcursortype(_NOCURSOR);

	do {
		buffer.clear();
		buffer.drawRect(0, 0, buffer.width - 1, buffer.height - 1, {BORDER_SIGN});		// Draw outer border

		Canvas legend(LEGEND_WIDTH, LEGEND_HEIGHT);
		legend.drawRect(0, -1, 24, 10, {BORDER_SIGN});
		legend.drawText(INSTRUCTIONS, 2, 0);
		legend.drawRect(23, -1, 30, 3, {BORDER_SIGN});
		legend.drawText(PERSONAL_DATA, 25, 0);
		legend.drawRect(23, 1, 30, 7, {BORDER_SIGN});
		legend.drawText(DONE_LIST, 25, 2);
		buffer.drawCanvas(legend, 1 + BOARD_VIEW_SIZE, 1);

		buffer.drawFilledRect(1, 1, BOARD_VIEW_SIZE, BOARD_VIEW_SIZE, {'+', BLACK, LIGHTCYAN});
		buffer.setPixel(cursor.getX() + 1, cursor.getY() + 1, '+');

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
		}
	} while(input_key != KEY_QUIT);

	_setcursortype(_NORMALCURSOR);
	return 0;
}
