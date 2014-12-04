#include "app.h"
#include "callbacks.h"

const gint Sensitivity = 10;

int offsetx, offsety, px, py, maxx, maxy;

inline static int
Min (const int a, const int b)
{
    return b < a ? b : a;
}

inline static int
Max (const int a, const int b)
{
    return b > a ? b : a;
}

inline static int
RoundDownToMultiple (const int i, const int m)
{
    return i / m * m;
}

inline static int
RoundToNearestMultiple (const int i, const int m)
{
    if (i % m > (double) m / 2.0)
        return (i / m + 1) * m;
    return i / m * m;
}

static void
destroy (GtkWidget * widget, gpointer data)
{
    gtk_main_quit ();
}

static gboolean
button_press_event (GtkWidget * w, GdkEventButton * event,
                    App * app)
{
    GtkAllocation alloc_w, alloc_p;

    if (event->button == 1) {
        GtkWidget *p = gtk_widget_get_parent (w);

        gtk_widget_get_allocation (w, &alloc_w);
        gtk_widget_get_allocation (p, &alloc_p);

        // offset == distance of parent widget from edge of screen ...

        gtk_widget_translate_coordinates (w, p, event->x,
                                          event->y, &offsetx,
                                          &offsety);

        // plus distance from pointer to edge of widget
        offsetx = (int) event->x;
        offsety = (int) event->y;
        // maxx, maxy both relative to the parent
        // note that we're rounding down now so that these max values don't get
        // rounded upward later and push the widget off the edge of its parent.
        maxx =
            RoundDownToMultiple (alloc_p.width -
                                 alloc_w.width, Sensitivity);
        maxy =
            RoundDownToMultiple (alloc_p.height -
                                 alloc_w.height, Sensitivity);
    }
    return TRUE;
}

static gboolean
motion_notify_event (GtkWidget * widget,
                     GdkEventMotion * event, App * app)
{
    
    gint x, y;
    gboolean ret;

    GET_UI_ELEMENT (GtkWidget, layout1);

    gtk_widget_translate_coordinates (widget, layout1, event->x,
                                          event->y, &x,
                                          &y);

    x -= offsetx;
    y -= offsety;

    // make sure the potential coordinates x,y:
    //   1) will not push any part of the widget outside of its parent container
    //   2) is a multiple of Sensitivity

    x = RoundToNearestMultiple (Max (Min (x, maxx), 0),
                                Sensitivity);
    y = RoundToNearestMultiple (Max (Min (y, maxy), 0),
                                Sensitivity);

    if (x != px || y != py) {
        px = x;
        py = y;
        /*g_print ("x: %i, y: %i\n", x, y);*/
        /*g_print ("x_event: %f, y_event: %f\n", event->x,*/
                 /*event->y);*/
        gtk_layout_move (GTK_LAYOUT (layout1), widget, x, y);
    }

    gtk_widget_queue_draw(layout1);

    return TRUE;
}

void
make_button (const gchar * const name, App * app)
{
    GtkWidget *b =
        (GtkWidget *) app_get_ui_element (app, name);

    gtk_widget_add_events (b, GDK_BUTTON_PRESS_MASK | GDK_BUTTON1_MOTION_MASK);

    g_signal_connect (G_OBJECT (b), "button-press-event",
                      G_CALLBACK (button_press_event), app);
    g_signal_connect (G_OBJECT (b), "motion-notify-event",
                      G_CALLBACK (motion_notify_event), app);
}

int
main (int argc, char *argv[])
{
    App *app;

    app = (App *) g_new (App, 1);

    gtk_init (&argc, &argv);

    app_init (app);

    GET_UI_ELEMENT (GtkWidget, mainwindow);
    GET_UI_ELEMENT (GtkWidget, layout1);

    g_signal_connect(G_OBJECT(layout1), "draw", G_CALLBACK(layout_draw_cb), app); 

    make_button ("eventbox1", app);
    make_button ("eventbox2", app);

    gtk_widget_show_all (mainwindow);

    gtk_main ();

    return 0;
}
