#include "app.h"
#include "callbacks.h"

gboolean layout_draw_cb(GtkWidget *widget, cairo_t *cr, App *app) {
    GtkAllocation alloc_source, alloc_end;

    gfloat x_source, y_source, height_source, width_source;
    gfloat x_end, y_end, height_end, width_end;
    double x0, y0, x1, y1, x2, y2, x3, y3, y_tip, x_tip;
    double arrow_tip_width, arrow_tip_length, alpha;

    PangoLayout *layout;
    PangoFontDescription *desc;

    GET_UI_ELEMENT(GtkWidget, eventbox1);
    GET_UI_ELEMENT(GtkWidget, eventbox2);

    gtk_widget_get_allocation(eventbox1, &alloc_source);
    gtk_widget_get_allocation(eventbox2, &alloc_end);

    x_source = alloc_source.x;
    y_source = alloc_source.y;
    width_source = alloc_source.width;
    height_source = alloc_source.height;

    x_end = alloc_end.x;
    y_end = alloc_end.y;
    width_end = alloc_end.width;
    height_end = alloc_end.height;

    x0 = x_source + width_source;
    y0 = y_source + height_source / 2;

    x3 = x_end;
    y3 = y_end + height_end / 2;

    arrow_tip_width = 10;
    arrow_tip_length = 50;
    alpha = atan2f (y3 - y0, x3 - x0);

    cairo_set_source_rgb (cr, 0, 1, 0);
    cairo_translate (cr, x3, y3);
    cairo_rotate (cr, alpha);
    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, -arrow_tip_length, -arrow_tip_width);
    cairo_line_to (cr, -arrow_tip_length, arrow_tip_width);
    cairo_fill (cr);

    x3 = -arrow_tip_length / 2;
    y3 = 0;

    cairo_user_to_device (cr, &x3, &y3);

    x_tip = x3;
    y_tip = (y3 - y0) / (x3 - x0) * (x_tip - x0) + y0;

    cairo_identity_matrix (cr);

    cairo_move_to (cr, x0, y0);
    cairo_curve_to (cr, x0 + 40, y0, x_tip, y_tip, x_tip,
                    y_tip);
    cairo_set_line_width (cr, 5);
    cairo_stroke (cr);

    cairo_set_source_rgb (cr, 0, 0.5, 0);

    layout = pango_cairo_create_layout (cr);

    pango_layout_set_markup (layout, "<span gravity=\"south\" weight=\"550\" size=\"x-large\">W<sub>el</sub></span>", -1);
    pango_cairo_update_layout (cr, layout);

    cairo_move_to (cr, x0, y0);
    pango_cairo_show_layout (cr, layout);

    return FALSE;
}

