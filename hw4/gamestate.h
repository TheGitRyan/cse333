#ifndef __GAMESTATE_H_
#define __GAMESTATE_H_

#ifdef __cplusplus
extern "C" {
#endif
	#include "libs/array2d.h"
	#include "model.h"
	#include "modelCallbacks.h"
#ifdef __cplusplus
}
#endif


class GameState {
private:
	Array2d boardState;
public:
	int score;
	~GameState() {
		Array2d_destroy(boardState, modelCallback_destroyElement);
	}
	void setBoard(Array2d board);
	Array2d getBoard();
	
	bool decr(int i, int j);
	int decrAllSpots(Model m);
	bool isComplete();
};


#endif
