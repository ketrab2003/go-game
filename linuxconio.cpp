#include "linuxconio.h"

#include <cstdio>
#include <termios.h>
#include <unistd.h>

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

int getch(void)
{
  char buf = 0;
  struct termios old = {0};
  fflush(stdout);
  if(tcgetattr(0, &old) < 0)
      perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if(tcsetattr(0, TCSANOW, &old) < 0)
      perror("tcsetattr ICANON");
  if(read(0, &buf, 1) < 0)
      perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if(tcsetattr(0, TCSADRAIN, &old) < 0)
      perror("tcsetattr ~ICANON");
  printf(" 0x%x ", buf);
  return buf;
}

int putch(int c) {
  return printf("%lc", c);
}

void settitle(const char *title) {

}