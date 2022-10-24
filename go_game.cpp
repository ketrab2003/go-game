#include "go_game.h"

#include <cstdlib>

template<typename T>
void swap(T& a, T& b) {
  T temp = a;
  a = b;
  b = temp;
}

#define min(a, b) (a < b ? a : b)

GameState::GameState(const int board_size) : board_size(board_size) {
  board = (BoardSpace*)malloc(sizeof(BoardSpace) * board_size*board_size);
  liberties_by_chain = (int*)malloc(sizeof(int) * board_size*board_size);
  reset();
}

void GameState::reset() {
  for(int i=0; i<board_size*board_size; ++i) {
    board[i] = {empty, -1};
    liberties_by_chain[i] = 0;
  }
  next_chain_id = 0;
  score_black = 0;
  score_white = 0;
  turn = black;
}

void GameState::swapWith(GameState &game_state) {
  swap(game_state.board, board);
  swap(game_state.liberties_by_chain, liberties_by_chain);
  swap(game_state.next_chain_id, next_chain_id);
  swap(game_state.score_black, score_black);
  swap(game_state.score_white, score_white);
  swap(game_state.turn, turn);
}

GameState::~GameState() {
  free(board);
  free(liberties_by_chain);
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

void GoGame::_clearBoard(GameState& game_state) {
  for(int i=0; i<board_size*board_size; ++i) {
    game_state.board[i].state = empty;
    game_state.board[i].chain_id = -1;
  }
}

void GoGame::_generateNextGameState() {
  _next_game_state.reset();
  _copyStonesToNextState();
  _propagateAllChains();
  _countLiberties();
  _captureAllUnliberatedChains();

  switch (_game_state.turn) {
    case black:
      _next_game_state.turn = white;
    break;
    case white:
      _next_game_state.turn = black;
    break;
  }
}

void GoGame::_copyStonesToNextState() {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      const BoardSpace original_space = _getSpace(x, y, _game_state);
      _setSpace(x, y, {original_space.state, -1}, _next_game_state);
    }
  }
}

void GoGame::_propagateAllChains() {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      const BoardSpace space = _getSpace(x, y, _next_game_state);
      if(space.state == empty || space.state == edge || space.chain_id != -1) {
        continue;
      }
      _propagateChain(x, y, _next_game_state.next_chain_id++);
    }
  }
}

void GoGame::_propagateChain(const int x, const int y, const int& chain_id) {
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
    _propagateChain(ix, iy, chain_id);
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

    _next_game_state.liberties_by_chain[i_space.chain_id]++;
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

      const int liberty_count = _next_game_state.liberties_by_chain[space.chain_id];
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
  _game_state.swapWith(_next_game_state);
}

GoGame::GoGame(const int board_size) :
  board_size(board_size),
  _game_state(board_size),
  _next_game_state(board_size) {
  _clearBoard(_game_state);
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
  if(getSpace(x, y).state != empty) {
    return occupied;
  }
  _setSpace(x, y, {getPlayersStone(), -1}, _game_state);
  
  _generateNextGameState();
  if(_getSpace(x, y, _next_game_state).state == empty) {
    _setSpace(x, y, {empty, -1}, _game_state);
    return suicidal;
  }

  _applyNextGameState();
  return done;
}

void GoGame::exportBoard(char *str) const {
  for(int y=0; y<board_size; ++y) {
    for(int x=0; x<board_size; ++x) {
      char& c = str[y*board_size + x];
      switch (getSpace(x, y).state) {
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
  str[board_size*board_size+1] = '\0';
}