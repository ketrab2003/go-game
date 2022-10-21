#include "go_game.h"

#include <cstdlib>

const int GoGame::_getIndexInBoard(const int x, const int y) {
  if(x < 0 || x >= getBoardWidth()) {
    return -1;
  }
  if(y < 0 || y >= getBoardHeight()) {
    return -1;
  }
  return y*getBoardWidth() + x;
}

void GoGame::_setSpace(const int x, const int y, const BoardSpace space) {
  const int index = _getIndexInBoard(x, y);
  if(index == -1) {
    return;
  }
  _board[index] = space;
}

void GoGame::_nextTurn() {
  switch (_turn_state) {
    case black:
      _turn_state = white;
    break;
    case white:
      _turn_state = black;
    break;
  }
}

void GoGame::_clearBoard() {
  for(int i=0; i<getBoardWidth()*getBoardHeight(); ++i) {
    _board[i].state = empty;
    _board[i].liberties = -1;
  }
}

GoGame::GoGame(const int board_width, const int board_height) {
  _board_width = board_width;
  _board_height = board_height;
  _board = (BoardSpace*)malloc(sizeof(BoardSpace) * board_width * board_height);
  _turn_state = black;
}

const int GoGame::getBoardWidth() {
  return _board_width;
}

const int GoGame::getBoardHeight() {
  return _board_height;
}

BoardSpace GoGame::getSpace(const int x, const int y) {
  const int index = _getIndexInBoard(x, y);
  if(index == -1) {
    return {edge, false};
  }
  return _board[index];
}

Player GoGame::getTurnState() {
  return _turn_state;
}

BoardSpaceState GoGame::getPlayersStone() {
  switch(getTurnState()) {
    case black:
      return blackStone;
    break;
    case white:
      return whiteStone;
    break;
  }
}

BoardSpaceState GoGame::getEnemysStone() {
  switch(getTurnState()) {
    case black:
      return whiteStone;
    break;
    case white:
      return blackStone;
    break;
  }
}

MoveResult GoGame::placeStone(const int x, const int y) {
  if(getSpace(x, y).state != empty) {
    return occupied;
  }
  _setSpace(x, y, {getPlayersStone(), -1});
  if(countLiberties(x, y) == 0) {
    _setSpace(x, y, {empty, -1});
    return suicidal;
  }
  _nextTurn();
  return done;
}



GoGame::~GoGame() {
  free(_board);
}