#include "go_game.h"

#include <cstdio>
#include <cstdlib>

template<typename T>
void swap(T& a, T& b) {
  T temp = a;
  a = b;
  b = temp;
}

GameState::GameState(const int board_size) : board_size(board_size) {
  board = (BoardSpace*)malloc(sizeof(BoardSpace) * board_size*board_size);
  reset();
}

void GameState::reset() {
  for(int i=0; i<board_size*board_size; ++i) {
    board[i] = {empty, -1};
  }
  score_black = 0;
  score_white = 0;
  score_bonus_white = 6.5;
  turn = black;
  chosen_x = -1;
  chosen_y = -1;
  isFirstTurn = true;
}

void GameState::resetChainIds() {
  for(int i=0; i<board_size*board_size; ++i) {
    board[i].chain_id = -1;
  }
}

bool GameState::copyTo(GameState &game_state) const {
  if(game_state.board_size != board_size) {
    return false;
  }

  game_state.reset();

  for(int i=0; i<board_size*board_size; ++i) {
    game_state.board[i] = board[i];
  }
  game_state.score_black = score_black;
  game_state.score_white = score_white;
  game_state.score_bonus_white = score_bonus_white;
  game_state.turn = turn;
  game_state.isFirstTurn = isFirstTurn;
  game_state.chosen_x = chosen_x;
  game_state.chosen_y = chosen_y;

  return true;
}

bool GameState::compareBoards(const GameState &game_state) const {
  if(game_state.board_size != board_size) {
    return false;
  }

  for(int i=0; i<board_size*board_size; ++i) {
    if(game_state.board[i].state != board[i].state) {
      return false;
    }
  }

  return true;
}

GameState::~GameState() {
  free(board);
}



int GoGame::_getIndexInBoard(const int x, const int y) const {
  if(x < 0 || x >= board_size) {
    return -1;
  }
  if(y < 0 || y >= board_size) {
    return -1;
  }
  return y*board_size + x;
}

BoardSpace GoGame::_getSpace(const int x, const int y, const GameState& game_state) const {
  const int index = _getIndexInBoard(x, y);
  if(index == -1) {
    return {edge, -1};
  }
  return game_state.board[index];
}

void GoGame::_setSpace(const int x, const int y, const BoardSpace value, GameState& game_state) {
  const int index = _getIndexInBoard(x, y);
  if(index == -1) {
    return;
  }
  game_state.board[index] = value;
}

bool GoGame::_madePlacement() const {
  return (_game_state.chosen_x != -1 && _game_state.chosen_y != -1);
}

bool GoGame::_isFirstTurn() const {
  return _original_game_state.isFirstTurn;
}

void GoGame::_generateNextGameState() {
  _game_state.copyTo(_next_game_state);

  _next_game_state.resetChainIds();
  _identifyAllChains();
  _countLiberties();
  // give advantage to just-placed stone by giving them one free liberty
  const int blessed_chain_id = _getSpace(_game_state.chosen_x,
                                         _game_state.chosen_y, 
                                         _next_game_state).chain_id;
  _liberties_by_chain[blessed_chain_id]++;
  _captureAllUnliberatedChains();
  
  // repeat liberty count, now without blessing, draws were resolved, because enemy wasn't 'blessed'
  _next_game_state.resetChainIds();
  _identifyAllChains();
  _countLiberties();
  _captureAllUnliberatedChains();

  _next_game_state.chosen_x = -1;
  _next_game_state.chosen_y = -1;

  switch (_game_state.turn) {
    case black:
      _next_game_state.turn = white;
    break;
    case white:
      _next_game_state.turn = black;
    break;
  }
}

void GoGame::_identifyAllChains() {
  _next_chain_id = 0;

  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      const BoardSpace space = _getSpace(x, y, _next_game_state);
      if(space.state == empty || space.state == edge || space.chain_id != -1) {
        continue;
      }
      _liberties_by_chain[_next_chain_id] = 0;
      _identifyChain(x, y, _next_chain_id++);
    }
  }
}

void GoGame::_identifyChain(const int x, const int y, const int& chain_id) {
  const BoardSpace space = _getSpace(x, y, _next_game_state);
  _setSpace(x, y, {space.state, chain_id}, _next_game_state);

  const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace i_space = _getSpace(ix, iy, _next_game_state);
    if(i_space.state != space.state || i_space.chain_id == chain_id) {
      continue;
    }
    _identifyChain(ix, iy, chain_id);
  }
}

void GoGame::_countLiberties() {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      const BoardSpace space = _getSpace(x, y, _next_game_state);
      if(space.state == empty) {
        _sendLiberties(x, y);
      }
    }
  }
}

