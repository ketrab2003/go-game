#pragma once

#include "conio2.h"
#include "array2D.hpp"

struct GUIUtils {
	static int getTerminalWidth();
	static int getTerminalHeight();
	static void initializeGui(const char* window_title);	// should by run exactly once, at the beggining of the program
	static void deinitializeGui();		// should be run exactly once, at the end of the program
};

struct Pixel {
	unsigned char sign = ' ';
	unsigned char foreground_color = WHITE;
	unsigned char background_color = BLACK;

	unsigned char last_printed[2] = { 0, 0 };		// used by alreadyPrinted() method

	unsigned char getAttr() const;
	bool alreadyPrinted() const;	// return whether pixel has changed since last printing, used to optimize printing time in slower terminals by not printing already printed characters
	void print();
};

class Canvas {
	Array2D<Pixel> _buffer;
	bool _validCoords(const int x, const int y) const;

public:
	Canvas(const int width, const int height);

	int getWidth() const;
	int getHeight() const;
	Pixel getPixel(const int x, const int y) const;

	// various methods to modify single pixel in canvas
	void setPixel(const int x, const int y, const Pixel& pixel = {});
	void setPixel(const int x, const int y, const unsigned char sign, const unsigned char foreground_color = WHITE, const unsigned char background_color = BLACK);
	void setPixelBackground(const int x, const int y, const unsigned char background_color);
	void setPixelForeground(const int x, const int y, const unsigned char foreground_color);
	void setPixelSign(const int x, const int y, const unsigned char sign);

	void clear();   // mark all pixels default (empty and black)
	void fill(const Pixel& pixel);  // mark all pixels empty and colored with provided background_color
	int drawText(const char* str, const int x, const int y, const int foreground_color = WHITE, const int background_color = BLACK);    // write text with provided (x,y) as upper-left corner, \n is supported for newline, return x position at the end of drawn text
	void drawRect(const int x, const int y, const int width, const int height, const Pixel& border = {}, const Pixel& filling = {});  // draw rectangle with provided (x,y) as upper-left corner
	void drawCanvas(const Canvas& canvas, const int x, const int y);

	void print();	// print canvas to the terminal, always starting from upper-left corner of the terminal
};