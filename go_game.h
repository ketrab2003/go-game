#pragma once

enum MoveResult {
  done,
  occupied,
  suicidal,
  already_placed,
  ko,
  handicap,
};

enum Player {
  black,
  white,
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
  const int board_size;
  Player turn;    // whose turn it is
  bool isFirstTurn;  // becomes false after first placement confirm, used to recognize whether to enable handicap
  int score_black;
  int score_white;
  float score_bonus_white;

  int chosen_x, chosen_y;

  GameState(const int board_size);
  void reset();
  void resetChainIds();
  bool copyTo(GameState& game_state) const;   // copies everything, except for liberties and chain ids,
                                              // they need to be re-identified after every change anyway
  bool compareBoards(const GameState& game_state) const;
  ~GameState();
};

class GoGame {
  GameState _previous_game_state;
  GameState _original_game_state;
  GameState _game_state;
  GameState _next_game_state;

  int *_liberties_by_chain;
  int _next_chain_id;
  
  int _getIndexInBoard(const int x, const int y) const;
  BoardSpace _getSpace(const int x, const int y, const GameState& game_state) const;

  void _setSpace(const int x, const int y, const BoardSpace space, GameState& game_state);
  bool _madePlacement() const;
  bool _isFirstTurn() const;

  void _generateNextGameState();
  void _identifyAllChains();
  void _identifyChain(const int x, const int y, const int& chain_id);
  void _countLiberties();
  void _sendLiberties(const int x, const int y);
  void _captureAllUnliberatedChains();    // also add captured stones to player score
  int _captureUnliberatedChain(const int x, const int y);   // return unliberated chain count, for player's score
  void _applyNextGameState();

  void _enableHandicap();

public:
  const int board_size;

  GoGame(const int board_size);

  BoardSpace getSpace(const int x, const int y) const;    // if coordinates are outside of board, it returns edge space
  Player getTurnState() const;    // return black or white depending on whose turn it is
  BoardSpaceState getPlayersStone() const;   // return black or white stone depending on whose turn it is
  BoardSpaceState getEnemysStone() const;    // return opposite of getPlayerStone()
  int getChosenX() const;
  int getChosenY() const;

  MoveResult placeStone(const int x, const int y);   // try to place stone at (x,y), color of stone is decided based on internal turn state
  bool confirmPlacement();
  void cancelPlacement();

  void exportBoard(char *str) const;

  ~GoGame();
};