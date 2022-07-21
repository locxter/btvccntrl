#include "visualisation.hpp"

// Helper method to transform an unscaled value to a scaled one
int Visualisation::getScaledValue(int unscaledValue) {
    return std::round(scalingRatio * unscaledValue);
}

// Method to actually draw the visualisation
bool Visualisation::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    int width = get_width();
    int height = get_height();
    if (showStartScreen) {
        // Draw a start screen if no visualisation has been requested yet
        Pango::FontDescription font;
        Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Connect to robot vacuum to see the visualisation.");
        int textWidth;
        int textHeight;
        context->set_source_rgb(0, 0, 0);
        font.set_family("Sans Serif");
        font.set_weight(Pango::WEIGHT_NORMAL);
        layout->set_font_description(font);
        layout->get_pixel_size(textWidth, textHeight);
        context->move_to((width - textWidth) / 2, (height - textHeight) / 2);
        layout->show_in_cairo_context(context);
    } else {
        // Calculate the scaling ratio and center the canvas
        int xMinCoordinate = 0;
        int xMaxCoordinate = 0;
        int xRange;
        int yMinCoordinate = 0;
        int yMaxCoordinate = 0;
        int yRange;
        for (int i = 0; i < map.size(); i++) {
            if (map[i][0] < xMinCoordinate) {
                xMinCoordinate = map[i][0];
            } else if (map[i][0] > xMaxCoordinate) {
                xMaxCoordinate = map[i][0];
            }
            if (map[i][1] < yMinCoordinate) {
                yMinCoordinate = map[i][1];
            } else if (map[i][1] > yMaxCoordinate) {
                yMaxCoordinate = map[i][1];
            }
        }
        xRange = std::abs(xMinCoordinate) + std::abs(xMaxCoordinate);
        yRange = std::abs(yMinCoordinate) + std::abs(yMaxCoordinate);
        if ((float) width / height > (float) xRange / yRange) {
            scalingRatio = (float) height / yRange;
            context->translate(std::round((width - getScaledValue(xRange)) / 2), 0);
        } else {
            scalingRatio = (float) width / xRange;
            context->translate(0, std::round((height - getScaledValue(yRange)) / 2));
        }
        // Draw the points
        context->set_source_rgb(0, 0, 0);
        for (int i = 0; i < map.size(); i++) {
            if (xMinCoordinate <= 0) {
                map[i][0] += std::abs(xMinCoordinate);
            } else {
                map[i][0] -= xMinCoordinate;
            }
            if (yMinCoordinate <= 0) {
                map[i][1] += std::abs(yMinCoordinate);
            } else {
                map[i][1] -= yMinCoordinate;
            }
            context->arc(getScaledValue(map[i][0]), getScaledValue(yRange - map[i][1]), std::max(getScaledValue(std::round(yRange * 0.005)), 2), 0, 2 * M_PI);
            context->fill();
        }
    }
    return true;
}

// Constructor
Visualisation::Visualisation() : Gtk::DrawingArea() {
}

// Method to update the visualisation
void Visualisation::showVisualisation(std::vector<std::vector<int>> map) {
    if (showStartScreen) {
        showStartScreen = false;
    }
    this->map = map;
    queue_draw();
}