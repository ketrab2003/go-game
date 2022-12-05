#pragma once
#include "conio2.h"
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

// data for legend
static const int LEGEND_WIDTH = 50;
static const int LEGEND_HEIGHT = 10;

static const char *INSTRUCTIONS = "Instructions:\n"
                                  "arrows - move cursor\n"
                                  "n - start a new game\n"
                                  "i - place stone\n"
                                  "enter - confirm\n"
                                  "esc - cancel action\n"
                                  "s - save game\n"
                                  "l - load game\n"
                                  "q - quit\n"
                                  "f - finish game\n";

static const char *PERSONAL_DATA = "Bartlomiej Krawisz, 193319";  // for window title and legend

static const char *DONE_LIST = "Done stuff:\n"
                               "a,b,...";

// game state messages for player
static const char *HANDICAP_MESSAGE = "handicap mode";
static const char *SUICIDE_MESSAGE = "move is suicidal";
static const char *OCCUPIED_MESSAGE = "place is occupied";
static const char *ALREADY_PLACED_MESSAGE = "already placed";
static const char *KO_MESSAGE = "ko rule";

// other messages
static const char *BOARD_SIZE_QUERY = "Provide board size: ";
static const char *FILENAME_QUERY = "Provide filename: ";
static const char *SAVING_HEADER = "### SAVING GAME ###";
static const char *LOADING_HEADER = "### LOADING GAME ###";
static const char *SAVE_SUCCESS = "Saved game!";
static const char *LOAD_ERROR = "Failed to load!";

static const char *BLACK_WON_MESSAGE = "Black won!";
static const char *WHITE_WON_MESSAGE = "White won!";
static const char *GAME_FINISH_DIALOG_INSTRUCTIONS =
                                "Press n to play new game\n"
                                "Press ESC to cancel\n"
                                "Press q to quit\n";

// user dialog restrictions
static const int MAX_FILENAME_LENGTH = 20;
static const int MAX_GAMEBOARD_SIZE = 9999;

// border view
static const unsigned char BORDER_SIGN = '#';
static const unsigned char EMPTY_SIGN = '.';
static const unsigned char EMPTY_COLOR = WHITE;
static const unsigned char EMPTY_BACKGROUND = BLACK;
static const unsigned char BLACK_STONE_SIGN = 'b';
static const unsigned char BLACK_STONE_COLOR = WHITE;
static const unsigned char BLACK_STONE_BACKGROUND = BLACK;
static const unsigned char WHITE_STONE_SIGN = 'w';
static const unsigned char WHITE_STONE_COLOR = BLACK;
static const unsigned char WHITE_STONE_BACKGROUND = WHITE;

// control keys
static const int KEY_UP = 0x148;
static const int KEY_DOWN = 0x150;
static const int KEY_LEFT = 0x14b;
static const int KEY_RIGHT = 0x14d;
static const int KEY_QUIT = 'q';
static const int KEY_NEW_GAME = 'n';
static const int KEY_CONFIRM = 0xd;
static const int KEY_CANCEL = 0x1b;
static const int KEY_PLACE_STONE = 'i';
static const int KEY_SAVE = 's';
static const int KEY_LOAD = 'l';
static const int KEY_FINISH_GAME = 'f';
static const int KEY_BACKSPACE = 0x08;