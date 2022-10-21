#pragma once

enum MoveResult {
  done,
  occupied,
  suicidal,
};

enum Player {
  black = 0,
  white = 1,
};

enum BoardSpaceState {
  empty,
  blackStone,
  whiteStone,
  edge,
};

struct BoardSpace {
  BoardSpaceState state;
  int liberties;
};

class GoGame {
private:
  BoardSpace *_board;
  int _board_width;
  int _board_height;
  Player _turn_state;   // whose turn it is
  
  const int _getIndexInBoard(const int x, const int y);
  void _setSpace(const int x, const int y, const BoardSpace space);
  void _nextTurn();
  void _clearBoard();

public:
  GoGame(const int board_width, const int board_height);
  const int getBoardWidth();
  const int getBoardHeight();
  BoardSpace getSpace(const int x, const int y);
  Player getTurnState();
  BoardSpaceState getPlayersStone();   // return black or white stone depending on whose turn it is
  BoardSpaceState getEnemysStone();    // return opposite of getPlayerStone()
  MoveResult placeStone(const int x, const int y);

  ~GoGame();
};