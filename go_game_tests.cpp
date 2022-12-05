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
  printf("Testing Simple stone placing...");
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
  assert(move_result == done && "simple stone placing should return 'done' result");
  assert(compare(result, expected) && "simple stone placing should work");

  puts("OK");
}

void placeOnOccupiedField() {
  printf("Testing Placing on occupied field...");
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
  assert(move_result == occupied && "place on occupied field should return 'occupied' result");
  assert(compare(result, expected) && "place on occupied field should let to put the same stone again");

  puts("OK");
}

void simpleStonePlacing2() {
  printf("Testing Simple stone placing further...");
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

  puts("OK");
}

void placementCancellation() {
  printf("Testing Placement cancellation...");
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

  puts("OK");
}

void simpleCapturing() {
  printf("Testing Simple capturing...");
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
  assert(compare(result, expected) && "simple capturing should be possible");

  puts("OK");
}

void nearEdgeCapturing() {
  printf("Testing Near-edge capturing...");
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
  assert(compare(result, expected) && "near-edge capturing should be possible");

  puts("OK");
}

void longChainCapturing() {
  printf("Testing Long chain capturing...");
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

  puts("OK");
}

void advancedSuicidal() {
  printf("Testing Advanced suicidal...");
  GoGame game(4);

  const int moves[][2] = {{0,1},{0,2},
                          {1,0},{1,2},
                          {1,1},{2,1},
                          {3,3},{2,0}};
  for(auto move: moves) {
    game.placeStone(move[0], move[1]);
    game.confirmPlacement();
  }

  const MoveResult move_result = game.placeStone(0, 0);
  assert(move_result == suicidal && "stone should be suicidal if move destroys its chain");

  puts("OK");
}

void koRule() {
  printf("Testing Ko rule...");
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

  const MoveResult move_result = game.placeStone(2, 1);
  assert(move_result == ko && "trying to repeat previous situation results in 'ko'");

  puts("OK");
}

void handicapMode() {
  printf("Testing Handicap...");

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

  puts("OK");
}

void creatingNewGame() {
  printf("Testing Creating new game...");

  GoGame game = GoGame(3);
  game.placeStone(1, 2);
  game.confirmPlacement();
  game.placeStone(0, 2);
  game.confirmPlacement();

  game = GoGame(4);
  char result[4*4 + 1];
  game.exportBoard(result);
  const char expected[] =
          "...."
          "...."
          "...."
          "....";
  assert(compare(result, expected) && "new game should be created");

  puts("OK");
}

void savingAndLoading() {
  printf("Testing Saving and loading:\n");
  const char *save_filename = "test_save.sav";

  GoGame game(3);

  game.placeStone(0, 0);
  game.confirmPlacement();
  game.placeStone(1, 1);
  game.confirmPlacement();

  printf("\tSaving game...");
  FILE* save_file = fopen(save_filename, "wb");
  game.save(save_file);
  fclose(save_file);
  puts("OK");

  printf("\tCreating new game with different size...");
  GoGame game2(5);
  char result1[5*5 + 1];

  game2.placeStone(3, 2);
  game2.confirmPlacement();
  game2.placeStone(1, 3);
  game2.confirmPlacement();
  game2.exportBoard(result1);
  const char expected[] =
          "....."
          "....."
          "...b."
          ".w..."
          ".....";
  assert(compare(result1, expected) && "new game should be created");
  puts("OK");

  printf("\tLoading old game...");
  FILE *load_file = fopen(save_filename, "rb");
  game2.load(load_file);
  fclose(load_file);
  char result2[3*3 + 1];

  game2.exportBoard(result2);
  const char expected2[] =
          "b.."
          ".w."
          "...";
  assert(compare(result2, expected2) && "saved game should be loaded");

  remove(save_filename);
  puts("OK");
}

void scoringCaptures() {
  printf("Testing Scoring captures...");

  GoGame game = GoGame(4);
  assert(game.getScoreBlack() == 0 && "new game should start with 0 points");
  assert(game.getScoreWhite() == 0 && "new game should start with 0 points");

  game.placeStone(1, 2);
  game.placeStone(2, 1);
  game.placeStone(2, 3);
  game.placeStone(3, 1);
  game.confirmPlacement();
  game.placeStone(2, 2);
  game.confirmPlacement();
  game.placeStone(1, 3);
  game.confirmPlacement();
  game.placeStone(3, 2);
  game.confirmPlacement();
  game.placeStone(3, 3);
  game.confirmPlacement();

  assert(game.getScoreBlack() == 2 && "after capturing two white stones black should have 2 points");
  assert(game.getScoreWhite() == 0 && "whites should still have 0 points");

  puts("OK");
}

void scoringTerritory() {
  printf("Testing Territory scoring...");

  GoGame game = GoGame(9);

  const int moves[][2] = {{0,0},{2,0},
                          {4,0},{6,0},
                          {5,0},{1,1},
                          {3,1},{2,1},
                          {4,1},{5,1},
                          {2,2},{6,1},
                          {3,2},{0,2},
                          {0,3},{1,2},
                          {1,3},{5,2},
                          {3,3},{5,3},
                          {3,4},{5,4},
                          {3,5},{5,5},
                          {3,6},{7,5},
                          {7,6},{8,5},
                          {8,6},{5,6},
                          {2,7},{6,6},
                          {3,7},{4,7},
                          {6,7},{5,7},
                          {7,7},{3,8},
                          {2,8},{4,8},
                          {6,8}};
  game.placeStone(8, 8);

  for(auto move: moves) {
    game.placeStone(move[0], move[1]);
    game.confirmPlacement();
  }

  char result[9*9+1];
  const char expected[] =
          "b.w.bbw.."
          ".wwbbww.."
          "wwbb.w..."
          "bb.b.w..."
          "...b.w..."
          "...b.w.ww"
          "...b.wwbb"
          "..bbwwbb."
          "..bww.b.b";
  game.exportBoard(result);
  assert(compare(result, expected) && "Figure 3");
  
  game.finishGame();

  // game.exportBoard(result);
  // for(int x=0; x<9; ++x) {
  //   for(int y=0; y<9; ++y) {
  //     const BoardSpace space = game.getSpace(y, x);
  //     printf("%d%c ", (int)space.territory_owner, result[x*9+y]);
  //   }
  //   printf("\n");
  // }

  // printf("b: %d w: %d\n", game.getScoreBlack(), game.getScoreWhite());

  assert(game.getScoreBlack() == 8+14+2 && "example from instruction pdf (Figure 3)");
  assert(game.getScoreWhite() == 14 && "example from instruction pdf (Figure 3)");

  puts("OK");
}

int main() {
  puts("Start testing...");
  simpleStonePlacing();
  placeOnOccupiedField();
  simpleStonePlacing2();
  placementCancellation();
  simpleCapturing();
  nearEdgeCapturing();
  longChainCapturing();
  advancedSuicidal();
  koRule();
  handicapMode();
  creatingNewGame();
  savingAndLoading();
  scoringCaptures();
  scoringTerritory();
  fprintf(stdout, "Passed all tests!\n");
}