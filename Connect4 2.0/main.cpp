#include "Game.h"





int main() {
	int move;
	GameNode * root = new GameNode();
	GameNode * game = root;
	for (;;) {
		game->board->print();
		for (;;) {
			std::cout << "Input Move: ";
			while (!scanf_s("%d", &move));
			if (move >= 0 && move < 7) {
				if (game->board->data[move][5] == pEmpty) {
					break;
				}
			}
		}
		game = new GameNode(game, move);
		if (game->board->scanWin() % 3) {
			game->board->print();
			std::cout << "You Win!!\n";
			root->deleteChildren();
			game = root;
		}
		game = new GameNode(game, BestMove(game, 6));
		if (game->board->scanWin() % 3) {
			game->board->print();
			std::cout << "You Lose!!\n";
			root->deleteChildren();
			game = root;
		}
	}
	return 0;
}