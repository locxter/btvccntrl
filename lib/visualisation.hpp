#ifndef VISUALISATION
#define VISUALISATION
#include <iostream>
#include <gtkmm-3.0/gtkmm.h>

// Visualisation class
class Visualisation : public Gtk::DrawingArea {
    private:
    // Attributes
    std::vector<std::vector<int>> map;
    int x;
    int y;
    int angle;
    int clickX = -1;
    int clickY = -1;
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    float scalingRatio;
    bool showStartScreen = true;

    // Helper method to transform an unscaled value to a scaled one
    int getScaledValue(int unscaledValue);

    protected:
    // Method to actually draw the visualisation
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& context) override;
    // Method to deal with mouse clicks
    bool on_button_press_event(GdkEventButton* event) override;

    public:
    // Constructors
    Visualisation();

    Visualisation(std::vector<std::vector<int>> map, int x, int y, int angle);

    // Getter
    int getClickX();

    int getClickY();

    // Method to update the visualisation
    void showVisualisation(std::vector<std::vector<int>> map, int x, int y, int angle);
};
#endif
