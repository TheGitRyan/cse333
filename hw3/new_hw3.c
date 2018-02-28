#include "model.h"
#include "libs/array2d.h"
#include "view.h"
#include <glib-object.h>
#include <glib/gstdio.h>



static void
open (GApplication *application, GFile **files, gint n_files, const char *hint, gpointer model)
{
  gint i;
  printf("%d\n", *(int*)model);
 

  for (i = 0; i < n_files; i++)
    {
      gchar *uri = g_file_get_uri (files[i]);
      g_print ("open %s\n", uri);
      g_free (uri);
    }

}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status;

  if (argc != 2) {
        printf("must provide an input file.\n");
        return 1;
  }


  /* initialize a new model using filename passed in*/
  //FILE* f = g_fopen(argv[1], "r");
  //Model model = model_new(f);

  int i = 12;

  printf("%d\n", i);

  /*
   * If you supply NULL to gtk_application_new you can run more than one instance
   * of the app at a time.  If you supply a correctly formated app name
   * (like "edu.washington.cs.cse333.ex04"), gtk+ will ensure that only a single
   * process is running, with a new main window being created for each invocation
   */
  app = gtk_application_new (NULL, G_APPLICATION_HANDLES_OPEN);
  g_signal_connect (app, "open", G_CALLBACK (open), &i);

  /*
   * Control is given to the gtk+ event loop.  It doesn't return until
   * the app is terminating.
   */
  printf("before app run\n");
  status = g_application_run (G_APPLICATION (app), argc, argv);
  printf("after app run\n");
  g_object_unref (app);

  return status;
}
