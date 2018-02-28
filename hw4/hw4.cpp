//cpp file with all of the implementatino for the model and the view, will almost definitely be separated into 2 c files, with a third defining the "action" enum

#include "game.h"
#include "gamestate.h"
#include "extensionboard.h"

#ifdef __cplusplus
extern "C" {
#endif

	#include "model.h"
	#include "libs/array2d.h"
	#include "view.h"
	#include <glib-object.h>
	#include <glib/gstdio.h>
	#include <string.h>

#ifdef __cplusplus
}
#endif

int GLOBAL_I = -1;
int GLOBAL_J = -1;

int left_offset;

static void update_label(GtkWidget *label, Game* game) {
  game->movesmade++;
  
  //const gchar *text = gtk_label_get_text(GTK_LABEL (label));
  int len_num = (game->movesmade / 10) + 1;
  char buf[12 + len_num];

  sprintf(buf, "%d moves made", game->movesmade);
  gtk_label_set_text(GTK_LABEL (label), buf);
}

static void update_score(GtkWidget *score_label, Game* game) {
	int len_num = (game->currentscore / 10) + 1;
	char buf[8 + len_num];

	sprintf(buf, "Score: %d", game->currentscore);
	gtk_label_set_text(GTK_LABEL (score_label), buf);
}

static void swapCandy(Game* game, Action action, GtkGrid *grid) {
	GtkWidget *label = gtk_grid_get_child_at(grid, left_offset, 0);
        GtkWidget *score_label = gtk_grid_get_child_at(grid, left_offset, 1);

	Model model = game->model;
	ExtensionBoard *pExtBoard = game->extensionBoard;
	GameState *gameState = game->gameState;

	bool matched = model_execute_action(model, action);
	detoggle(grid, GLOBAL_J, GLOBAL_I);
	if (matched) {
                ((Game*)game)->currentscore += gameState->decrAllSpots(model);
                pExtBoard->swap_all_empty(model);
                pExtBoard->fill_empty(model);
		update_label(label, game);
        }	

	int iter_limit = 1000;
	int iter_count = 0;
	while(matched && iter_count < iter_limit) {
		iter_count++;
		matched = has_match(model);
		detoggle(grid, GLOBAL_J, GLOBAL_I);
		if (matched) {
			((Game*)game)->currentscore += gameState->decrAllSpots(model);
			pExtBoard->swap_all_empty(model);
			pExtBoard->fill_empty(model);
		}
	}
	
	view_refresh(GTK_WIDGET (grid), model);
	update_score(score_label, game);
}


static void swapCandyUp(GtkWidget *button, gpointer game) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    Model model = ((Game*)game)->model;
    int num_rows  = Array2d_rows(model->image_vals);
    Action action = action_new(swap_down, (num_rows-1) - GLOBAL_I, GLOBAL_J); 
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Game*)game, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyDown(GtkWidget *button, gpointer game) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    Model model = ((Game*)game)->model;
    int num_rows  = Array2d_rows(model->image_vals);
    Action action = action_new(swap_up, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Game*)game, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyLeft(GtkWidget *button, gpointer game) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    Model model = ((Game*)game)->model;
    int num_rows  = Array2d_rows(model->image_vals);
    Action action = action_new(swap_left, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Game*)game, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static void swapCandyRight(GtkWidget *button, gpointer game) {
  if (GLOBAL_I >= 0 && GLOBAL_J >= 0) {
    Model model = ((Game*)game)->model;
    int num_rows  = Array2d_rows(model->image_vals);
    Action action = action_new(swap_right, (num_rows-1) - GLOBAL_I, GLOBAL_J);
    GtkWidget *grid = gtk_widget_get_parent(button);
    swapCandy((Game*)game, action, GTK_GRID (grid));
  } else {
	printf("no candy selected.\n");
  }
}

static gboolean serializeGame (GtkWidget *widget, GdkEvent *event, gpointer game) {
	FILE* f = fopen("test.out", "w");
	((Game*)game)->serialize(f);
	return FALSE;
}

static void open (GtkApplication *app, GFile **files, gint n_files, const gchar *hint, gpointer game)
{
  GtkWidget *window, *grid, *label, *score_label; 
  Model model = ((Game*)game)->model;
  ExtensionBoard *pExtBoard = ((Game*)game)->extensionBoard;
  GameState *gameState = ((Game*)game)->gameState;

  bool matched;
  int iter_limit = 1000;
  int iter_count = 0;
  do {
        iter_count++;
        matched = has_match(model);
        if (matched) {
                ((Game*)game)->currentscore += gameState->decrAllSpots(model);
                pExtBoard->swap_all_empty(model);
                pExtBoard->fill_empty(model);
        }
  } while(matched && iter_count < iter_limit);

  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  g_signal_connect (window, "delete-event", G_CALLBACK (serializeGame), game);
  gtk_window_set_title(GTK_WINDOW (window), "hw4");
  gtk_container_set_border_width(GTK_CONTAINER (window), 10);

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new();
  /* Pack the container in the window */
  gtk_container_add(GTK_CONTAINER (window), grid);
  view_refresh(grid, model);
 
  /*
   * Set up the side window buttons in order to make swaps operate
   */
  
  left_offset = Array2d_columns(model->image_vals);

  int len_num_moves = (((Game*)game)->movesmade / 10) + 1;
  char moves_buf[12 + len_num_moves];
  sprintf(moves_buf, "%d moves made", ((Game*)game)->movesmade);
  
  int len_num_score = (((Game*)game)->currentscore / 10) + 1;
  char score_buf[8 + len_num_score];
  sprintf(score_buf, "Score: %d", ((Game*)game)->currentscore);


  label = gtk_label_new(moves_buf);
  score_label = gtk_label_new(score_buf);
  
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
 
  g_signal_connect(buttonUp, "clicked", G_CALLBACK (swapCandyUp), game);
  g_signal_connect(buttonDown, "clicked", G_CALLBACK (swapCandyDown), game);
  g_signal_connect(buttonLeft, "clicked", G_CALLBACK (swapCandyLeft), game);
  g_signal_connect(buttonRight, "clicked", G_CALLBACK (swapCandyRight), game);
  
  gtk_grid_attach(GTK_GRID (grid), label, left_offset, 0, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), score_label, left_offset, 1, 2, 1);
  
  gtk_grid_attach(GTK_GRID (grid), buttonLeft, left_offset, 2, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonRight, left_offset, 3, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonUp, left_offset, 4, 2, 1);
  gtk_grid_attach(GTK_GRID (grid), buttonDown, left_offset, 5, 2, 1);

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

  Game *game = new Game();
  game->deserialize(f);
  

  /* 
   * If you supply NULL to gtk_application_new you can run more than one instance
   * of the app at a time.  If you supply a correctly formated app name
   * (like "edu.washington.cs.cse333.ex04"), gtk+ will ensure that only a single
   * process is running, with a new main window being created for each invocation            
  */
  app = gtk_application_new (NULL, G_APPLICATION_HANDLES_OPEN);
  g_signal_connect (app, "open", G_CALLBACK (open), game);

  /*
   * Control is given to the gtk+ event loop.  It doesn't return until
   * the app is terminating.
  */
  status = g_application_run (G_APPLICATION (app), argc, argv);
  delete game;
  g_object_unref (app);
  return status;
}

