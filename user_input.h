#pragma once

class Cursor {
	int _x;
	int _y;
	int _xLimit;
	int _yLimit;

  int _viewboxPositionX;
  int _viewboxPositionY;
  int _viewboxWidth;
  int _viewboxHeight;

public:
	Cursor(const int xLimit, const int yLimit, const int xViewboxWidth, const int xViewboxHeight);

	void setX(const int x);
	int getX() const;
	void setY(const int y);
	int getY() const;

  int getViewboxPositionX() const;
  int getViewboxPositionY() const;
  int getLocalX() const;
  int getLocalY() const;

  void setCoords(const int x, const int y);
	void moveRelative(const int x, const int y);
};

class UserInput {
public:
  static int getKey();
  static int getNumber(const int maxLimit);
  static void getFilename(char *dest, const int lengthLimit);
};