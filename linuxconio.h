#pragma once

#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED		4
#define MAGENTA		5
#define BROWN		6
#define LIGHTGRAY	7
#define DARKGRAY	8
#define LIGHTBLUE	9
#define LIGHTGREEN	10
#define LIGHTCYAN	11
#define LIGHTRED	12
#define LIGHTMAGENTA	13
#define YELLOW		14
#define WHITE		15

#define _NOCURSOR	0
#define _SOLIDCURSOR	1
#define _NORMALCURSOR	2

#ifndef EOF
#define EOF		-1
#endif

#define LASTMODE	-1
#define C80		3
#define C4350		8

void gotoxy(int x, int y);		// move cursor to the position (x,y)
int wherex(void);			// return x coordinate of the cursor
int wherey(void);			// return y coordinate of the cursor 
void _setcursortype(int cur_t);		// set cursor type: _NOCURSOR, _SOLIDCURSOR or _NORMALCURSOR

void textattr(int newattr);		// set foreground and background color at once (newattr = background * 16 + foreground)
void textbackground(int newcolor);	// set background color of new characters
void textcolor(int newcolor);		// set foreground color of new characters

void clrscr(void);			// clear entire screen

int getch(void);			// wait for keyboard input and return the read character

int putch(int c);			// write single character to screen at current cursor position

void settitle(const char *title);