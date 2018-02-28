#include "action.h"
#include <stdlib.h>

//create a new action tuple
Action action_new(action_type type, int i, int j){
	Action action = (Action)malloc(sizeof(Action_s));
	action->type = type;
	action->i = i;
	action->j = j;
	return action;
}

//destroy the given action tuple
void action_destroy(Action action) {
	free(action);
	action = NULL;
}

