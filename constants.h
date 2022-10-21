#pragma once
#define max(a, b) (a > b ? a : b)


static const int BOARD_VIEW_SIZE = 7;

static const int LEGEND_WIDTH = 52;
static const int LEGEND_HEIGHT = 7;

static const int TERMINAL_WIDTH = 3 + BOARD_VIEW_SIZE + LEGEND_WIDTH;
static const int TERMINAL_HEIGHT = 3 + max(BOARD_VIEW_SIZE, LEGEND_HEIGHT);

static const char* INSTRUCTIONS = "Instructions:\n"
                                  "arrows - move cursor\n"
                                  "q - quit\n"
                                  "n - start a new game\n";
static const char* PERSONAL_DATA = "Bartlomiej Krawisz, 193319";
static const char* DONE_LIST = "Done stuff:\n"
                               "a,b,...";

static const char BORDER_SIGN = '#';

static const int KEY_UP = 0x48;
static const int KEY_DOWN = 0x50;
static const int KEY_LEFT = 0x4b;
static const int KEY_RIGHT = 0x4d;
static const int KEY_QUIT = 'q';
static const int KEY_NEW_GAME = 'n';
static const int KEY_CONFIRM = 0xd;
static const int KEY_CANCEL = 0x1b;
static const int KEY_PLACE_STONE = 'i';
static const int KEY_SAVE = 's';
static const int KEY_LOAD = 'l';
static const int KEY_FINISH_GAME = 'f';