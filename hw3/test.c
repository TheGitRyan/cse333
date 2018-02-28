#include "modelCallbacks.h"
#include "model.h"
#include "action.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("please provide an input and output file.\n");
		return 1;
	}
	

	FILE* in = fopen(argv[1], "r");
	FILE* out = fopen(argv[2], "a+");
	Model model = model_new(in);

	Action a_swap_right = action_new(swap_right, 1, 1);
	model_execute_action(model, a_swap_right);
	model_serialize(model, out);
	fprintf(out, "\n");

	Action a_swap_left = action_new(swap_left, 1, 1);
        model_execute_action(model, a_swap_left);
	model_serialize(model, out);
	fprintf(out, "\n");
	
	Action a_swap_down = action_new(swap_down, 1, 1);
	model_execute_action(model, a_swap_down);
	model_serialize(model, out);
	fprintf(out, "\n");
	
	Action a_swap_up = action_new(swap_up, 1,1);
	model_execute_action(model, a_swap_up);
	model_serialize(model, out);	
	fprintf(out, "\n");

	fclose(out);
	fclose(in);
	model_destroy(model);
	return 0;
}
