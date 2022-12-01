#include "linuxconio.h"
#include "linuxconio.h"
#include <cstdio>

int cursor_x=1, cursor_y=1;

void gotoxy(int x, int y) {
  cursor_x = x;
  cursor_y = y;
  printf("\e[%d;%dH", y, x);
}

int wherex() {
  return cursor_x;
}

int wherey() {
  return cursor_y;
}

void _setcursortype(int cur_t) {
  switch(cur_t) {
    case _NOCURSOR:
      printf("\e[?25l");
    break;
    case _NORMALCURSOR:
      printf("\e[?25h");
    break;
  }
}

void textattr(int newattr) {
  textbackground(newattr >> 4);
  textcolor(newattr % 16);
}

void textbackground(int newcolor) {
  printf("\e[48;5;%dm", newcolor+40);
}

void textcolor(int newcolor) {
  printf("\e[38;5;%dm", newcolor+30);
}

void clrscr() {
  printf("\e[2J");
}

int getch_zero_buffer = 0;

int getch() {
  if(getch_zero_buffer) {
    int temp = getch_zero_buffer;
    getch_zero_buffer = 0;
    return temp;
  }
  getch_zero_buffer = getchar();

}

int putch(int c) {
  return printf("%lc", c);
}

void settitle(const char *title) {

}