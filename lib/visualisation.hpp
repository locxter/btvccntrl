#ifndef VISUALISATION_HELPERS
#define VISUALISATION_HELPERS
#include <iostream>
#include <cmath>
#include <gtkmm-3.0/gtkmm.h>

// Visualisation class
class Visualisation : public Gtk::DrawingArea {
    private:
    // Attributes
    float scalingRatio;
    bool showStartScreen = true;
    std::vector<std::vector<int>> map;
    int xCoordinate = 0;
    int yCoordinate = 0;
    int angle = 0;

    // Helper method to transform an unscaled value to a scaled one
    int getScaledValue(int unscaledValue);

    protected:
    // Method to actually draw the visualisation
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& context) override;

    public:
    // Constructor
    Visualisation();

    // Method to update the visualisation
    void showVisualisation(std::vector<std::vector<int>> map, int xCoordinate, int yCoordinate, int angle);
};
#endif