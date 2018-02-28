//header file for model implementation

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include "libs/array2d.h"
#include "modelCallbacks.h"
#include "action.h"

typedef struct {
	Array2d image_vals;
} Model_s, *Model;


//struct to store values of changes made to model so that view can do efficient updates
typedef struct {
        int i;
        int j;
        int image_val;
} Update_s, *Update;

//creates a new update struct and initializes its members
Update update_new(int i, int j, int image_val);

//frees an update struct and cleans the pointer
void update_destroy(Update update);


//Uses the given file and the modelCallback methods to read in and initialize
//a new array which is the primary data structure for the model.
Model model_new(FILE* in);

//takes in a model pointer and destroys it
void model_destroy(Model model);

//serialize hte given model using the array2d serializer methods
void model_serialize(Model model, FILE* out);

//executes an action that would be passed by the view, after being triggered by
//a listener. View waits for user input, then when it gets user input it converts
//it to an action and passes it to the model to execute.
Update* model_execute_action(Model m, Action a);

//swap element at position (i,j) with the element above it if possible.
Update*  model_swap_up(Model m, int i, int j);

//swap element at position (i,j) with the element below it if possible.
Update* model_swap_down(Model m, int i, int j);

//swap element at position (i,j) with the element to it's right if possible.
Update* model_swap_right(Model m, int i, int j);

//swap element at position (i,j) with the element to it's left if possible.
Update* model_swap_left(Model m, int i, int j);

#endif
