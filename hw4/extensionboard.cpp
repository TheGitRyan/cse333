#include "extensionboard.h"
#include <stdint.h>

void ExtensionBoard::setBoard(Array2d board, Array2d offsets) {
	this->board = board;

	if (offsets) {
		this->offsets = offsets;
	} else {
		int num_cols = board->cols;
		Array2d empty_offsets = (Array2d)malloc(sizeof(Array2d_s));
		empty_offsets->rows = 1;
		empty_offsets->cols = num_cols;
		empty_offsets->pData = (Array2d_value_t*)malloc(sizeof(Array2d_value_t) * num_cols);
		for (int i = 0; i < num_cols; i++) {
			*(empty_offsets->pData + i) = (Array2d_value_t)0;
		}
		this->offsets = empty_offsets;
	}
}

Array2d ExtensionBoard::getBoard(){
	return this->board;
}

Array2d ExtensionBoard::getOffsets() {
	return this->offsets;
}

int *ExtensionBoard::get(int j, int n) {
	int *candies = (int*)malloc(sizeof(int) * n);
	int num_rows = Array2d_rows(this->board);
	int i = (intptr_t)Array2d_get(this->offsets, 0, j);

	for (int k = 0; k < n; k++) {
		int val = (intptr_t)Array2d_get(this->board, i, j);
		*(candies + k) = val;
		
		i = (i+1) % num_rows;
	}
	Array2d_set(this->offsets, 0, j, (Array2d_value_t)i);
	return candies;
}

void ExtensionBoard::__swap_up(Model m, int i, int j) {
        int a = (intptr_t)Array2d_get(m->image_vals, i, j);
        int b = (intptr_t)Array2d_get(m->image_vals, i + 1, j);
        Array2d_set(m->image_vals, i, j, (Array2d_value_t)b);
        Array2d_set(m->image_vals, i + 1, j, (Array2d_value_t)a);
}

void ExtensionBoard::swap_to_top(Model m, int i, int j) {
        while (i + 1 < m->image_vals->rows) {
                __swap_up(m, i, j);
                i++;
        }
}

void ExtensionBoard::swap_all_empty(Model m) {
	Array2d arr = m->image_vals;
	int num_rows = arr->rows;
	int num_cols = arr->cols;

	for (int i = num_rows-1; i >= 0; i--) {
		for (int j = 0; j < num_cols; j++) {
			if (((intptr_t)Array2d_get(arr, i, j)) == -1) {
				this->swap_to_top(m, i, j);	
			}		
		}
	}
}


void ExtensionBoard::fill_empty(Model m) {
	Array2d arr = m->image_vals;
	int num_rows = arr->rows;
	int num_cols = arr->cols;

	for (int j = 0; j < num_cols; j++) {
		int i = num_rows - 1;
		int count = 0;
		while (((intptr_t)Array2d_get(arr, i, j)) == -1 && i >= 0) {
			count++;
			i--;
		}
		i++;

		int *new_vals = this->get(j, count);

		int k = 0;
		while(i < num_rows) {
			int val = *(new_vals + k);
			Array2d_set(arr, i, j, (Array2d_value_t)val);
			k++;
			i++;
		}
	}
}