void GoGame::_sendLiberties(const int x, const int y) {
  int visited_chains[4] = {-1,-1,-1,-1};
  const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};

  for(int i=0; i<4; ++i) {
    const int ix = x+directions[i][0];
    const int iy = y+directions[i][1];
    const BoardSpace i_space = _getSpace(ix, iy, _next_game_state);

    if(i_space.chain_id == -1) {
      continue;
    }

    bool visited = false;
    for(int j=0; j<i; ++j) {
      if(i_space.chain_id == visited_chains[j]) {
        visited = true;
      }
    }
    if(visited) {
      continue;
    }

    _liberties_by_chain[i_space.chain_id]++;
    visited_chains[i] = i_space.chain_id;
  }
}

void GoGame::_captureAllUnliberatedChains() {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      const BoardSpace space = _getSpace(x, y, _next_game_state);
      if(space.chain_id == -1) {
        continue;
      }

      const int liberty_count = _liberties_by_chain[space.chain_id];
      if(liberty_count > 0) {
        continue;
      }

      const int score = _captureUnliberatedChain(x, y);
      switch (space.state) {
        case blackStone:
          _next_game_state.score_white += score;
        break;
        case whiteStone:
          _next_game_state.score_black += score;
        break;
        default:  
        break;
      }
    }
  }
}

int GoGame::_captureUnliberatedChain(const int x, const int y) {
  const BoardSpace space = _getSpace(x, y, _next_game_state);
  _setSpace(x, y, {empty, -1}, _next_game_state);

  int score = 1;
  
  const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace i_space = _getSpace(ix, iy, _next_game_state);
    if(i_space.state == space.state) {
      score += _captureUnliberatedChain(ix, iy);
    }
  }

  return score;
}

void GoGame::_applyNextGameState() {
  _original_game_state.copyTo(_previous_game_state);
  _next_game_state.copyTo(_game_state);
  _game_state.isFirstTurn = false;
  _game_state.copyTo(_original_game_state);
}

void GoGame::_enableHandicap() {
  _next_game_state.score_bonus_white = 0.5;
}

GoGame::GoGame(const int board_size) :
  _previous_game_state(board_size),
  _original_game_state(board_size),
  _game_state(board_size),
  _next_game_state(board_size),
  board_size(board_size) {
  _liberties_by_chain = (int*)malloc(sizeof(int) * board_size*board_size);
}

BoardSpace GoGame::getSpace(const int x, const int y) const {
  return _getSpace(x, y, _game_state);
}

Player GoGame::getTurnState() const {
  return _game_state.turn;
}

BoardSpaceState GoGame::getPlayersStone() const {
  switch(getTurnState()) {
    case black:
      return blackStone;
    break;
    case white:
      return whiteStone;
    break;
  }
  return empty;
}

BoardSpaceState GoGame::getEnemysStone() const {
  switch(getTurnState()) {
    case black:
      return whiteStone;
    break;
    case white:
      return blackStone;
    break;
  }
  return empty;
}

MoveResult GoGame::placeStone(const int x, const int y) {
  if(_madePlacement() && !_isFirstTurn()) {
    return already_placed;
  }

  bool handicap_mode = false;
  if(_madePlacement() && _isFirstTurn()) {
    handicap_mode = true;
  }

  if(getSpace(x, y).state != empty) {
    return occupied;
  }

  _setSpace(x, y, {getPlayersStone(), -1}, _game_state);
  _game_state.chosen_x = x;
  _game_state.chosen_y = y;
  _generateNextGameState();

  if(_getSpace(x, y, _next_game_state).state == empty) {
    _setSpace(x, y, {empty, -1}, _game_state);
    return suicidal;
  }

  if(_previous_game_state.compareBoards(_next_game_state)) {
    _setSpace(x, y, {empty, -1}, _game_state);
    return ko;
  }

  _next_game_state.copyTo(_game_state);
  _game_state.chosen_x = x;
  _game_state.chosen_y = y;
  if(_isFirstTurn()) {
    _game_state.turn = black;
  }

  if(handicap_mode) {
    _enableHandicap();
    return handicap;
  }

  return done;
}

bool GoGame::confirmPlacement() {
  if(_madePlacement()) {
    _applyNextGameState();
    return true;
  }
  return false;
}

void GoGame::cancelPlacement() {
  _original_game_state.copyTo(_game_state);
}

void GoGame::exportBoard(char *str) const {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      char& c = str[y*board_size + x];
      const BoardSpace space = getSpace(x, y);
      switch (space.state) {
        case empty:
          c = '.';
        break;
        case blackStone:
          c = 'b';
        break;
        case whiteStone:
          c = 'w';
        break;
        case edge:
          c = '#';
        break;
      }
    }
  }
  str[board_size*board_size] = '\0';
}

GoGame::~GoGame() {
  free(_liberties_by_chain);
}