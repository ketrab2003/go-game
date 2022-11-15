#pragma once

#include "array2D.h"
#include <cstdio>

enum MoveResult {
  done,
  occupied,
  suicidal,
  already_placed,
  ko,
  handicap,
};

enum Player {
  black = 0,
  white = 1,
};

enum BoardSpaceState {
  empty = 0,
  blackStone = 1,
  whiteStone = 2,
  edge = 3,
};

struct BoardSpace {
  BoardSpaceState state = empty;
  int visited_id = -1;
};

class GameState {
  Array2D<BoardSpace> _board;
  int _board_size;

  bool _validCoords(const int x, const int y) const;
public:
  Player turn;    // whose turn it is
  bool isFirstTurn;  // becomes false after first placement confirm, used to recognize whether to enable handicap
  int score_black;
  int score_white;
  float score_bonus_white;

  int chosen_x, chosen_y;

  GameState(const int board_size);
  void reset_visited();

  BoardSpace getSpace(const int x, const int y) const;
  void setSpace(const int x, const int y, const BoardSpace space);
  void setSpace(const int x, const int y, const BoardSpaceState space_state);
  void visitSpace(const int x, const int y, const int id);

  int getBoardSize() const;

  bool compareBoards(const GameState& game_state) const;

  void save(FILE *file) const;
  void load(FILE *file);
};

class GoGame {
  GameState _previous_game_state;
  GameState _original_game_state;
  GameState _game_state;
  GameState _next_game_state;

  bool _madePlacement() const;
  bool _isFirstTurn() const;

  void _generateNextGameState();
  int _countChainLiberties(const int x, const int y, const int &visit_id);
  void _removeChain(const int x, const int y);
  void _applyNextGameState();

  void _enableHandicap();

public:

  GoGame(const int board_size);

  BoardSpace getSpace(const int x, const int y) const;    // if coordinates are outside of board, it returns edge space
  int getBoardSize() const;
  Player getTurnState() const;    // return black or white depending on whose turn it is
  BoardSpaceState getPlayersStone() const;   // return black or white stone depending on whose turn it is
  BoardSpaceState getEnemysStone() const;    // return opposite of getPlayerStone()
  int getChosenX() const;
  int getChosenY() const;

  MoveResult placeStone(const int x, const int y);   // try to place stone at (x,y), color of stone is decided based on internal turn state
  bool confirmPlacement();
  void cancelPlacement();

  void save(FILE *file) const;
  void load(FILE *file);

  void exportBoard(char *str) const;
};