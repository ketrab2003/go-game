#pragma once
#define max(a, b) (a > b ? a : b)


static const int BOARD_VIEW_SIZE = 7;

static const int LEGEND_WIDTH = 50;
static const int LEGEND_HEIGHT = 9;

static const int TERMINAL_WIDTH = 1 + BOARD_VIEW_SIZE + LEGEND_WIDTH;
static const int TERMINAL_HEIGHT = max(BOARD_VIEW_SIZE, LEGEND_HEIGHT);

static const char *INSTRUCTIONS = "Instructions:\n"
                                  "arrows - move cursor\n"
                                  "n - start a new game\n"
                                  "i - place stone\n"
                                  "enter - confirm\n"
                                  "esc - cancel action\n"
                                  "s - save game\n"
                                  "l - load game\n"
                                  "q - quit\n";

static const char *PERSONAL_DATA = "Bartlomiej Krawisz, 193319";

static const char *DONE_LIST = "Done stuff:\n"
                               "a,b,...";

static const char *HANDICAP_MESSAGE = "handicap mode";
static const char *SUICIDE_MESSAGE = "move is suicidal";
static const char *OCCUPIED_MESSAGE = "place is occupied";
static const char *ALREADY_PLACED_MESSAGE = "already placed";
static const char *KO_MESSAGE = "KO rule";

static const char BORDER_SIGN = '#';
static const unsigned char SIGN_EMPTY = '.';
static const unsigned char SIGN_BLACK_STONE = 'b';
static const unsigned char SIGN_WHITE_STONE = 'w';

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