#include "go_game.h"

#include "array2D.h"

bool GameState::_validCoords(const int x, const int y) const {
  if(x < 0 || x >= getBoardSize()) {
    return false;
  }
  if(y < 0 || y >= getBoardSize()) {
    return false;
  }
  return true;
}

GameState::GameState(const int board_size)
: _board(board_size, board_size) {
  _board_size = board_size;
  score_black = 0;
  score_white = 0;
  score_bonus_white = 6.5;
  turn = black;
  chosen_x = -1;
  chosen_y = -1;
  isFirstTurn = true;
}

void GameState::resetVisited() {
  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      _board[x][y].visited_id = -1;
    }
  }
}

void GameState::resetOwnership() {
  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      _board[x][y].territory_owner = none;
    }
  }
}

BoardSpace GameState::getSpace(const int x, const int y) const {
  if(!_validCoords(x, y)) {
    return {edge, -1};
  }
  return _board[x][y];
}

void GameState::setSpace(const int x, const int y, const BoardSpace space) {
  if(!_validCoords(x, y)) {
    return;
  }
  _board[x][y] = space;
}

void GameState::setSpace(const int x, const int y, const BoardSpaceState space_state) {
  if(!_validCoords(x, y)) {
    return;
  }
  _board[x][y].state = space_state;
}

void GameState::visitSpace(const int x, const int y, const int id) {
  if(!_validCoords(x, y)) {
    return;
  }
  _board[x][y].visited_id = id;
}

void GameState::setSpaceOwner(const int x, const int y, const Player owner) {
  if(!_validCoords(x, y)) {
    return;
  }
  _board[x][y].territory_owner = owner;
}

int GameState::getBoardSize() const {
  return _board_size;
}

bool GameState::compareBoards(const GameState &game_state) const {
  if(game_state.getBoardSize() != getBoardSize()) {
    return false;
  }

  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      const BoardSpaceState state1 = game_state.getSpace(x, y).state;
      const BoardSpaceState state2 = getSpace(x, y).state;
      if(state1 != state2) {
        return false;
      }
    }
  }

  return true;
}

void GameState::save(FILE *file) const {
  if(file == NULL) {
    return;
  }

  const int board_size = getBoardSize();
  fwrite(&board_size, sizeof(board_size), 1, file);

  fwrite(&turn, sizeof(turn), 1, file);
  fwrite(&isFirstTurn, sizeof(isFirstTurn), 1, file);
  fwrite(&score_black, sizeof(score_black), 1, file);
  fwrite(&score_white, sizeof(score_white), 1, file);
  fwrite(&score_bonus_white, sizeof(score_bonus_white), 1, file);
  fwrite(&chosen_x, sizeof(chosen_x), 1, file);
  fwrite(&chosen_y, sizeof(chosen_y), 1, file);

  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      const BoardSpaceState state = getSpace(x, y).state;
      fwrite(&state, sizeof(state), 1, file);
    }
  }
}

void GameState::load(FILE *file) {
  if(file == NULL) {
    return;
  }

  fread(&_board_size, sizeof(_board_size), 1, file);
  if(_board.getWidth() != _board_size || _board.getHeight() != _board_size) {
    _board = Array2D<BoardSpace>(_board_size, _board_size);
  }

  fread(&turn, sizeof(turn), 1, file);
  fread(&isFirstTurn, sizeof(isFirstTurn), 1, file);
  fread(&score_black, sizeof(score_black), 1, file);
  fread(&score_white, sizeof(score_white), 1, file);
  fread(&score_bonus_white, sizeof(score_bonus_white), 1, file);
  fread(&chosen_x, sizeof(chosen_x), 1, file);
  fread(&chosen_y, sizeof(chosen_y), 1, file);

  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      BoardSpaceState state;
      fread(&state, sizeof(state), 1, file);
      setSpace(x, y, state);
    }
  }
}



bool GoGame::_madePlacement() const {
  return (_game_state.chosen_x != -1 && _game_state.chosen_y != -1);
}

bool GoGame::_isFirstTurn() const {
  return _original_game_state.isFirstTurn;
}

