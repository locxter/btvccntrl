#include "visualisation.hpp"

// Helper method to transform an unscaled value to a scaled one
int Visualisation::getScaledValue(int unscaledValue) {
    return std::round(scalingRatio * unscaledValue);
}

// Method to actually draw the visualisation
bool Visualisation::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    int width = get_width();
    int height = get_height();
    int xRange = std::abs(xMin) + std::abs(xMax);
    int yRange = std::abs(yMin) + std::abs(yMax);
    int maxRange = std::max(xRange, yRange);
    if (showStartScreen) {
        // Draw a start screen if no visualisation has been requested yet
        Pango::FontDescription font;
        Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Connect to robot vacuum to see the visualisation.");
        int textWidth;
        int textHeight;
        context->set_source_rgb(255, 255, 255);
        font.set_family("Sans Serif");
        font.set_weight(Pango::WEIGHT_NORMAL);
        layout->set_font_description(font);
        layout->get_pixel_size(textWidth, textHeight);
        context->move_to(((float) width - textWidth) / 2, ((float) height - textHeight) / 2);
        layout->show_in_cairo_context(context);
    } else {
        // Calculate the scaling ratio and center the canvas
        if ((float) width / height > (float) xRange / yRange) {
            scalingRatio = (float) height / yRange;
            context->translate(std::round(((float) width - getScaledValue(xRange)) / 2), 0);
        } else {
            scalingRatio = (float) width / xRange;
            context->translate(0, std::round(((float) height - getScaledValue(yRange)) / 2));
        }
        // Draw the points
        context->set_source_rgb(255, 255, 255);
        for (int i = 0; i < map.size(); i++) {
            context->arc(getScaledValue(map[i][0]), getScaledValue(yRange - map[i][1]), std::max(getScaledValue(std::round(maxRange * 0.005)), 2), 0, 2 * M_PI);
            context->fill();
        }
        // Draw the robot and it's movement direction
        context->set_source_rgb(0, 255, 0);
        context->arc(getScaledValue(x), getScaledValue(yRange - y), std::max(getScaledValue(std::round(maxRange * 0.01)), 4), 0, 2 * M_PI);
        context->fill();
        context->set_line_width(std::max(getScaledValue(std::round(maxRange * 0.005)), 2));
        context->move_to(getScaledValue(x), getScaledValue(yRange - y));
        context->line_to(getScaledValue(x) + std::round(std::max(getScaledValue(std::round(maxRange * 0.05)), 20) * std::sin((180 - angle) * (M_PI / 180.0))), getScaledValue(yRange - y) + std::round(std::max(getScaledValue(std::round(maxRange * 0.05)), 20) * std::cos((180 - angle) * (M_PI / 180.0))));
        context->stroke();
    }
    return true;
}


// Method to deal with mouse clicks
bool Visualisation::on_button_press_event(GdkEventButton* event) {
    int width = get_width();
    int height = get_height();
    int xOffset = std::round(((float) width - getScaledValue(std::abs(xMin) + std::abs(xMax))) / 2);
    int yOffset = std::round(((float) height - getScaledValue(std::abs(yMin) + std::abs(yMax))) / 2);
    if (!showStartScreen && event->type == GDK_BUTTON_PRESS && event->x > xOffset && event->x < width - xOffset && event->y > yOffset && event->y < height - yOffset) {
        clickX = ((event->x - xOffset) * (1 / scalingRatio)) + xMin;
        clickY = ((height - (yOffset + event->y)) * (1 / scalingRatio)) + yMin;
    }
    return true;
}

// Constructor
Visualisation::Visualisation() : Gtk::DrawingArea() {
    add_events(Gdk::BUTTON_PRESS_MASK);
}

// Getter
int Visualisation::getClickX() {
    int returnValue = clickX;
    clickX = -1;
    return returnValue;
}

int Visualisation::getClickY() {
    int returnValue = clickY;
    clickY = -1;
    return returnValue;
}

// Method to update the visualisation
void Visualisation::showVisualisation(std::vector<std::vector<int>> map, int x, int y, int angle) {
    xMin = 0;
    xMax = 0;
    yMin = 0;
    yMax = 0;
    showStartScreen = false;
    // Determine value ranges
    for (int i = 0; i < map.size(); i++) {
        if (map[i][0] < xMin) {
            xMin = map[i][0];
        } else if (map[i][0] > xMax) {
            xMax = map[i][0];
        }
        if (map[i][1] < yMin) {
            yMin = map[i][1];
        } else if (map[i][1] > yMax) {
            yMax = map[i][1];
        }
    }
    // Move all the data to positive values
    for (int i = 0; i < map.size(); i++) {
        map[i][0] -= xMin;
        map[i][1] -= yMin;
    }
    x -= xMin;
    y -= yMin;
    this->map = map;
    this->x = x;
    this->y = y;
    this->angle = angle;
    queue_draw();
}
