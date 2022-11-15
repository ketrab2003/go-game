#include "gui_utils.h"
#include "conio2.h"

#include <cstdlib>

void Pixel::print() const {
  textcolor(foreground_color);
  textbackground(background_color);
  putch(sign);
}



bool Canvas::_validCoords(const int x, const int y) const {
  if(x < 0 || x >= getWidth()) {
    return false;
  }
  if(y < 0 || y >= getHeight()) {
    return false;
  }
  return true;
}

Canvas::Canvas(const int width, const int height)
: _buffer(width, height) {
  clear();
}

int Canvas::getWidth() const {
  return _buffer.getWidth();
}

int Canvas::getHeight() const {
  return _buffer.getHeight();
}

Pixel Canvas::getPixel(const int x, const int y) const {
  if(!_validCoords(x, y)) {
    return Pixel();
  }
  return _buffer[x][y];
}

void Canvas::setPixel(const int x, const int y, const Pixel pixel) {
  if(!_validCoords(x, y)) {
    return;
  }
  _buffer[x][y] = pixel;
}

void Canvas::setPixel(const int x, const int y, const unsigned char sign, const int foreground_color, const int background_color) {
  setPixel(x, y, Pixel{sign, foreground_color, background_color});
}

void Canvas::setPixelBackground(const int x, const int y, const int background_color) {
  if(!_validCoords(x, y)) {
    return;
  }
  _buffer[x][y].background_color = background_color;
}

void Canvas::setPixelForeground(const int x, const int y, const int foreground_color) {
  if(!_validCoords(x, y)) {
    return;
  }
  _buffer[x][y].foreground_color = foreground_color;
}

void Canvas::setPixelSign(const int x, const int y, const unsigned char sign) {
  if(!_validCoords(x, y)) {
    return;
  }
  _buffer[x][y].sign = sign;
}

void Canvas::clear() {
  fill(Pixel());
}

void Canvas::fill(const Pixel pixel) {
  for(int x=0; x<getWidth(); ++x) {
    for(int y=0; y<getHeight(); ++y) {
      setPixel(x, y, pixel);
    }
  }
}

void Canvas::drawText(const char *str, const int x, const int y, const int foreground_color, const int background_color) {
  int iy = 0;
  int ix = 0;
  for(int i=0; str[i] != '\0'; ++i) {
    if(str[i] == '\n') {
      iy++;
      ix = 0;
      continue;
    }
    setPixel(x+ix, y+iy, str[i], foreground_color, background_color);
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

void Canvas::drawCanvas(const Canvas& canvas, const int& x, const int& y) {
  for(int ix=0; ix<canvas.getWidth(); ix++) {
    for(int iy=0; iy<canvas.getHeight(); iy++) {
      setPixel(x+ix, y+iy, canvas.getPixel(ix, iy));
    }
  }
}

void Canvas::print(const int x, const int y) const {
  gotoxy(x, y);
  for(int iy=0; iy<getHeight(); ++iy) {
    for(int ix=0; ix<getWidth(); ++ix) {
      getPixel(ix, iy).print();
    }
    putch('\n');
  }
}