//implementation of the view methods

#include "view.h"
#include "model.h"

void detoggle (GtkGrid *grid, int left, int top) {
	if(left >= 0 && top >= 0) {
		GtkWidget *button = gtk_grid_get_child_at(grid, left, top);
		GLOBAL_I = -1;
		GLOBAL_J = -1;
        	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button), FALSE);	
	}	
}

void toggleCoordinates(GtkWidget *button) {

  GtkContainer *grid = GTK_CONTAINER (gtk_widget_get_parent(button));

  detoggle(GTK_GRID (grid), GLOBAL_J, GLOBAL_I);

  GValue left = G_VALUE_INIT;
  GValue top = G_VALUE_INIT;

  g_value_init(&left, G_TYPE_INT);
  g_value_init(&top, G_TYPE_INT);

  gtk_container_child_get_property(grid, button, "left-attach", &left);
  gtk_container_child_get_property(grid, button, "top-attach", &top);

  GLOBAL_I = g_value_get_int(&top);
  GLOBAL_J = g_value_get_int(&left);

}

/* maps the file idx to the actual file name */
char *fileName(int fileIdx) {
        switch (fileIdx) {
                case 0:
                        return "images/40x40/blue.png";
                case 1:
                        return "images/40x40/green.png";
                case 2:
                        return "images/40x40/orange.png";
                case 3:
                        return "images/40x40/purple.png";
                case 4:
                        return "images/40x40/red.png";
                case 5:
                        return "images/40x40/yellow.png";
                default:
                        break;
        }
	return (char*)-1;
}

/* refresh the updated cells in the grid */
void view_refresh_changed(GtkWidget *grid, Model model, Update *updates, int num_updates){
	if (updates == NULL) { return; }

	for (int k = 0; k < num_updates; k++) {
		Update update = (Update)updates[k];
		int i = update->i;
		int j = update->j;
		int image_val = update->image_val;


		int num_rows = Array2d_rows(model->image_vals);		
		GtkWidget *cell = gtk_grid_get_child_at (GTK_GRID (grid), j, (num_rows - 1) - i);
                if (cell != NULL) {
                        gtk_container_remove (GTK_CONTAINER (grid), cell);
                }

                /* create toggle button and init with image */
                GtkWidget *imageButton = gtk_toggle_button_new();
                GtkWidget *image = gtk_image_new_from_file(fileName(image_val));
                gtk_button_set_image(GTK_BUTTON (imageButton), image);
                gtk_button_set_relief(GTK_BUTTON (imageButton), GTK_RELIEF_NONE);

                /*set up the button to load it's coordinates when toggled on and clean them when toggled off*/
                g_signal_connect(imageButton, "toggled", G_CALLBACK (toggleCoordinates), NULL);

                gtk_grid_attach(GTK_GRID (grid), imageButton, j, (num_rows-1) - i, 1, 1);
                gtk_widget_show(imageButton);
	}
	
	for (int i = 0; i < num_updates; i++) {
		update_destroy(*(updates + i));
	}
	free(updates);
	updates = NULL;
}
	


/* refresh the grid widget with the vals from the model */
void view_refresh(GtkWidget *grid, Model model){
  int num_rows = Array2d_rows(model->image_vals);
  int num_cols = Array2d_columns(model->image_vals);
  
  for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
  		GtkWidget *cell = gtk_grid_get_child_at (GTK_GRID (grid), j, (num_rows - 1) - i);
	 	if (cell != NULL) {
			gtk_container_remove (GTK_CONTAINER (grid), cell);
		}

                /* create toggle button and init with image */
                GtkWidget *imageButton = gtk_toggle_button_new();
                int image_val = Array2d_get(model->image_vals, i, j);

                GtkWidget *image = gtk_image_new_from_file(fileName(image_val));
                gtk_button_set_image(GTK_BUTTON (imageButton), image);
		gtk_button_set_relief(GTK_BUTTON (imageButton), GTK_RELIEF_NONE);		

		/*set up the button to load it's coordinates when toggled on and clean them when toggled off*/
                g_signal_connect(imageButton, "toggled", G_CALLBACK (toggleCoordinates), NULL);

                gtk_grid_attach(GTK_GRID (grid), imageButton, j, (num_rows-1) - i, 1, 1);
		gtk_widget_show(imageButton);
        }
  }
}