void GoGame::_generateNextGameState() {
  _next_game_state = _game_state;
  _next_game_state.resetVisited();

  const int start_x = _next_game_state.chosen_x;
  const int start_y = _next_game_state.chosen_y;

  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(int i=0; i<4; ++i) {
    const int ix = start_x + directions[i][0];
    const int iy = start_y + directions[i][1];
    const BoardSpace ispace = _next_game_state.getSpace(ix, iy);
    if(ispace.state == getEnemysStone()) {  // first check only enemies
      if(_countChainLiberties(ix, iy, i) == 0) {
        _removeChain(ix, iy);
      }
    }
  }

  if(_countChainLiberties(start_x, start_y, 4) == 0) {
    // placed stone lost => the whole state is invalid, so we don't need to remove the whole chain
    _next_game_state.setSpace(start_x, start_y, empty);
  }

  _next_game_state.chosen_x = -1;
  _next_game_state.chosen_y = -1;

  switch (_game_state.turn) {
    case black:
      _next_game_state.turn = white;
    break;
    case white:
      _next_game_state.turn = black;
    break;
    default:
    break;
  }
}

int GoGame::_countChainLiberties(const int x, const int y, const int &visit_id) {
  const BoardSpace here = _next_game_state.getSpace(x, y);
  if(here.visited_id == visit_id) {
    return 0;
  }
  _next_game_state.visitSpace(x, y, visit_id);

  if(here.state == empty) {
    return 1;
  }

  int liberties = 0;

  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace ispace = _next_game_state.getSpace(ix, iy);
    if(ispace.state == empty || ispace.state == here.state) {
      liberties += _countChainLiberties(ix, iy, visit_id);
    }
  }

  return liberties;
}

void GoGame::_removeChain(const int x, const int y) {
  const BoardSpace here = _next_game_state.getSpace(x, y);
  // add points during removal
  if(here.state == blackStone) {
    _next_game_state.score_white++;
  } else if(here.state == whiteStone) {
    _next_game_state.score_black++;
  }

  _next_game_state.setSpace(x, y, empty);
  
  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace ispace = _next_game_state.getSpace(ix, iy);
    if(ispace.state == here.state) {
      _removeChain(ix, iy);
    }
  }
}

void GoGame::_applyNextGameState() {
  _previous_game_state = _original_game_state;
  _game_state = _next_game_state;
  _game_state.isFirstTurn = false;
  _original_game_state = _game_state;
}

void GoGame::_removeDeadChains() {
  _setAllTerritoriesOwner();

  int visit_id = 1;
  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<0; ++y) {
      const BoardSpace ispace = _next_game_state.getSpace(x, y);
      if(ispace.state != empty && ispace.visited_id < visit_id && _isChainDead(x, y, visit_id++)) {
        _removeChain(x, y);
      }
    }
  }
}

bool GoGame::_isChainDead(const int x, const int y, const int &visit_id) {
  const BoardSpace here = _next_game_state.getSpace(x, y);
  bool isDead = true;

  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace space = _next_game_state.getSpace(ix, iy);

    if(space.state == here.state && space.visited_id != visit_id) {
      isDead &= _isChainDead(ix, iy, visit_id);
    } else if(space.state == empty) {
      switch(here.state) {
        case blackStone:
          if(space.territory_owner == black) {
            return false;
          }
        break;
        case whiteStone:
          if(space.territory_owner == white) {
            return false;
          }
        break;
        default:
        break;
      }
    }

    if(!isDead) {
      return false;
    }
  }

  return true;
}

void GoGame::_setAllTerritoriesOwner() {
  _next_game_state.resetVisited();
  _next_game_state.resetOwnership();

  int visit_id = 1;
  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      const BoardSpace space = _next_game_state.getSpace(x, y);
      if(space.state == empty && space.visited_id < visit_id) {
        const Player owner = _identifyTerritory(x, y, visit_id++);
        _setTerritoryOwner(x, y, owner);
      }
    }
  }
}

