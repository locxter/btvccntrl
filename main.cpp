#include <iostream>
#include <fstream> 
#include "lib/botvac-controller.hpp"
#include "lib/visualisation.hpp"

// Main function
int main(int argc, char** argv) {
    // Check for the right number of command line arguments
    if (argc == 2) {
        // Communication and navigation related variables
        const std::string SERIAL_PORT = argv[1];
        BotvacController botvacController;
        std::vector<std::vector<int>> map;
        // UI components
        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("com.github.locxter.btvccntrl");
        Gtk::Window window;
        Gtk::Grid grid;
        Gtk::Button connectButton("Connect");
        Gtk::Label pitchLabel("Pitch:");
        Gtk::Label pitchData;
        Gtk::Label rollLabel("Roll:");
        Gtk::Label rollData;
        Gtk::Label chargeLabel("Charge:");
        Gtk::Label chargeData;
        Gtk::Label leftMagnetLabel("Left magnet:");
        Gtk::Label leftMagnetData;
        Gtk::Label rightMagnetLabel("Right magnet:");
        Gtk::Label rightMagnetData;
        Gtk::Label wallLabel("Wall:");
        Gtk::Label wallData;
        Gtk::Label leftDropLabel("Left drop:");
        Gtk::Label leftDropData;
        Gtk::Label rightDropLabel("Right drop:");
        Gtk::Label rightDropData;
        Gtk::Label leftWheelLabel("Left wheel:");
        Gtk::Label leftWheelData;
        Gtk::Label rightWheelLabel("Right wheel:");
        Gtk::Label rightWheelData;
        Gtk::Label leftFrontLabel("Left front:");
        Gtk::Label leftFrontData;
        Gtk::Label rightFrontLabel("Right front:");
        Gtk::Label rightFrontData;
        Gtk::Label leftSideLabel("Left side:");
        Gtk::Label leftSideData;
        Gtk::Label rightSideLabel("Right side:");
        Gtk::Label rightSideData;
        Visualisation visualisation;
        Gtk::Button forwardButton;
        Gtk::Label brushLabel("Brush:");
        Gtk::SpinButton brushInput(Gtk::Adjustment::create(0, 0, 10000, 10));
        Gtk::Button leftButton;
        Gtk::Button rightButton;
        Gtk::Label vacuumLabel("Vacuum:");
        Gtk::SpinButton vacuumInput(Gtk::Adjustment::create(0, 0, 100, 1));
        Gtk::Button backwardButton;
        Gtk::Label sideBrushLabel("Side brush:");
        Gtk::ComboBoxText sideBrushInput;
        Gtk::Label aboutLabel("2022 locxter");
        // Add functions to the buttons and inputs
        connectButton.signal_clicked().connect([&]() {
            if (!botvacController.IsOpen()) {
                botvacController.initialize(SERIAL_PORT);
                connectButton.set_label("Disconnect");
            } else {
                std::ifstream testFile;
                int counter = 0;
                std::string completeFilename = "map-" + std::to_string(counter) + ".csv";
                std::ofstream outputFile;
                botvacController.disconnect();
                testFile.open(completeFilename);
                while (testFile.is_open()) {
                    testFile.close();
                    counter++;
                    completeFilename = "map-" + std::to_string(counter) + ".csv";
                    testFile.open(completeFilename);
                }
                testFile.close();
                outputFile.open(completeFilename);
                outputFile << "X:, Y:" << std::endl;
                for (int i = 0; i < map.size(); i++) {
                    outputFile << map[i][0] << ", " << map[i][1];
                    if (i < map.size() - 1) {
                        outputFile << std::endl;
                    }
                }
                outputFile.close();
                connectButton.set_label("Connect");
            }
        });
        forwardButton.signal_clicked().connect([&]() {
            botvacController.moveRobot(350, 175);
        });
        leftButton.signal_clicked().connect([&]() {
            botvacController.rotateRobot(-90, 175);
        });
        rightButton.signal_clicked().connect([&]() {
            botvacController.rotateRobot(90, 175);
        });
        backwardButton.signal_clicked().connect([&]() {
            botvacController.moveRobot(-350, 175);
        });
        brushInput.signal_value_changed().connect([&]() {
            std::cout << "Brush: " << brushInput.get_value() << std::endl;
            botvacController.controlBrush(std::round(brushInput.get_value()));
        });
        vacuumInput.signal_value_changed().connect([&]() {
            std::cout << "Vacuum: " << vacuumInput.get_value() << std::endl;
            botvacController.controlVacuum(std::round(vacuumInput.get_value()));
        });
        sideBrushInput.signal_changed().connect([&]() {
            if (sideBrushInput.get_active_row_number() == 0) {
                botvacController.turnSideBrushOn();
            } else {
                botvacController.turnSideBrushOff();
            }
        });
        // Create a background function for updating the map and it's visualisation
        Glib::signal_timeout().connect([&]() -> bool {
            if (botvacController.IsOpen()) {
                const int SIMILARITY_THRESHOLD = 40;
                std::vector<std::vector<int>> scan = botvacController.getLidarScan();
                for (int i = 0; i < scan.size(); i++) {
                    bool uniqueCoordinates = true;
                    for (int j = 0; j < map.size(); j++) {
                        if ((scan[i][0] > map[j][0] - SIMILARITY_THRESHOLD && scan[i][0] < map[j][0] + SIMILARITY_THRESHOLD) && (scan[i][1] > map[j][1] - SIMILARITY_THRESHOLD && scan[i][1] < map[j][1] + SIMILARITY_THRESHOLD)) {
                            uniqueCoordinates = false;
                        }
                    }
                    if (uniqueCoordinates) {
                        map.push_back(scan[i]);
                    }
                }
                if (!map.empty()) {
                    visualisation.showVisualisation(map);
                }
                pitchData.set_label(std::to_string((int) std::round(botvacController.getPitch())));
                rollData.set_label(std::to_string((int) std::round(botvacController.getRoll())));
                chargeData.set_label(std::to_string(botvacController.getCharge()));
                leftMagnetData.set_label(std::to_string(botvacController.getLeftMagnetSensor()));
                rightMagnetData.set_label(std::to_string(botvacController.getRightMagnetSensor()));
                wallData.set_label(std::to_string(botvacController.getWallSensor()));
                leftDropData.set_label(std::to_string(botvacController.getLeftDropSensor()));
                rightDropData.set_label(std::to_string(botvacController.getRightDropSensor()));
                leftWheelData.set_label(std::to_string(botvacController.isLeftWheelExtended()));
                rightWheelData.set_label(std::to_string(botvacController.isRightWheelExtended()));
                leftFrontData.set_label(std::to_string(botvacController.isLeftFrontBumperPressed()));
                rightFrontData.set_label(std::to_string(botvacController.isRightFrontBumperPressed()));
                leftSideData.set_label(std::to_string(botvacController.isLeftSideBumperPressed()));
                rightSideData.set_label(std::to_string(botvacController.isRightSideBumperPressed()));
            }
            return true;
        }, 4000);
        // Create the main grid
        grid.set_column_spacing(10);
        grid.set_row_spacing(10);
        connectButton.set_hexpand(true);
        grid.attach(connectButton, 0, 0, 4);
        pitchLabel.set_hexpand(true);
        grid.attach(pitchLabel, 0, 1);
        pitchData.set_hexpand(true);
        grid.attach(pitchData, 1, 1);
        rollLabel.set_hexpand(true);
        grid.attach(rollLabel, 2, 1);
        rollData.set_hexpand(true);
        grid.attach(rollData, 3, 1);
        chargeLabel.set_hexpand(true);
        grid.attach(chargeLabel, 0, 2);
        chargeData.set_hexpand(true);
        grid.attach(chargeData, 1, 2);
        leftMagnetLabel.set_hexpand(true);
        grid.attach(leftMagnetLabel, 2, 2);
        leftMagnetData.set_hexpand(true);
        grid.attach(leftMagnetData, 3, 2);
        rightMagnetLabel.set_hexpand(true);
        grid.attach(rightMagnetLabel, 0, 3);
        rightMagnetData.set_hexpand(true);
        grid.attach(rightMagnetData, 1, 3);
        wallLabel.set_hexpand(true);
        grid.attach(wallLabel, 2, 3);
        wallData.set_hexpand(true);
        grid.attach(wallData, 3, 3);
        leftDropLabel.set_hexpand(true);
        grid.attach(leftDropLabel, 0, 4);
        leftDropData.set_hexpand(true);
        grid.attach(leftDropData, 1, 4);
        rightDropLabel.set_hexpand(true);
        grid.attach(rightDropLabel, 2, 4);
        rightDropData.set_hexpand(true);
        grid.attach(rightDropData, 3, 4);
        leftWheelLabel.set_hexpand(true);
        grid.attach(leftWheelLabel, 0, 5);
        leftWheelData.set_hexpand(true);
        grid.attach(leftWheelData, 1, 5);
        rightWheelLabel.set_hexpand(true);
        grid.attach(rightWheelLabel, 2, 5);
        rightWheelData.set_hexpand(true);
        grid.attach(rightWheelData, 3, 5);
        leftFrontLabel.set_hexpand(true);
        grid.attach(leftFrontLabel, 0, 6);
        leftFrontData.set_hexpand(true);
        grid.attach(leftFrontData, 1, 6);
        rightFrontLabel.set_hexpand(true);
        grid.attach(rightFrontLabel, 2, 6);
        rightFrontData.set_hexpand(true);
        grid.attach(rightFrontData, 3, 6);
        leftSideLabel.set_hexpand(true);
        grid.attach(leftSideLabel, 0, 7);
        leftSideData.set_hexpand(true);
        grid.attach(leftSideData, 1, 7);
        rightSideLabel.set_hexpand(true);
        grid.attach(rightSideLabel, 2, 7);
        rightSideData.set_hexpand(true);
        grid.attach(rightSideData, 3, 7);
        visualisation.set_hexpand(true);
        visualisation.set_vexpand(true);
        grid.attach(visualisation, 0, 8, 4);
        forwardButton.set_image_from_icon_name("go-up-symbolic");
        forwardButton.set_halign(Gtk::Align::ALIGN_CENTER);
        forwardButton.set_valign(Gtk::Align::ALIGN_CENTER);
        grid.attach(forwardButton, 1, 9, 1, 2);
        brushLabel.set_hexpand(true);
        grid.attach(brushLabel, 3, 9);
        brushInput.set_hexpand(true);
        grid.attach(brushInput, 3, 10);
        leftButton.set_image_from_icon_name("go-previous-symbolic");
        leftButton.set_halign(Gtk::Align::ALIGN_CENTER);
        leftButton.set_valign(Gtk::Align::ALIGN_CENTER);
        grid.attach(leftButton, 0, 11, 1, 2);
        rightButton.set_image_from_icon_name("go-next-symbolic");
        rightButton.set_halign(Gtk::Align::ALIGN_CENTER);
        rightButton.set_valign(Gtk::Align::ALIGN_CENTER);
        grid.attach(rightButton, 2, 11, 1, 2);
        vacuumLabel.set_hexpand(true);
        grid.attach(vacuumLabel, 3, 11);
        vacuumInput.set_hexpand(true);
        grid.attach(vacuumInput, 3, 12);
        backwardButton.set_image_from_icon_name("go-down-symbolic");
        backwardButton.set_halign(Gtk::Align::ALIGN_CENTER);
        backwardButton.set_valign(Gtk::Align::ALIGN_CENTER);
        grid.attach(backwardButton, 1, 13, 1, 2);
        sideBrushLabel.set_hexpand(true);
        grid.attach(sideBrushLabel, 3, 13);
        sideBrushInput.append("On");
        sideBrushInput.append("Off");
        sideBrushInput.set_active(1);
        sideBrushInput.set_hexpand(true);
        grid.attach(sideBrushInput, 3, 14);
        aboutLabel.set_hexpand(true);
        grid.attach(aboutLabel, 0, 15, 4);
        // Create the main window
        window.set_title("btvccntrl");
        window.set_border_width(10);
        window.set_default_size(640, 640);
        window.set_size_request(480, 640);
        window.add(grid);
        window.show_all();
        return app->run(window);
    } else {
        // Throw an error on invalid number of command line arguments
        std::cout << "Wrong number of arguments. One argument containing the serial port expected." << std::endl;
        std::cout << "Example: " << argv[0] << " /dev/ttyACM0" << std::endl;
        return 1;
    }
}