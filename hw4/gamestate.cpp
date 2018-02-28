#include "gamestate.h"
#include <stdint.h>

void GameState::setBoard(Array2d board) {
	this->boardState = board;
	Array2d_get(this->boardState, 0, 0);
}

Array2d GameState::getBoard() {
	return this->boardState;
}

bool GameState::decr(int i, int j) {
	if (i >= 0 && i < Array2d_rows(this->boardState) && j >= 0 && j < Array2d_columns(this->boardState)) {
		int value = (intptr_t)(Array2d_get(this->boardState, i, j));
		if (value > 0) {
			int result = value - 1;
			Array2d_set(this->boardState, i, j, (Array2d_value_t)result);
			score++;
			return true;
		}
	}
	return false;
}

int GameState::decrAllSpots(Model m) {
	Array2d modelArr = m->image_vals;
	
	int rows = Array2d_rows(modelArr);
	int cols = Array2d_columns(modelArr);
	int numDecremented = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int value = (intptr_t)(Array2d_get(modelArr, i, j));
			if (value == -1) {
				bool decremented = this->decr(i, j);
				if (decremented) numDecremented++;	
			}
		}
	}
	return numDecremented;
}

bool GameState::isComplete() {
	for (int i = 0; i < Array2d_rows(this->boardState); i++) {
		for (int j = 0; j < Array2d_columns(this->boardState); j++) {
			int value = (intptr_t)(Array2d_get(this->boardState, i, j)) - 1;
			if (value > 0) return false;
		}
	}
	return true;
}
