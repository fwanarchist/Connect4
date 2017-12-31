
#include "Game.h"
#include <iostream>
#include <Windows.h>

int main() {
	GameState * game = new GameState();
	int move;
	for (int i = 0; i < 42; i++) {
		printBoard(game->board);
		printf("Enter your move (0-6): ");
		while (true) {
			scanf_s("%d", &move);
			if (move < 7 || move >= 0) {
				if (game->board->b[move][5] == EMPTY) {
					break;
				}
			}
			printf("Invalid Move.\n");
			printf("Enter your move (0-6): ");
		}
		game = new GameState(game, move);
		classify(game, 5);
		for (int j = 0; j < 7; j++) {
			std::cout << classString(game->children[j]->boardClass);
		}
		std::cout << std::endl;

		
		int bestMove = 3;
		if (game->children[bestMove]->boardClass == BAD_MOVE || game->children[bestMove]->boardClass == RED_WIN) {
			for (int j = 0; j < 7; j++) {
				if (game->children[j]->boardClass != BAD_MOVE && game->children[j]->boardClass != RED_WIN) {
					bestMove = j;
					break;
				}
			}
			if (bestMove == 3) {
				for (int j = 0; j < 7; j++) {
					if (game->children[j]->boardClass != BAD_MOVE) {
						bestMove = j;
						break;
					}
				}
			}
		}
		for (int j = 0; j < 7; j++) {
			if (game->children[j]->boardClass == BLACK_WIN) {
				bestMove = j;
				break;
			}
			else if (game->children[j]->blackWins > game->children[bestMove]->blackWins &&
				game->children[j]->boardClass != BAD_MOVE &&
				game->children[j]->boardClass != RED_WIN) {
				bestMove = j;
			}
		}
		if (game->children[bestMove]->blackWins == 0 && game->children[bestMove]->boardClass != BLACK_WIN) {
			for (int j = 0; j < 7; j++) {
				if (game->children[j]->redWins < game->children[bestMove]->redWins &&
					game->children[j]->boardClass != BAD_MOVE &&
					game->children[j]->boardClass != RED_WIN) {
					bestMove = j;
				}
			}
		}
		game = game->parent;
		delete game->children[move];
		game = new GameState(game, move);
		game = new GameState(game, bestMove);
	}
}