//this will be the implementation of our underlying model

#include <stdlib.h>
#include "model.h"
#include "libs/array2d.h"
#include "modelCallbacks.h"

//creates a new model
Model model_new(FILE* in) {
	Model model = (Model)malloc(sizeof(Model_s));
	if(model == NULL) {
		return (Model)NULL;
	}
	model->image_vals = Array2d_deserialize(in, modelCallback_deserializeElement);

	return model;
}

//serialize the model by appealing to the 
void model_serialize(Model model, FILE* out) {
	Array2d_serialize(model->image_vals, out, modelCallback_serializeElement);
}

//destroys the given model
void model_destroy(Model model) {
	//destroy the array inside, then the struct itself and clean the pointer
	Array2d_destroy(model->image_vals, modelCallback_destroyElement);
	free(model);
	model = NULL;
}


//malloc and return a new model
Update update_new(int i, int j, int image_val) {
	Update update = (Update)malloc(sizeof(Update_s));
	if (!update) {
		return;
	}
	update->i = i;
	update->j = j;
	update->image_val = image_val;
	return update;
}

//free a malloc'd update struct
void update_destroy(Update update) {
	free(update);
	update = NULL;
}

//execute query by interpreting the action type and calling associated method
Update* model_execute_action(Model m, Action a) {
	switch (a->type){
		case swap_left:
			return model_swap_left(m, a->i, a->j);
		case swap_right:
			return model_swap_right(m, a->i, a->j);
		case swap_up:
			return model_swap_up(m, a->i, a->j);
		case swap_down:
			return model_swap_down(m, a->i, a->j);
		default:
			return NULL;
		
	}
	action_destroy(a);
}

//swap (i,j)th element with the (i, j-1)th element
Update* model_swap_up(Model m, int i, int j) {
	int num_rows = Array2d_rows(m->image_vals);
	int num_cols = Array2d_columns(m->image_vals);
	if (0 <= i - 1 && i < num_rows && 0 <= j && j < num_cols){
		Array2d_swap(m->image_vals, i, j, i-1, j);

		Update *updates = (Update*)malloc(2 * sizeof(Update));
                updates[0] = update_new(i, j, Array2d_get(m->image_vals,i,j));
                updates[1] = update_new(i-1, j, Array2d_get(m->image_vals,i-1,j));
                return updates;
	}
	return NULL;
}

//swap (i,j)th element with the (i, j+1)th element
Update* model_swap_down(Model m, int i, int j) {
        int num_rows = Array2d_rows(m->image_vals);
        int num_cols = Array2d_columns(m->image_vals);
        if (0 <= i && i+1 < num_rows && 0 <= j && j < num_cols){
                Array2d_swap(m->image_vals, i, j, i+1, j);

		Update *updates = (Update*)malloc(2 * sizeof(Update));
                updates[0] = update_new(i, j, Array2d_get(m->image_vals,i,j));
                updates[1] = update_new(i+1, j, Array2d_get(m->image_vals,i+1,j));
                return updates;
        }
	return NULL;
}

//swap (i,j)th element with (i+1,j)th element
Update* model_swap_right(Model m, int i, int j) {
        int num_rows = Array2d_rows(m->image_vals);
        int num_cols = Array2d_columns(m->image_vals);
        if (0 <= i && i < num_rows && 0 <= j && j+1 < num_cols){
                Array2d_swap(m->image_vals, i, j, i, j+1);

		Update *updates = (Update*)malloc(2 * sizeof(Update));
                updates[0] = update_new(i, j, Array2d_get(m->image_vals, i,j));
                updates[1] = update_new(i, j+1, Array2d_get(m->image_vals,i,j+1));
                return updates;
        }
	return NULL;
}

//swap (i,j)th element with (i-1,j)th element
Update* model_swap_left(Model m, int i, int j) {
        int num_rows = Array2d_rows(m->image_vals);
        int num_cols = Array2d_columns(m->image_vals);
        if (0 <= i && i < num_rows && 0 <= j - 1 && j < num_cols){
                Array2d_swap(m->image_vals, i, j, i, j-1);
		
		Update *updates = (Update*)malloc(2 * sizeof(Update));
		updates[0] = update_new(i, j, Array2d_get(m->image_vals, i,j));
		updates[1] = update_new(i, j-1, Array2d_get(m->image_vals,i,j-1));
		return updates;
        }
	return NULL;
}


