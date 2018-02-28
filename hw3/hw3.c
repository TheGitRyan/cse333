//c file with all of the implementatino for the model and the view, will almost definitely be separated into 2 c files, with a third defining the "action" enum

#include "model.h"
#include "libs/array2d.h"
#include "view.h"
#include <glib-object.h>
#include <glib/gstdio.h>
#include <string.h>

int GLOBAL_I = -1;
int GLOBAL_J = -1;

int left_offset;
int num_moves = 0;

static void update_label(GtkWidget *label) {
  num_moves++;
  gchar *text = gtk_label_get_text(GTK_LABEL (label));
  int len_num = (num_moves / 10) + 1;
  char buf[12 + len_num];

  sprintf(buf, "%d moves made", num_moves);
  gtk_label_set_text(GTK_LABEL (label), buf);
}

static void swapCandy(Model model, Action action, GtkGrid *grid) {
	int num_rows = Array2d_rows(model->image_vals);
	Update *updates = model_execute_action(model, action);
	detoggle(grid, GLOBAL_J, GLOBAL_I);
	view_refresh_changed(grid, model, updates, 2);
	GtkWidget *label = gtk_grid_get_child_at(grid, left_offset, 0);
	update_label(label);
}


static void swapCandyUp(GtkWidget *button, gpointer model) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    int num_rows  = Array2d_rows(((Model)model)->image_vals);
    Action action = action_new(swap_down, (num_rows-1) - GLOBAL_I, GLOBAL_J); 
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Model)model, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyDown(GtkWidget *button, gpointer model) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    int num_rows  = Array2d_rows(((Model)model)->image_vals);
    Action action = action_new(swap_up, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Model)model, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyLeft(GtkWidget *button, gpointer model) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    int num_rows  = Array2d_rows(((Model)model)->image_vals);
    Action action = action_new(swap_left, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Model)model, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyRight(GtkWidget *button, gpointer model) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    int num_rows  = Array2d_rows(((Model)model)->image_vals);
    Action action = action_new(swap_right, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Model)model, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void open (GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer model)
{
  GtkWidget *window, *grid, *label; 

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title(GTK_WINDOW (window), "hw3");
  gtk_container_set_border_width(GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new();
  /* Pack the container in the window */
  gtk_container_add(GTK_CONTAINER (window), grid);
 
  view_refresh(grid, (Model) model);
 
  /*
   * Set up the side window buttons in order to make swaps operate
   */
  
  left_offset = Array2d_columns(((Model)model)->image_vals);
  label = gtk_label_new("0 moves made");
  
  /* add the control buttons */
  GtkWidget *imageUp = gtk_image_new_from_file("./images/direction/up.png"); 
  GtkWidget *imageDown = gtk_image_new_from_file("./images/direction/down.png");
  GtkWidget *imageRight = gtk_image_new_from_file("./images/direction/right.png");
  GtkWidget *imageLeft = gtk_image_new_from_file("./images/direction/left.png");

  GtkWidget *buttonUp = gtk_button_new();
  GtkWidget *buttonDown = gtk_button_new();
  GtkWidget *buttonRight = gtk_button_new();
  GtkWidget *buttonLeft = gtk_button_new();

  gtk_button_set_image(GTK_BUTTON (buttonUp), imageUp);
  gtk_button_set_image(GTK_BUTTON (buttonDown), imageDown);
  gtk_button_set_image(GTK_BUTTON (buttonRight), imageRight);
  gtk_button_set_image(GTK_BUTTON (buttonLeft), imageLeft);
 
  g_signal_connect(buttonUp, "clicked", G_CALLBACK (swapCandyUp), (Model)(model));
  g_signal_connect(buttonDown, "clicked", G_CALLBACK (swapCandyDown), (Model)(model));
  g_signal_connect(buttonLeft, "clicked", G_CALLBACK (swapCandyLeft), (Model)(model));
  g_signal_connect(buttonRight, "clicked", G_CALLBACK (swapCandyRight), (Model)(model));
  
  gtk_grid_attach(GTK_GRID (grid), label, left_offset, 0, 2, 1);
  
  gtk_grid_attach(GTK_GRID (grid), buttonLeft, left_offset, 1, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonRight, left_offset, 2, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonUp, left_offset, 3, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonDown, left_offset, 4, 2, 1);

  /* Now that we are done packing our widgets, we show them all
   * in one go, by calling gtk_widget_show_all() on the window.
   * This call recursively calls gtk_widget_show() on all widgets
   * that are contained in the window, directly or indirectly.
   */
  gtk_widget_show_all (window);
}


int main (int argc, char *argv[])
{
  GtkApplication *app;
  int status;
   
  if (argc != 2) {
	printf("must provide an input file.\n");
	return 1;
  }
  

  /* initialize a new model using filename passed in*/
  FILE* f = g_fopen(argv[1], "r");
  Model model = model_new(f);

  /* 
   * If you supply NULL to gtk_application_new you can run more than one instance
   * of the app at a time.  If you supply a correctly formated app name
   * (like "edu.washington.cs.cse333.ex04"), gtk+ will ensure that only a single
   * process is running, with a new main window being created for each invocation            
  */
  app = gtk_application_new (NULL, G_APPLICATION_HANDLES_OPEN);
  g_signal_connect (app, "open", G_CALLBACK (open), model);

  /*
   * Control is given to the gtk+ event loop.  It doesn't return until
   * the app is terminating.
  */
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  model_destroy(model);
  return status;
}

