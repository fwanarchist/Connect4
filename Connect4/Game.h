#pragma once

#include <iostream>

typedef enum BoardClass : unsigned char {
	BAD_MOVE = 0,
	RED_WIN,
	BLACK_WIN,
	NO_WIN
};

typedef enum BoardSpace : unsigned char {
	EMPTY = 0,
	RED,
	BLACK
};

typedef BoardSpace BoardArray[7][6];

typedef struct Board {
	BoardArray b;
	Board() {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 6; j++) {
				b[i][j] = EMPTY;
			}
		}
	}
	Board(Board * _board, int move, BoardSpace player) {
		*this = *_board;
		for (int i = 0; i < 6; i++) {
			if (!b[move][i]) {
				b[move][i] = player;
				return;
			}
		}
	}
};

typedef struct GameState {
	Board * board;
	BoardSpace player;
	BoardClass boardClass;
	GameState * parent;
	GameState * children[7];
	int redWins, blackWins;
	GameState() {
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		parent = 0;
		boardClass = NO_WIN;
		player = RED;
		board = new Board;
		redWins = 0;
		blackWins = 0;
	}
	GameState(BoardSpace _player) {
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		parent = 0;
		boardClass = NO_WIN;
		player = _player;
		board = new Board;
		redWins = 0;
		blackWins = 0;
	}
	GameState(GameState * _parent, int move, bool need_board = true) {
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		
		_parent->children[move] = this;
		parent = _parent;
		boardClass = NO_WIN;
		player = _parent->player == RED ? BLACK : RED;
		if (need_board) {
			board = new Board(_parent->board, move, _parent->player);
		}
		else {
			board = 0;
		}
		redWins = 0;
		blackWins = 0;
	}
	void deleteChildren() {
		for (int i = 0; i < 7; i++) {
			if (children[i]) {
				delete children[i];
				children[i] = 0;
			}
		}
	}
	~GameState() {
		for (int i = 0; i < 7; i++) {
			if (children[i]) {
				delete children[i];
			}
		}
		if (board) {
			delete board;
		}
	}
};

void printBoard(Board * board);
char * spaceString(BoardSpace p);
char * classString(BoardClass w);
BoardClass scanWin(BoardArray b);
void classify(GameState * root, int depth);

