#ifndef __E_BOARD__
#define __E_BOARD__

#ifdef __cplusplus
extern "C" {
#endif
	#include "libs/array2d.h"
	#include "modelCallbacks.h"
	#include "model.h"
#ifdef __cplusplus
}
#endif

class ExtensionBoard {
private:
	//swaps the value at (i,j) up one spot
	void __swap_up(Model m, int i, int j);
	
	//swap the given value from current row upn to the 0th row
	void swap_to_top(Model m, int i, int j);
public:
	Array2d board;
	Array2d offsets;
	~ExtensionBoard() {
		Array2d_destroy(board, modelCallback_destroyElement);
	}
	Array2d getBoard();
	Array2d getOffsets();

	int *get(int i, int num);
	void setBoard(Array2d board, Array2d offsets);
	void swap_all_empty(Model m);
	void fill_empty(Model m);
};

#endif
