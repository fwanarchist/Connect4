#pragma once

#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <thread>
#include <vector>

enum Player: unsigned char {
	pEmpty,
	pRed,
	pBlack
};

enum GameState : unsigned char {
	gNoWin,
	gRedWin,
	gBlackWin,
	gInvalid
};

void printPlayer(Player player) {
	switch (player) {
	case pRed:
		printf("X");
		break;
	case pBlack:
		printf("O");
		break;
	case pEmpty:
		printf(" ");
		break;
	}
}

struct Board {
	Player data[7][6];
	Board() {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 6; j++) {
				data[i][j] = pEmpty;
			}
		}
	}
	Board(Board * parent, int move, Player player) {
		*this = *parent;
		for (int i = 0; i < 6; i++) {
			if (data[move][i] == pEmpty) {
				data[move][i] = player;
				break;
			}
		}
	}
	GameState scanWin() {
		Player temp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				temp = data[i][j];
				if (temp == data[i + 1][j + 1] &&
					temp == data[i + 2][j + 2] &&
					temp == data[i + 3][j + 3] &&
					temp) {
					return (GameState)temp;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 3; j < 6; j++) {
				temp = data[i][j];
				if (temp == data[i + 1][j - 1] &&
					temp == data[i + 2][j - 2] &&
					temp == data[i + 3][j - 3] &&
					temp) {
					return (GameState)temp;
				}
			}
		}
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 3; j++) {
				temp = data[i][j];
				if (temp == data[i][j + 1] &&
					temp == data[i][j + 2] &&
					temp == data[i][j + 3] &&
					temp) {
					return (GameState)temp;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 6; j++) {
				temp = data[i][j];
				if (temp == data[i + 1][j] &&
					temp == data[i + 2][j] &&
					temp == data[i + 3][j] &&
					temp) {
					return (GameState)temp;
				}
			}
		}
		return gNoWin;
	}
	void print() {
		for (int i = 5; i >= 0; i--) {
			for (int j = 0; j < 7; j++) {
				printf("|");
				printPlayer(data[j][i]);
			}
			std::cout << "|\n";
		}
		for (int i = 0; i < 7; i++) {
			printf(" %d", i);
		}
		printf("\n");
	}
};

struct GameNode {
	GameNode * parent;
	GameNode * children[7];
	union {
		struct {
			unsigned int redWins;
			unsigned int blackWins;
		};
		unsigned int wins[2];
	};
	GameState state;
	Board * board;
	Player player;
	GameNode() {
		parent = 0;
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		redWins = 0;
		blackWins = 0;
		state = gNoWin;
		board = new Board();
		player = pRed;
	}
	GameNode(Player _player) {
		parent = 0;
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		redWins = 0;
		blackWins = 0;
		state = gNoWin;
		board = new Board();
		player = _player;
	}
	GameNode(GameNode * _parent, int move) {
		_parent->children[move] = this;
		parent = _parent;
		for (int i = 0; i < 7; i++) {
			children[i] = 0;
		}
		redWins = 0;
		blackWins = 0;
		state = gNoWin;
		board = new Board(_parent->board, move, parent->player);
		player = parent->player == pRed ? pBlack : pRed;
	}
	void deleteChildren() {
		for (int i = 0; i < 7; i++) {
			if (children[i]) {
				delete this->children[i];
				children[i] = 0;
			}
		}
	}
	~GameNode() {
		deleteChildren();
		delete this->board;
	}
};



void checkMove(GameNode * root, int depth) {
	root->state = root->board->scanWin();
	if (root->state % 3) {
		root->wins[root->state - 1] = 1;
		return;
	}
	if (depth) {
		for (int i = 0; i < 7; i++) {
			if (!root->board->data[i][5]) {
				checkMove(new GameNode(root, i), depth - 1);
				root->blackWins += root->children[i]->blackWins;
				root->redWins += root->children[i]->redWins;
			}
			else {
				(new GameNode(root, i))->state = gInvalid;
			}
		}
		for (int i = 0; i < 7; i++) {
			if (root->player == root->children[i]->state) {
				root->state = root->children[i]->state;
				root->deleteChildren();
				return;
			}
		}
		for (int i = 0; i < 7; i++) {
			if (!(root->children[i]->state == gInvalid || (root->children[i]->state != root->player && root->children[i]->state != gNoWin))) {
				root->deleteChildren();
				return;
			}
		}
		root->state = root->player == pRed ? gBlackWin : gRedWin;
		root->deleteChildren();
	}
}

void checkMoveMulti(GameNode * root, int depth, std::thread ** threads, int index) {
	checkMove(root, depth);
	threads[index] = 0;
}

int BestMove(GameNode * game, int depth) {
	for (int i = 0; i < 7; i++) {
		if (!game->board->data[i][5]) {
			checkMove(new GameNode(game, i), depth);
		}
		else {
			(new GameNode(game, i))->state = gInvalid;
		}
	}
	int bestMove = 3;
	GameState myWin = (GameState)game->player;
	GameState opWin = game->player == pRed ? gBlackWin : gRedWin;
	if (game->children[bestMove]->state == gInvalid || game->children[bestMove]->state == opWin) {
		for (int j = 0; j < 7; j++) {
			if (game->children[j]->state != gInvalid && game->children[j]->state != opWin) {
				bestMove = j;
				break;
			}
		}
		if (bestMove == 3) {
			for (int j = 0; j < 7; j++) {
				if (game->children[j]->state != gInvalid) {
					bestMove = j;
					break;
				}
			}
		}
	}
	for (int j = 0; j < 7; j++) {
		if (game->children[j]->state == myWin) {
			game->deleteChildren();
			return j;
		}
		else if (game->children[j]->wins[myWin - 1] > game->children[bestMove]->wins[myWin - 1] &&
			game->children[j]->state != gInvalid &&
			game->children[j]->state != opWin) {
			bestMove = j;
		}
	}
	if (game->children[bestMove]->wins[myWin - 1] == 0 && game->children[bestMove]->state != myWin) {
		for (int j = 0; j < 7; j++) {
			if (game->children[j]->wins[opWin - 1] < game->children[bestMove]->wins[opWin - 1] &&
				game->children[j]->state != gInvalid &&
				game->children[j]->state != opWin) {
				bestMove = j;
			}
		}
	}
	game->deleteChildren();
	return bestMove;
}


