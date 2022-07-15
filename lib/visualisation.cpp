#include "visualisation.hpp"

// Helper method to transform an unscaled value to a scaled one
int Visualisation::getScaledValue(int unscaledValue) {
    return std::round(scalingRatio * unscaledValue);
}

// Method to actually draw the visualisation
bool Visualisation::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    int width = get_width();
    int height = get_height();
    // Calculate the scaling ratio and center the canvas
    if ((float) width / height > 1) {
        scalingRatio = height / 1080.0;
        context->translate(std::round((width - (scalingRatio * 1080)) / 2), 0);
    } else {
        scalingRatio = width / 1080.0;
        context->translate(0, std::round((height - (scalingRatio * 1080)) / 2));
    }
    if (showStartScreen) {
        // Draw a start screen if no visualisation has been requested yet
        Pango::FontDescription font;
        Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Connect robot vacuum to see the visualisation.");
        int textWidth;
        int textHeight;
        context->set_source_rgb(0, 0, 0);
        font.set_family("Sans Serif");
        font.set_weight(Pango::WEIGHT_NORMAL);
        layout->set_font_description(font);
        layout->get_pixel_size(textWidth, textHeight);
        context->move_to((getScaledValue(1080) - textWidth) / 2, (getScaledValue(1080) - textHeight) / 2);
        layout->show_in_cairo_context(context);
    } else {
        // Draw the axes
        context->set_source_rgb(0, 0, 0);
        context->set_line_width(getScaledValue(5));
        context->move_to(getScaledValue(540), 0);
        context->line_to(getScaledValue(540), getScaledValue(1080));
        context->stroke();
        context->move_to(0, getScaledValue(540));
        context->line_to(getScaledValue(1080), getScaledValue(540));
        context->stroke();
        // Draw the points
        for (int i = 0; i < scan.size(); i++) {
            if (scan[i] > 5000 || scan[i] == 0) {
                continue;
            }
            int xCoordinate = std::round(scan[i] * std::cos((i + 270) * (M_PI / 180.0)) * (-540.0 / 5000));
            int yCoordinate = std::round(scan[i] * std::sin((i + 270) * (M_PI / 180.0)) * (540.0 / 5000));
            context->arc(getScaledValue(540 + xCoordinate), getScaledValue(540 + yCoordinate), getScaledValue(5), 0, 2 * M_PI);
            context->fill();
        }
    }
    return true;
}

// Constructor
Visualisation::Visualisation() : Gtk::DrawingArea() {
}

// Method to update the visualisation
void Visualisation::showVisualisation(std::vector<int> scan) {
    if (showStartScreen) {
        showStartScreen = false;
    }
    this->scan = scan;
    queue_draw();
}