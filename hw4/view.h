//header file for the view portion 
#ifndef _VIEW_H_
#define _VIEW_H_

#include "libs/array2d.h"
#include <gtk/gtk.h>
#include "model.h"

extern int GLOBAL_I;
extern int GLOBAL_J;

/*
 * refreshes the grid container with the items in the model updating all images to match 
 * the values stored in the model array
 */
void view_refresh(GtkWidget *grid, Model model);

/*
 * refreshes the grid with an array of coordinates which need to be updated
 */
void view_refresh_changed(GtkWidget *grid, Model model, Update updates[], int num_updates);

/* converts the file index in the array to the image file path */
char *fileName(int fileIdx);

void toggleCoordinate(GtkWidget *button);

void detoggle(GtkGrid *grid, int left, int top);

#endif
