#pragma once
#include "conio2.h"

struct Pixel {
  char sign = ' ';
	int color = WHITE;
	int background_color = BLACK;

  void print();
};

class Canvas {
private:
  Pixel *_buffer;
	int _width, _height;

  const int _getIndexInBuffer(const int x, const int y);
  const int _getBufferSize();
  void _setPixel(const int index, const Pixel pixel);

public:
  Canvas(const int width, const int height);
  const int getWidth();
  const int getHeight();
  const Pixel getPixel(const int x, const int y);
  void setPixel(const int x, const int y, const Pixel pixel={});
  void setPixel(const int x, const int y, const char sign, const int color=WHITE, const int background_color=BLACK);
  void clear();
  void fill(const int background_color);
  void drawText(const char *str, const int x, const int y, const int color=WHITE, const int background_color=BLACK);
  void drawRect(const int x, const int y, const int width, const int height, const Pixel border={}, const Pixel filling={});
  void drawFilledRect(const int x, const int y, const int width, const int height, const Pixel filling={});
  void drawCanvas(Canvas& canvas, const int& x, const int& y);
  void print(const int x=0, const int y=0);
  ~Canvas();
};