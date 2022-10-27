#include <cassert>
#include <cstdio>
#include "go_game.h"

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
  char result[7*7 + 1];

  game.placeStone(4, 2);
  game.confirmPlacement();
  game.placeStone(4, 3);
  game.confirmPlacement();
  game.placeStone(2, 1);
  game.confirmPlacement();
  MoveResult move_result = game.placeStone(3, 4);
  game.confirmPlacement();
  
  game.exportBoard(result);
  const char expected[] =
          "......."
          "..b...."
          "....b.."
          "....w.."
          "...w..."
          "......."
          ".......";
  assert(move_result == done && "simple stone placing returns 'done' result");
  assert(compare(result, expected) && "simple stone placing");
}

void placeOnOccupiedField() {
  GoGame game(5);
  char result[5*5 + 1];

  game.placeStone(2, 2);
  game.confirmPlacement();

  MoveResult move_result = game.placeStone(2, 2);
  game.placeStone(1, 1);
  game.confirmPlacement();

  game.exportBoard(result);
  const char expected[] =
          "....."
          ".w..."
          "..b.."
          "....."
          ".....";
  assert(move_result == occupied && "place on occupied field returns 'occupied' result");
  assert(compare(result, expected) && "place on occupied field lets to put the same stone again");
}

void simpleStonePlacing2() {
  GoGame game(4);
  char result[4*4 + 1];

  game.placeStone(1, 1);

  game.exportBoard(result);
  const char expected[] =
          "...."
          ".b.."
          "...."
          "....";
  assert(compare(result, expected) && "placed stone should be on board even before confirmation");

  game.confirmPlacement();
  game.placeStone(1, 2);
  const MoveResult move_result = game.placeStone(2, 3);
  assert(move_result == already_placed && "trying to place stone second time without confirmation nor cancelling should result in 'already_placed'");

  game.confirmPlacement();

  game.exportBoard(result);
  const char expected2[] =
          "...."
          ".b.."
          ".w.."
          "....";
  assert(compare(result, expected2) && "after failed second placing, previous chosen place should be preserved");
}

void placementCancellation() {
  GoGame game(4);
  char result[4*4 + 1];

  game.placeStone(0, 0);
  game.confirmPlacement();

  game.placeStone(2, 1);
  game.placeStone(1, 1);

  game.exportBoard(result);
  const char expected[] =
          "b..."
          "..w."
          "...."
          "....";
  assert(compare(result, expected) && "after choosing other place, stone should not switch places");

  game.cancelPlacement();

  game.exportBoard(result);
  const char expected2[] =
          "b..."
          "...."
          "...."
          "....";
  assert(compare(result, expected2) && "after cancelling, placed stone should disappear");

  game.placeStone(1, 1);
  
  game.exportBoard(result);
  const char expected3[] =
          "b..."
          ".w.."
          "...."
          "....";
  assert(compare(result, expected3) && "after cancelling, player should be able to choose other place");
}

void simpleCapturing() {
  GoGame game(7);
  char result[7*7 + 1];

  game.placeStone(4, 2);
  game.confirmPlacement();
  game.placeStone(3, 2);
  game.confirmPlacement();
  game.placeStone(3, 1);
  game.confirmPlacement();
  game.placeStone(0, 0);
  game.confirmPlacement();
  game.placeStone(3, 3);
  game.confirmPlacement();
  game.placeStone(1, 0);
  game.confirmPlacement();
  game.placeStone(2, 2);
  game.confirmPlacement();

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

void nearEdgeCapturing() {
  GoGame game(5);
  char result[5*5 + 1];

  game.placeStone(0, 0);
  game.confirmPlacement();
  game.placeStone(0, 1);
  game.confirmPlacement();
  game.placeStone(2, 2);
  game.confirmPlacement();
  game.placeStone(1, 0);
  game.confirmPlacement();

  game.exportBoard(result);
  const char expected[] =
          ".w..."
          "w...."
          "..b.."
          "....."
          ".....";
    assert(compare(result, expected) && "near-edge capturing");
}

void longChainCapturing() {
  GoGame game(5);
  char result[5*5 + 1];

  const int moves[][2] = {{0,4},{1,0},
                          {1,1},{2,0},
                          {2,1},{3,0},
                          {3,1},{0,1},
                          {1,2},{4,1},
                          {2,2},{0,2},
                          {3,2},{4,2},
                          {2,3},{1,3},
                          {4,3},{3,3},
                          {4,4},{2,4}};
  for(auto move: moves) {
    game.placeStone(move[0], move[1]);
    game.confirmPlacement();
  }
  game.exportBoard(result);
  const char expected[] =
          ".www."
          "w...w"
          "w...w"
          ".w.wb"
          "b.w.b";
  assert(compare(result, expected) && "capturing of long chains should be supported");
}

void koRule() {
  GoGame game(4);
  char result[4*4 + 1];

  const int moves[][2] = {{2,0},{1,0},
                          {3,1},{0,1},
                          {2,2},{1,2},
                          {0,3},{2,1}};
  for(auto move: moves) {
    game.placeStone(move[0], move[1]);
    game.confirmPlacement();
  }
  game.placeStone(1, 1);
  game.confirmPlacement();

  game.exportBoard(result);
  const char expected[] =
          ".wb."
          "wb.b"
          ".wb."
          "b...";
  assert(compare(result, expected) && "newly placed stone should get priority in capturing");

  MoveResult move_result = game.placeStone(2, 1);
  assert(move_result == ko && "trying to repeat previous situation results in 'ko'");
}

void handicapMode() {
  GoGame game(4);
  char result[4*4 + 1];

  game.placeStone(1, 1);
  const MoveResult move_result = game.placeStone(2, 2);

  assert(move_result == handicap && "game should allow black on the beggining place multiple stones, and return 'handicap' result");

  game.placeStone(2, 1);
  game.confirmPlacement();

  game.exportBoard(result);
  const char expected[] =
          "...."
          ".bb."
          "..b."
          "....";
  assert(compare(result, expected) && "game should allow black on the beggining place multiple stones");

  game.placeStone(1, 0);
  const MoveResult move_result2 = game.placeStone(2, 0);

  assert(move_result2 == already_placed && "white should never be able to place multiple stones");

  game.confirmPlacement();
  game.placeStone(1, 3);
  const MoveResult move_result3 = game.placeStone(2, 3);

  assert(move_result3 == already_placed && "black should be able to place multiple stones only in first turn");
}

void scoringCaptures() {
  // TODO: implement
}

void scoringTerritory() {
  // TODO: implement
}

int main() {
  simpleStonePlacing();
  placeOnOccupiedField();
  simpleStonePlacing2();
  placementCancellation();
  simpleCapturing();
  nearEdgeCapturing();
  longChainCapturing();
  koRule();
  handicapMode();
  fprintf(stdout, "Passed all tests!\n");
}