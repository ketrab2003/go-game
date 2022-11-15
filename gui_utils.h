#pragma once

#include "array2D.h"
#include "conio2.h"

struct Pixel {
  unsigned char sign = ' ';
	int foreground_color = WHITE;
	int background_color = BLACK;

  void print() const;
};

class Canvas {
  Array2D<Pixel> _buffer;
  bool _validCoords(const int x, const int y) const;
  Pixel& _accessPixel(const int x, const int y);

public:
  Canvas(const int width, const int height);

  int getWidth() const;
  int getHeight() const;
  Pixel getPixel(const int x, const int y) const;

  void setPixel(const int x, const int y, const Pixel pixel={});
  void setPixel(const int x, const int y, const unsigned char sign, const int foreground_color=WHITE, const int background_color=BLACK);
  void setPixelBackground(const int x, const int y, const int background_color);
  void setPixelForeground(const int x, const int y, const int foreground_color);
  void setPixelSign(const int x, const int y, const unsigned char sign);

  void clear();   // mark all pixels default (empty and black)
  void fill(const Pixel pixel);  // mark all pixels empty and colored with provided background_color
  void drawText(const char *str, const int x, const int y, const int foreground_color=WHITE, const int background_color=BLACK);    // write text with provided (x,y) as upper-left corner, \n is supported for newline
  void drawRect(const int x, const int y, const int width, const int height, const Pixel border={}, const Pixel filling={});  // draw rectangle with provided (x,y) as upper-left corner
  void drawFilledRect(const int x, const int y, const int width, const int height, const Pixel filling={});
  void drawCanvas(const Canvas& canvas, const int& x, const int& y);

  void print(const int x=1, const int y=1) const;
};