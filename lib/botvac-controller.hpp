#ifndef BOTVAC_CONTROLLER
#define BOTVAC_CONTROLLER
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <libserial/SerialStream.h>

// Botvac controller class
class BotvacController : public LibSerial::SerialStream {
    private:
    // Attributes
    std::string input;
    std::vector<std::vector<int>> map;
    int x = 0;
    int y = 0;
    int angle = 0;
    int similarityThreshold = 0;

    public:
    // Constructor
    BotvacController();

    // Getter
    int getX();

    int getY();

    int getAngle();

    int getSimilarityThreshold();

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

    // Method the get LIDAR scan as a relative x, y coordinate vector
    std::vector<std::vector<int>> getLidarScan();

    // Method the get LIDAR scan as an absolute x, y coordinate vector
    std::vector<std::vector<int>> getLidarMap();

    // Setter
    void setSimilarityThreshold(int threshold);

    // Method to initialize the robot for further operation
    void initialize(std::string serialPort);

    // Method to disconnect the robot
    void disconnect();

    // Method to move the robot
    void moveRobot(int distance, int speed);

    // Method to rotate the robot
    void rotateRobot(int angle, int speed);

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