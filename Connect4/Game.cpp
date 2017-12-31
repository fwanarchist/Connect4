#include "Game.h"

BoardClass scanWin(BoardArray b) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (b[i][j] == b[i + 1][j + 1] &&
				b[i][j] == b[i + 2][j + 2] &&
				b[i][j] == b[i + 3][j + 3] &&
				b[i][j]) {
				return (BoardClass)b[i][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j < 6; j++) {
			if (b[i][j] == b[i + 1][j - 1] &&
				b[i][j] == b[i + 2][j - 2] &&
				b[i][j] == b[i + 3][j - 3] &&
				b[i][j]) {
				return (BoardClass)b[i][j];
			}
		}
	}
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			if (b[i][j] == b[i][j + 1] &&
				b[i][j] == b[i][j + 2] &&
				b[i][j] == b[i][j + 3] &&
				b[i][j]) {
				return (BoardClass)b[i][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			if (b[i][j] == b[i + 1][j] &&
				b[i][j] == b[i + 2][j] &&
				b[i][j] == b[i + 3][j] &&
				b[i][j]) {
				return (BoardClass)b[i][j];
			}
		}
	}
	return NO_WIN;
}

void printBoard(Board * board)
{
	for (int j = 5; j >= 0; j--) {
		for (int i = 0; i < 7; i++) {
			std::cout << spaceString(board->b[i][j]);
		}
		std::cout << std::endl;
	}
}

char * spaceString(BoardSpace p)
{
	switch (p) {
	case EMPTY:
		return "0";
	case RED:
		return "R";
	case BLACK:
		return "B";
	}
	return " ";
}

char * classString(BoardClass w)
{
	switch (w) {
	case BAD_MOVE:
		return "X";
	case NO_WIN:
		return "N";
	case RED_WIN:
		return "R";
	case BLACK_WIN:
		return "B";
	}
	return " ";
}

void classify(GameState * root, int depth) {
	root->boardClass = scanWin(root->board->b);
	if (root->boardClass % 3) {
		if (root->boardClass == RED_WIN) {
			root->parent->redWins += 1;
		}
		else {
			root->parent->blackWins += 1;
		}
		return;
	}
	if (depth) {
		for (int i = 0; i < 7; i++) {
			if (!root->board->b[i][5]) {
				classify(new GameState(root, i), depth - 1);
				root->blackWins += root->children[i]->blackWins;
				root->redWins += root->children[i]->redWins;
			}
			else {
				(new GameState(root, i, false))->boardClass = BAD_MOVE;
			}
		}
		for (int i = 0; i < 7; i++) {
			if (root->player == root->children[i]->boardClass) {
				root->boardClass = root->children[i]->boardClass;
				if (depth != 5) {
					root->deleteChildren();
				}
				return;
			}
		}
		for (int i = 0; i < 7; i++) {
			if (!(root->children[i]->boardClass == BAD_MOVE || (root->children[i]->boardClass != root->player && root->children[i]->boardClass != NO_WIN))) {
				if (depth != 5) {
					root->deleteChildren();
				}
				return;
			}
		}
		if (depth != 5) {
			root->deleteChildren();
		}
		root->boardClass = root->player == RED ? BLACK_WIN : RED_WIN;
	}
}
