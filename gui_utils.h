#pragma once
#include "conio2.h"

struct Pixel {
  char sign = ' ';
	int color = WHITE;
	int background_color = BLACK;

  void print() const;
};

class Canvas {
  Pixel *_buffer;     // internal state of all pixels in the canvas, should be accessed only by constructor, destructor and personal getter/setter

  int _getIndexInBuffer(const int x, const int y) const;
  int _getBufferSize() const;
  void _setPixel(const int index, const Pixel pixel);   // private because we don't want user to deal with internal implementation

public:
  const int width, height;  // dimensions of canvas, set on object's construction and later unchangeable

  Canvas(const int width, const int height);

  const Pixel getPixel(const int x, const int y) const;

  void setPixel(const int x, const int y, const Pixel pixel={});
  void setPixel(const int x, const int y, const char sign, const int color=WHITE, const int background_color=BLACK);

  void clear();   // mark all pixels empty and black
  void fill(const int background_color);  // mark all pixels empty and colored with provided background_color
  void drawText(const char *str, const int x, const int y, const int color=WHITE, const int background_color=BLACK);    // write text with provided (x,y) as upper-left corner, \n is supported for newline
  void drawRect(const int x, const int y, const int width, const int height, const Pixel border={}, const Pixel filling={});  // draw rectangle with provided (x,y) as upper-left corner
  void drawFilledRect(const int x, const int y, const int width, const int height, const Pixel filling={});
  void drawCanvas(const Canvas& canvas, const int& x, const int& y);

  void print(const int x=1, const int y=1) const;

  ~Canvas();
};