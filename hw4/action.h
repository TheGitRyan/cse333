//header file for the actions enum

#ifndef _ACTION_H_
#define _ACTION_H_

typedef enum {
	swap_left,
	swap_right,
	swap_up,
	swap_down
} action_type;

typedef struct {
	action_type type;
	int i;
	int j;
} Action_s, *Action;

//create a new action tuple
Action action_new(action_type type, int i, int j);

//destroy the given action tuple
void action_destroy(Action action);

#endif
