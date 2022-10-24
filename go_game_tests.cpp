#include <cassert>
#include <cstdio>
#include "go_game.h"

void applyTextToGameState(GameState& game_state, const char *text) {
  for(int y=0; y<game_state.board_size; ++y) {
    for(int x=0; x<game_state.board_size; ++x) {
      const char sign = text[y*game_state.board_size + x];
      switch(sign) {
        case '.':
          game_state.board[y*game_state.board_size + x] = {empty, -1};
        break;
        case 'b':
          game_state.board[y*game_state.board_size + x] = {blackStone, -1};
        break;
        case 'w':
          game_state.board[y*game_state.board_size + x] = {whiteStone, -1};
        break;
      }
    }
  }
}

bool compare(const char *str1, const char *str2) {
  for(int i=0; str1[i] != '\0' && str2[i] != '\0'; ++i) {
    if(str1[i] != str2[i]) {
      return false;
    }
  }
  return true;
}

void simpleStonePlacing() {
  GoGame game(7);
  game.placeStone(4, 2);
  game.placeStone(4, 3);
  game.placeStone(2, 1);
  game.placeStone(3, 4);
  char result[7*7 + 1];
  game.exportBoard(result);
  const char expected[] =
          "......."
          "..b...."
          "....b.."
          "....w.."
          "...w..."
          "......."
          ".......";
  assert(compare(result, expected) && "simple stone placing");
}

void simpleCapturing() {
  GoGame game(7);
  game.placeStone(4, 2);
  game.placeStone(3, 2);
  game.placeStone(3, 1);
  game.placeStone(0, 0);
  game.placeStone(3, 3);
  game.placeStone(1, 0);
  game.placeStone(2, 2);
  char result[7*7 + 1];
  game.exportBoard(result);
  const char expected[] =
          "ww....."
          "...b..."
          "..b.b.."
          "...b..."
          "......."
          "......."
          ".......";
  assert(compare(result, expected) && "simple capturing");
}

int main() {
  simpleStonePlacing();
  simpleCapturing();
  printf("Passed all tests!\n");
}