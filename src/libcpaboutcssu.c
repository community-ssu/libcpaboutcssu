#include <hildon-cp-plugin/hildon-cp-plugin-interface.h>
#include <hildon/hildon.h>
#include <gtk/gtk.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include <gdk/gdkx.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define CSSU_LIST_FILE "/var/lib/dpkg/info/mp-fremantle-community-pr.list"
#define IMAGE "/usr/share/pixmaps/maemo.org.png"

osso_return_t execute(osso_context_t* osso, gpointer user_data, gboolean user_activated)
{
	GtkWidget     *dialog      = gtk_dialog_new();
	GtkWidget     *pan         = hildon_pannable_area_new();
	GdkPixbuf     *image       = gdk_pixbuf_new_from_file(IMAGE, NULL);
	GtkWidget     *text_view   = hildon_text_view_new();
	GtkWidget     *box         = gtk_vbox_new(FALSE, 0);
	GtkTextBuffer *buffer      = gtk_text_buffer_new(NULL);

	if (g_file_test (CSSU_LIST_FILE, G_FILE_TEST_EXISTS)) {
		gchar *version = NULL;
		GError *err = NULL;

		if (!g_spawn_command_line_sync ("sh -c \"dpkg -l mp-fremantle-community-pr | tail -1 | awk '{print $3}'\"", &version, NULL, NULL /* Always 0 :\ */, &err))
		{
			g_printerr ("Spawning dpkg failed: %s\n", err->message);
			version = g_strdup ("Version could not be determined");
			g_error_free (err);
		}
		g_strstrip (version);

		const char *flavor;
		gchar *p = NULL;
		if ((p = strchr(version, 'S')) != NULL) {
			flavor = "Stable";
		} else if ((p = strchr(version, 'T')) != NULL) {
			flavor = "Testing";
		} else {
			flavor = "Unknown";
		}

		gtk_text_buffer_insert_at_cursor (buffer, "\n\nCommunity SSU installed\n", -1);
		gtk_text_buffer_insert_at_cursor (buffer, "Version: ", -1);
		gtk_text_buffer_insert_at_cursor (buffer, version, -1);
		gtk_text_buffer_insert_at_cursor (buffer, "\nFlavor: ", -1);
		gtk_text_buffer_insert_at_cursor (buffer, flavor, -1);
		gtk_text_buffer_insert_at_cursor (buffer, "\n\nThanks to:\nAndrew Flegg\nThomas Perl\nNicolai Hess\nNiels Breet\nAndre Klapper\nChristian Ratzenhofer\nFaheem Pervez\nRoman Morav‌čík\nTomasz Pieniążek\nAndrew Zhilin\nIvaylo Dimitrov\nPali Rohár\nJavier S. Pedro\n", -1);
		gtk_text_buffer_insert_at_cursor (buffer, "And everyone who was involved.\n\n ~ Mohammad Abu-Garbeyyeh", -1);

		g_free (version);
	} else {
		gtk_text_buffer_insert_at_cursor (buffer, "\nCommunity SSU meta package not installed", -1);
	}

	gtk_text_view_set_editable (GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW(text_view), FALSE);
	GtkTextIter iter;
	gtk_text_buffer_get_start_iter (buffer, &iter);
	
	if (image)
	{
		gtk_text_buffer_insert_pixbuf(buffer, &iter, image);
		g_object_unref(image);
	}

	hildon_text_view_set_buffer (HILDON_TEXT_VIEW(text_view), GTK_TEXT_BUFFER(buffer));

	g_object_unref(buffer);

	hildon_pannable_area_add_with_viewport (HILDON_PANNABLE_AREA(pan), text_view);

	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), pan, TRUE, TRUE, 0);

	gtk_window_set_title (GTK_WINDOW(dialog), "About Maemo 5 Community SSU");
	gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(user_data));

	GdkGeometry geometry;
	if (gdk_screen_get_width(gdk_display_get_default_screen(gdk_display_get_default())) < 800) 
	    geometry.min_height = 500; 
	else 
	    geometry.min_height = 360;
	
	geometry.min_width = 480; 
	gtk_window_set_geometry_hints (GTK_WINDOW(dialog), 
				       dialog, 
				       &geometry, 
				       GDK_HINT_MIN_SIZE); 

	gtk_widget_show_all (dialog);
	gtk_dialog_run (GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);

	return OSSO_OK;
}

osso_return_t save_state(osso_context_t* osso, gpointer user_data)
{
	return OSSO_OK;
}
