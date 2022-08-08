#include <iostream>
#include <fstream> 
#include "lib/botvac-controller.hpp"
#include "lib/pathfinder.hpp"
#include "lib/visualisation.hpp"

// Main function
int main(int argc, char** argv) {
    // Check for the right number of command line arguments
    if (argc == 3) {
        // Communication and navigation related variables
        const bool USE_NETWORK = std::string(argv[1]) == "network";
        const std::string DEVICE = argv[2];
        BotvacController botvacController;
        Pathfinder pathfinder;
        std::vector<std::vector<int>> map;
        botvacController.setMinPointDistance(50);
        botvacController.setInaccuracyFilterRatio(0.01);
        pathfinder.setSimplificationFactor(100);
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
            if (!botvacController.isConnected()) {
                botvacController.connect(DEVICE, USE_NETWORK);
                connectButton.set_label("Disconnect");
            } else {
                std::ifstream testFile;
                int counter = 0;
                std::string completeFilename = "map-" + std::to_string(counter) + ".csv";
                std::ofstream outputFile;
                botvacController.disconnect();
                // Write map to CSV file
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
            botvacController.moveRobot(500, 100);
            if (!map.empty()) {
                visualisation.showVisualisation(map, botvacController.getX(), botvacController.getY(), botvacController.getAngle());
            }
        });
        leftButton.signal_clicked().connect([&]() {
            botvacController.rotateRobot(-90, 100);
            if (!map.empty()) {
                visualisation.showVisualisation(map, botvacController.getX(), botvacController.getY(), botvacController.getAngle());
            }
        });
        rightButton.signal_clicked().connect([&]() {
            botvacController.rotateRobot(90, 100);
            if (!map.empty()) {
                visualisation.showVisualisation(map, botvacController.getX(), botvacController.getY(), botvacController.getAngle());
            }
        });
        backwardButton.signal_clicked().connect([&]() {
            botvacController.moveRobot(-500, 100);
            if (!map.empty()) {
                visualisation.showVisualisation(map, botvacController.getX(), botvacController.getY(), botvacController.getAngle());
            }
        });
        brushInput.signal_value_changed().connect([&]() {
            botvacController.controlBrush(std::round(brushInput.get_value()));
        });
        vacuumInput.signal_value_changed().connect([&]() {
            botvacController.controlVacuum(std::round(vacuumInput.get_value()));
        });
        sideBrushInput.signal_changed().connect([&]() {
            if (sideBrushInput.get_active_row_number() == 0) {
                botvacController.turnSideBrushOn();
            } else {
                botvacController.turnSideBrushOff();
            }
        });
        // Create a background function for updating the map and it's visualisation as well as handling navigation
        Glib::signal_timeout().connect_seconds([&]() -> bool {
            if (botvacController.isConnected()) {
                int currentX = botvacController.getX();
                int currentY = botvacController.getY();
                int clickX = visualisation.getClickX();
                int clickY = visualisation.getClickY();
                static std::vector<std::vector<int>> path;
                botvacController.updateLidar();
                map = botvacController.getLidarMap();
                // Plan path if wanted
                if (!map.empty() && path.empty() && clickX != -1 && clickY != -1) {
                    Gtk::MessageDialog dialog(window, "Pathfinder");
                    dialog.set_secondary_text("Valid path found.\nEnable autonomous navigation?");
                    pathfinder.setMap(map);
                    path = pathfinder.findPath(currentX, currentY, clickX, clickY);
                    if (path.empty()) {
                        dialog.set_secondary_text("No valid path found");
                    } else {
                        dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
                    }
                    int response = dialog.run();
                    if (response != Gtk::RESPONSE_OK) {
                        path.clear();
                    }
                }
                // Follow path if possible
                if (!path.empty()) {
                    std::vector<int> coordinates = path[0];
                    int currentAngle = botvacController.getAngle();
                    int angle = 0;
                    int distance = std::abs(coordinates[1] - currentY);
                    if (coordinates[0] < currentX) {
                        distance = std::abs(coordinates[0] - currentX);
                        angle = 270;
                    } else if (coordinates[0] > currentX) {
                        distance = std::abs(coordinates[0] - currentX);
                        angle = 90;
                    } else if (coordinates[1] < currentY) {
                        angle = 180;
                    }
                    if (currentAngle != angle) {
                        int angleToGo = angle - currentAngle;
                        if (std::abs(angleToGo) == 270) {
                            angleToGo /= -3;
                        }
                        botvacController.rotateRobot(angleToGo, 100);
                        visualisation.showVisualisation(map, currentX, currentY, angle);
                        while (Gtk::Main::events_pending()) {
                            Gtk::Main::iteration();
                        }
                    }
                    botvacController.moveRobot(distance, 100);
                    path.erase(path.begin());
                }
                // Update visualisation and other data
                if (!map.empty()) {
                    visualisation.showVisualisation(map, botvacController.getX(), botvacController.getY(), botvacController.getAngle());
                    while (Gtk::Main::events_pending()) {
                        Gtk::Main::iteration();
                    }
                }
                botvacController.updateAccelerometer();
                pitchData.set_label(std::to_string((int) std::round(botvacController.getPitch())));
                rollData.set_label(std::to_string((int) std::round(botvacController.getRoll())));
                while (Gtk::Main::events_pending()) {
                    Gtk::Main::iteration();
                }
                botvacController.updateCharge();
                chargeData.set_label(std::to_string(botvacController.getCharge()));
                while (Gtk::Main::events_pending()) {
                    Gtk::Main::iteration();
                }
                botvacController.updateAnalogSensors();
                leftMagnetData.set_label(std::to_string(botvacController.getLeftMagnetStrength()));
                rightMagnetData.set_label(std::to_string(botvacController.getRightMagnetStrength()));
                wallData.set_label(std::to_string(botvacController.getWallDistance()));
                leftDropData.set_label(std::to_string(botvacController.getLeftDropDistance()));
                rightDropData.set_label(std::to_string(botvacController.getRightDropDistance()));
                while (Gtk::Main::events_pending()) {
                    Gtk::Main::iteration();
                }
                botvacController.updateDigitalSensors();
                leftWheelData.set_label(std::to_string(botvacController.isLeftWheelExtended()));
                rightWheelData.set_label(std::to_string(botvacController.isRightWheelExtended()));
                leftFrontData.set_label(std::to_string(botvacController.isLeftFrontBumperPressed()));
                rightFrontData.set_label(std::to_string(botvacController.isRightFrontBumperPressed()));
                leftSideData.set_label(std::to_string(botvacController.isLeftSideBumperPressed()));
                rightSideData.set_label(std::to_string(botvacController.isRightSideBumperPressed()));
            }
            return true;
        }, 2);
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
        std::cout << "Wrong number of arguments. Two arguments containing the connection mode (serial or network) and device expected." << std::endl;
        std::cout << "Example: " << argv[0] << " serial /dev/ttyACM0" << std::endl;
        return 1;
    }
}