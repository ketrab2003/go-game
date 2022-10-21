#include "gui_utils.h"

#include <cstdlib>

void Pixel::print() {
  textcolor(color);
  textbackground(background_color);
  putch(sign);
}

const int Canvas::_getIndexInBuffer(const int x, const int y) {
  if(x < 0 || x >= getWidth()) {
    return -1;
  }
  if(y < 0 || y >= getHeight()) {
    return -1;
  }
  return y*getWidth() + x;
}

const int Canvas::_getBufferSize() {
  return getWidth()*getHeight();
}

void Canvas::_setPixel(const int index, const Pixel pixel) {
  if(index < 0 || index >= _getBufferSize()) {
    return;
  }
  _buffer[index] = pixel;
}

Canvas::Canvas(const int width, const int height) {
  _width = width;
  _height = height;
  _buffer = (Pixel*)malloc(sizeof(Pixel) * width * height);
  clear();
}

const int Canvas::getWidth() {
  return _width;
}

const int Canvas::getHeight() {
  return _height;
}

const Pixel Canvas::getPixel(const int x, const int y) {
  const int index = _getIndexInBuffer(x, y);
  if(index == -1) {
    return Pixel{};
  }
  return _buffer[index];
}

void Canvas::setPixel(const int x, const int y, const Pixel pixel) {
  _setPixel(_getIndexInBuffer(x, y), pixel);
}

void Canvas::setPixel(const int x, const int y, const char sign, const int color, const int background_color) {
  setPixel(x, y, Pixel{sign, color, background_color});
}

void Canvas::clear() {
  fill(BLACK);
}

void Canvas::fill(const int background_color) {
  for(int i=0; i<_getBufferSize(); ++i) {
      _setPixel(i, Pixel{' ', WHITE, background_color});
  }
}

void Canvas::drawText(const char *str, const int x, const int y, const int color, const int background_color) {
  int iy = 0;
  int ix = 0;
  for(int i=0; str[i] != '\0'; ++i) {
    if(str[i] == '\n') {
      iy++;
      ix = 0;
      continue;
    }
    setPixel(x+ix, y+iy, Pixel{str[i], color, background_color});
    ix++;
  }
}

void Canvas::drawRect(const int x, const int y, const int width, const int height, const Pixel border, const Pixel filling) {
  for(int ix=0; ix<width; ++ix) {
    for(int iy=0; iy<height; ++iy) {
      if(ix == 0 || iy == 0 || ix == width-1 || iy == height-1) {
        setPixel(x+ix, y+iy, border);
      } else {
        setPixel(x+ix, y+iy, filling);
      }
    }
  }
}

void Canvas::drawFilledRect(const int x, const int y, const int width, const int height, const Pixel filling) {
  drawRect(x, y, width, height, filling, filling);
}

void Canvas::drawCanvas(Canvas& canvas, const int& x, const int& y) {
  for(int ix=0; ix<canvas.getWidth(); ix++) {
    for(int iy=0; iy<canvas.getHeight(); iy++) {
      setPixel(x+ix, y+iy, canvas.getPixel(ix, iy));
    }
  }
}

void Canvas::print(const int x, const int y) {
  gotoxy(x, y);
  for(int iy=0; iy<_height; ++iy) {
    for(int ix=0; ix<_width; ++ix) {
      _buffer[_getIndexInBuffer(ix, iy)].print();
    }
    putch('\n');
  }
}

Canvas::~Canvas() {
  free(_buffer);
}