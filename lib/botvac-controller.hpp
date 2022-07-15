#ifndef LIBSERIAL_HELPERS
#define LIBSERIAL_HELPERS
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <libserial/SerialStream.h>

// Image sorter class
class BotvacController : public LibSerial::SerialStream {
    private:
    // Attributes
    std::string input;

    public:
    // Constructor
    BotvacController();

    // Method to initialize the robot for further operation
    void initialize(std::string serialPort);

    // Method to shutdown the robot
    void shutdown();

    // Method to get pitch in degrees
    float getPitch();

    // Method to get roll in degrees
    float getRoll();

    // Method to get charge in percent
    int getCharge();

    // Method to get left magnet sensor reading
    int getLeftMagnetSensor();

    // Method to get right magnet sensor reading
    int getRightMagnetSensor();

    // Method to get wall sensor distance in mm
    int getWallSensor();

    // Method to get left drop sensor distance in mm
    int getLeftDropSensor();

    // Method to get right drop sensor distance in mm
    int getRightDropSensor();

    // Method to check whether the left wheel is extended
    bool isLeftWheelExtended();

    // Method to check whether the left wheel is extended
    bool isRightWheelExtended();

    // Method to check whether the left front bumper is extended
    bool isLeftFrontBumperPressed();

    // Method to check whether the right front bumper is extended
    bool isRightFrontBumperPressed();

    // Method to check whether the left side bumper is extended
    bool isLeftSideBumperPressed();

    // Method to check whether the right side bumper is extended
    bool isRightSideBumperPressed();

    // Method the get LIDAR scan as a distance in mm vector
    std::vector<int> getLidarScan();

    // Method to move the robot
    void moveRobot(int leftWheelDistance, int rightWheelDistance, int speed);

    // Method to control the brush
    void controlBrush(int rpm);

    // Method to control the vacuum motor
    void controlVacuum(int dutyCycle);

    // Method to turn the side brush on
    void turnSideBrushOn();

    // Method to turn the side brush off
    void turnSideBrushOff();
};
#endif