Player GoGame::_identifyTerritory(const int x, const int y, const int &visit_id) {
  const BoardSpace here = _next_game_state.getSpace(x, y);
  Player here_owner = none;

  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    const BoardSpace ispace = _next_game_state.getSpace(ix, iy);
    if(ispace.state == empty && ispace.visited_id != visit_id) {
      const Player neighbour_owner = _identifyTerritory(ix, iy, visit_id);
      if(neighbour_owner == none) {
        return none;
      } else if(here_owner == none || here_owner == neighbour_owner) {
        here_owner = neighbour_owner;
      } else {
        return none;
      }
    }
  }

  return here_owner;
}

void GoGame::_setTerritoryOwner(const int x, const int y, const Player &owner) {
  const BoardSpace here = _next_game_state.getSpace(x, y);
  if(here.state != empty) {
    return;
  }
  if(here.territory_owner == owner) {
    return;
  }

  _next_game_state.setSpaceOwner(x, y, owner);

  static const int directions[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
  for(auto direction : directions) {
    const int ix = x+direction[0];
    const int iy = y+direction[1];
    _setTerritoryOwner(ix, iy, owner);
  }
}

void GoGame::_countTerritoryPoints() {
  for(int x=0; x<getBoardSize(); ++x) {
    for(int y=0; y<getBoardSize(); ++y) {
      const BoardSpace space = _next_game_state.getSpace(x, y);
      switch(space.territory_owner) {
        case black:
          _next_game_state.score_black++;
        break;
        case white:
          _next_game_state.score_white++;
        break;
        default:
        break;
      }
    }
  }
}

void GoGame::_enableHandicap() {
  _next_game_state.score_bonus_white = 0.5;
  _game_state.score_bonus_white = 0.5;
}

GoGame::GoGame(const int board_size) :
  _previous_game_state(board_size),
  _original_game_state(board_size),
  _game_state(board_size),
  _next_game_state(board_size) {
}

BoardSpace GoGame::getSpace(const int x, const int y) const {
  return _game_state.getSpace(x, y);
}

int GoGame::getBoardSize() const {
  return _game_state.getBoardSize();
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
    default:
      return empty;
    break;
  }
}

BoardSpaceState GoGame::getEnemysStone() const {
  switch(getTurnState()) {
    case black:
      return whiteStone;
    break;
    case white:
      return blackStone;
    break;
    default:
      return empty;
    break;
  }
}

int GoGame::getChosenX() const {
  return _game_state.chosen_x;
}

int GoGame::getChosenY() const {
  return _game_state.chosen_y;
}

int GoGame::getScoreBlack() const {
  return _game_state.score_black;
}

int GoGame::getScoreWhite() const {
  return _game_state.score_white;
}

float GoGame::getScoreWhiteBonus() const {
  return _game_state.score_bonus_white;
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

  _game_state.setSpace(x, y, getPlayersStone());
  _game_state.chosen_x = x;
  _game_state.chosen_y = y;
  _generateNextGameState();

  if(_next_game_state.getSpace(x, y).state == empty) {
    _game_state = _original_game_state;
    return suicidal;
  }

  if(_previous_game_state.compareBoards(_next_game_state)) {
    _game_state = _original_game_state;
    return ko;
  }

  _game_state = _next_game_state;
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
  _game_state = _original_game_state;
}

void GoGame::finishGame() {
  _removeDeadChains();
  _setAllTerritoriesOwner();
  _countTerritoryPoints();
  _game_state = _next_game_state;
}

void GoGame::save(FILE *file) const {
  _previous_game_state.save(file);
  _original_game_state.save(file);
  _game_state.save(file);
}

void GoGame::load(FILE *file) {
  _previous_game_state.load(file);
  _original_game_state.load(file);
  _game_state.load(file);
}

void GoGame::exportBoard(char *str) const {
  for(int y=0; y<getBoardSize(); ++y) {
    for(int x=0; x<getBoardSize(); ++x) {
      char& c = str[y*getBoardSize() + x];
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
  str[getBoardSize()*getBoardSize()] = '\0';
}