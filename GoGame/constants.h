#pragma once
#include "conio2.h"
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

// config for general layout
static const int LEGEND_POSITION_TOP = 0;	// distance between top of the terminal and legend
static const int BOARD_POSITION_TOP = 0;	// distance between top of the terminal and board
static const int ALL_OFFSET = 0;	// distance between everything and left side of the terminal
static const int BOARD_OFFSET = 1;		// distance between board and legend (if negative then board is on the left, and legend on the right)

// data for legend (all positions in this section are local for legend)
static const int LEGEND_WIDTH = 50;
static const int LEGEND_HEIGHT = 10;

static const char* INSTRUCTIONS =
"Instructions:\n"
"arrows - move cursor\n"
"n - start a new game\n"
"i - place stone\n"
"enter - confirm\n"
"esc - cancel action\n"
"s - save game\n"
"l - load game\n"
"q - quit\n"
"f - finish game\n";
static const int INSTRUCTIONS_POSITION_X = 0;
static const int INSTRUCTIONS_POSITION_Y = 0;

static const char* PERSONAL_DATA = "Bartlomiej Krawisz, 193319";  // for window title and legend
static const int PERSONAL_DATA_POSITION_X = 23;
static const int PERSONAL_DATA_POSITION_Y = 0;

static const char* DONE_LIST =
"Done:\n"
"a,b,c,d,e,f,g,h,i,j,k,l";
static const int DONE_LIST_POSITION_X = 23;
static const int DONE_LIST_POSITION_Y = 2;

static const char* SCORE_MESSAGE_FORMAT = "B: %d W: %d (+%.1f)";	// should contain %d for black's score, %d for white's score, and %.1f for white's bonus points
static const int SCORE_MESSAGE_MAX_LENGTH = 40;
static const int SCORE_MESSAGE_POSITION_X = 23;
static const int SCORE_MESSAGE_POSITION_Y = 5;

static const char* LOCATION_MESSAGE_FORMAT = "X: %d Y: %d";		// should contain %d for X position, and %d for Y position
static const int LOCATION_MESSAGE_MAX_LENGTH = 30;
static const int LOCATION_MESSAGE_POSITION_X = 23;
static const int LOCATION_MESSAGE_POSITION_Y = 6;

// game state messages for player (\7 makes warning sound)
static const char* HANDICAP_MESSAGE = "handicap mode";
static const char* SUICIDE_MESSAGE = "move is suicidal\7";
static const char* OCCUPIED_MESSAGE = "place is occupied\7";
static const char* ALREADY_PLACED_MESSAGE = "already placed\7";
static const char* KO_MESSAGE = "ko rule\7";
static const int GAME_STATUS_MAX_LENGTH = 30;	// must be bigger than the length of every individual message above
static const int GAME_STATUS_POSITION_X = 23;
static const int GAME_STATUS_POSITION_Y = 7;
static const char GAME_STATUS_COLOR = RED;

// other messages
static const char* FILENAME_QUERY = "Provide filename: ";
static const char* SAVING_HEADER = "### SAVING GAME ###";
static const char* LOADING_HEADER = "### LOADING GAME ###";
static const char* SAVE_SUCCESS = "Game saved!";
static const char* SAVE_ERROR = "Failed to save!";
static const char* LOAD_SUCCESS = "Game loaded!";
static const char* LOAD_ERROR = "Failed to load!";

static const char* BLACK_WON_MESSAGE = "Black won!";
static const char* WHITE_WON_MESSAGE = "White won!";
static const int WINNING_MESSAGE_MAX_LENGTH = 20;	// must be bigger than the length of every individual winning message above

static const char* GAME_FINISH_DIALOG_INSTRUCTIONS =
"Press n to play new game\n"
"Press ESC to cancel\n"
"Press q to quit\n";

static const char* CHOOSE_BOARD_SIZE_HEADER = "### Choose board size ###";
static const char* BOARD_SIZES[] = { "9x9", "13x13", "19x19", "other" };
static const int BOARD_SIZE_VALUES[] = { 9, 13, 19, -1 };   // sizes defined in BOARD_SIZES (-1 means user input)
static const char* BOARD_SIZE_QUERY = "Provide board size: ";

// user dialog restrictions
static const int FILENAME_MAX_LENGTH = 20;
static const int MAX_GAMEBOARD_SIZE = 999;
static const int MIN_GAMEBOARD_SIZE = 2;

// border view
static const unsigned char BORDER_SIGN = '#';
static const unsigned char BORDER_COLOR = WHITE;
static const unsigned char BORDER_BACKGROUND = BLACK;
static const unsigned char EMPTY_SIGN = 0xc5;	// cross-like sign
static const unsigned char EMPTY_COLOR = BLACK;
static const unsigned char EMPTY_BACKGROUND = DARKGRAY;
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