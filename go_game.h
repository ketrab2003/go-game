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
  int chain_id;
};

struct GameState {
  BoardSpace *board;
  int board_size;
  int *liberties_by_chain;
  int next_chain_id;
  Player turn;    // whose turn it is
  int score_black;
  int score_white;

  GameState(const int board_size);
  void reset();
  void swapWith(GameState& game_state);
  ~GameState();
};

class GoGame {
  GameState _game_state;
  GameState _next_game_state;
  
  int _getIndexInBoard(const int x, const int y) const;
  BoardSpace _getSpace(const int x, const int y, const GameState& game_state) const;

  void _setSpace(const int x, const int y, const BoardSpace space, GameState& game_state);
  void _clearBoard(GameState& game_state);

  void _generateNextGameState();
  void _copyStonesToNextState();
  void _propagateAllChains();
  void _propagateChain(const int x, const int y, const int& chain_id);
  void _countLiberties();
  void _sendLiberties(const int x, const int y);
  void _captureAllUnliberatedChains();    // also add captured stones to player score
  int _captureUnliberatedChain(const int x, const int y);   // return unliberated chain count, for player's score
  void _applyNextGameState();

public:
  const int board_size;

  GoGame(const int board_size);
  GoGame(const GameState& game_state);

  BoardSpace getSpace(const int x, const int y) const;    // if coordinates are outside of board, it returns edge space
  Player getTurnState() const;    // return black or white depending on whose turn it is
  BoardSpaceState getPlayersStone() const;   // return black or white stone depending on whose turn it is
  BoardSpaceState getEnemysStone() const;    // return opposite of getPlayerStone()

  MoveResult placeStone(const int x, const int y);   // try to place stone at (x,y), color of stone is decided based on internal turn state

  void exportBoard(char *src) const;
};