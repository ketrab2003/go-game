#include "gui_utils.h"
#include "conio2.h"

#include <cstdlib>

int GUIUtils::getTerminalWidth() {
  text_info info;
  gettextinfo(&info);
  return info.screenwidth;
}

int GUIUtils::getTerminalHeight() {
  text_info info;
  gettextinfo(&info);
  return info.screenheight - 1;
}

void GUIUtils::initializeGui(const char *window_title) {
  settitle(window_title);
	clrscr();
	_setcursortype(_NOCURSOR);
}

void GUIUtils::deinitializeGui() {
	gotoxy(1, 1);
  textattr(BLACK * 16 + WHITE);
  clrscr();
	_setcursortype(_NORMALCURSOR);
}



unsigned char Pixel::getAttr() const {
  return background_color * 16 + foreground_color;
}

bool Pixel::alreadyPrinted() const {
  return last_printed[0] == sign
      && last_printed[1] == getAttr();
}

void Pixel::print() {
  textattr(getAttr());
  putch(sign);
  last_printed[0] = sign;
  last_printed[1] = getAttr();
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
  _buffer[x][y].background_color = pixel.background_color;
  _buffer[x][y].foreground_color = pixel.foreground_color;
  _buffer[x][y].sign = pixel.sign;
}

void Canvas::setPixel(const int x, const int y, const unsigned char sign, const unsigned char foreground_color, const unsigned char background_color) {
  setPixel(x, y, Pixel{sign, foreground_color, background_color});
}

void Canvas::setPixelBackground(const int x, const int y, const unsigned char background_color) {
  if(!_validCoords(x, y)) {
    return;
  }
  if(_buffer[x][y].background_color == background_color) {
    return;
  }
  _buffer[x][y].background_color = background_color;
}

void Canvas::setPixelForeground(const int x, const int y, const unsigned char foreground_color) {
  if(!_validCoords(x, y)) {
    return;
  }
  if(_buffer[x][y].foreground_color == foreground_color) {
    return;
  }
  _buffer[x][y].foreground_color = foreground_color;
}

void Canvas::setPixelSign(const int x, const int y, const unsigned char sign) {
  if(!_validCoords(x, y)) {
    return;
  }
  if(_buffer[x][y].sign == sign) {
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

int Canvas::drawText(const char *str, const int x, const int y, const int foreground_color, const int background_color) {
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

  return x+ix;
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

void Canvas::print(const int x, const int y) {
  bool must_jump = true;
  for(int iy=0; iy<getHeight(); ++iy) {
    for(int ix=0; ix<getWidth(); ++ix) {
      const Pixel pixel = getPixel(ix, iy);
      if(pixel.alreadyPrinted()) {
        must_jump = true;
        continue;
      }
      if(must_jump) {
        gotoxy(x+ix, y+iy);
        must_jump = false;
      }
      _buffer[ix][iy].print();
    }
    must_jump = true;
  }
}