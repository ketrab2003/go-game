#include "user_input.h"

#include "conio2.h"
#include "constants.h"
#include "go_game.h"

#define max(a, b) (a > b ? a : b)

Cursor::Cursor(const int xLimit, const int yLimit, const int viewboxWidth, const int viewboxHeight) {
  _xLimit = xLimit;
  _yLimit = yLimit;
  _viewboxWidth = viewboxWidth;
  _viewboxHeight = viewboxHeight;
  _x = 0;
  _y = 0;
  _viewboxPositionX = -1;
  _viewboxPositionY = -1;
}

void Cursor::setX(const int x) {
  if(x < 0) {
    _x = 0;
    _viewboxPositionX = -1;
    return;
  }
  if(x >= _xLimit) {
    _x = _xLimit - 1;
    if(_viewboxWidth - 1 <= getLocalX()) {
      _viewboxPositionX = _x - _viewboxWidth + 2;
    }
    return;
  }
  _x = x;

  if(_x < _viewboxPositionX) {
    _viewboxPositionX = _x;
  }
  if(_viewboxPositionX + _viewboxWidth - 1 < _x) {
    _viewboxPositionX = _x - _viewboxWidth + 1;
  }
}

int Cursor::getX() const {
  return _x;
}

void Cursor::setY(const int y) {
  if(y < 0) {
    _y = 0;
    _viewboxPositionY = -1;
    return;
  }
  if(y >= _yLimit) {
    _y = _yLimit - 1;
    if(_viewboxHeight - 1 <= getLocalY()) {
      _viewboxPositionY = _y - _viewboxHeight + 2;
    }
    return;
  }
  _y = y;

  if(_y < _viewboxPositionY) {
    _viewboxPositionY = _y;
  }
  if(_viewboxPositionY + _viewboxHeight - 1 < _y) {
    _viewboxPositionY = _y - _viewboxHeight + 1;
  }
}

int Cursor::getY() const {
  return _y;
}

int Cursor::getViewboxPositionX() const {
  return _viewboxPositionX;
}

int Cursor::getViewboxPositionY() const {
  return _viewboxPositionY;
}

int Cursor::getLocalX() const {
  return _x - _viewboxPositionX;
}

int Cursor::getLocalY() const {
  return _y - _viewboxPositionY;
}

void Cursor::setCoords(const int x, const int y) {
  setX(x);
  setY(y);
}

void Cursor::moveRelative(const int x, const int y) {
  setX(getX() + x);
  setY(getY() + y);
}


int UserInput::getKey() {
	int input = getch();
	if(input == 0) {
		return getch() + 0x100;
	}
	return input;
}

int UserInput::getNumber(const int maxLimit) {
  _setcursortype(_NORMALCURSOR);
  int readValue = 0;
  int position = 0;

  while(true) {
    int input_key = getKey();

    if(input_key == KEY_BACKSPACE && position > 0) {
      gotoxy(wherex()-1, wherey());
      putch(' ');
      gotoxy(wherex()-1, wherey());
      readValue /= 10;
      position--;
    }

    if('0' <= input_key && input_key <= '9') {
      readValue *= 10;
      readValue += input_key - '0';
      if(readValue <= maxLimit) {
        putch(input_key);
        position++;
      } else {
        readValue /= 10;
      }
    } else
    if(input_key == KEY_CONFIRM) {
      break;
    }
  }

  _setcursortype(_NOCURSOR);
  return readValue;
}

void UserInput::getFilename(char *dest, const int lengthLimit) {
  _setcursortype(_NORMALCURSOR);
  int position = 0;

  while(true) {
    int input_key = getKey();

    if(input_key == KEY_BACKSPACE && position > 0) {
      gotoxy(wherex()-1, wherey());
      putch(' ');
      gotoxy(wherex()-1, wherey());
      position--;
    }

    if(position >= lengthLimit) {
      continue;
    }

    if(('a' <= input_key && input_key <= 'z')
    || ('A' <= input_key && input_key <= 'Z')
    || ('0' <= input_key && input_key <= '9')
    || (input_key == '.')) {
      dest[position] = input_key;
      position++;
      putch(input_key);
    } else
    if(input_key == KEY_CONFIRM) {
      break;
    }
  }

  dest[position] = '\0';
  _setcursortype(_NOCURSOR);
}