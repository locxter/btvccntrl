#ifndef BOTVAC_CONTROLLER
#define BOTVAC_CONTROLLER
#include <cmath>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <iostream>
#include <libserial/SerialStream.h>
#include <sstream>
#include <unistd.h>

// Botvac controller class
class BotvacController {
    private:
    // Attributes
    bool connected = false;
    bool useNetwork = false;
    std::string device;
    LibSerial::SerialStream serial;
    curlpp::Easy network;
    std::stringstream response;
    float pitch = 0;
    float roll = 0;
    int charge = 0;
    int leftMagnetStrength = 0;
    int rightMagnetStrength = 0;
    int wallDistance = 0;
    int leftDropDistance = 0;
    int rightDropDistance = 0;
    bool leftWheelExtended = false;
    bool rightWheelExtended = false;
    bool leftFrontBumperPressed = false;
    bool rightFrontBumperPressed = false;
    bool leftSideBumperPressed = false;
    bool rightSideBumperPressed = false;
    std::vector<std::vector<int>> scan;
    std::vector<std::vector<int>> map;
    int x = 0;
    int y = 0;
    int angle = 0;
    int minPointDistance = 0;
    float inaccuracyFilterRatio = 0;

    // Helper method to send commands and receive responses the same way regardless of the connection mode
    void sendCommand(std::string command);

    public:
    // Constructors
    BotvacController();

    BotvacController(std::string device, bool useNetwork = false);

    BotvacController(int minPointDistance);

    BotvacController(float inaccuracyFilterRatio);

    BotvacController(int minPointDistance, float inaccuracyFilterRatio);

    BotvacController(int minPointDistance, std::string device, bool useNetwork = false);

    BotvacController(float inaccuracyFilterRatio, std::string device, bool useNetwork = false);

    BotvacController(int minPointDistance, float inaccuracyFilterRatio, std::string device, bool useNetwork = false);

    // Getter
    bool isConnected();

    bool usesNetwork();

    float getPitch();

    float getRoll();

    int getCharge();

    int getLeftMagnetStrength();

    int getRightMagnetStrength();

    int getWallDistance();

    int getLeftDropDistance();

    int getRightDropDistance();

    bool isLeftWheelExtended();

    bool isRightWheelExtended();

    bool isLeftFrontBumperPressed();

    bool isRightFrontBumperPressed();

    bool isLeftSideBumperPressed();

    bool isRightSideBumperPressed();

    // Method the get LIDAR scan as a relative x, y coordinate vector
    std::vector<std::vector<int>> getLidarScan();

    // Method the get LIDAR map as an absolute x, y coordinate vector
    std::vector<std::vector<int>> getLidarMap();

    int getX();

    int getY();

    int getAngle();

    int getMinPointDistance();

    int getInaccuracyFilterRatio();

    // Setter
    void setMinPointDistance(int distance);

    void setInaccuracyFilterRatio(float ratio);

    // Method to connect to the robot
    void connect(std::string device, bool useNetwork = false);

    // Method to disconnect the robot
    void disconnect();

    // Method to update the accelerometer data
    void updateAccelerometer();

    // Method to update the charge data
    void updateCharge();

    // Method to update the analog sensor data
    void updateAnalogSensors();

    // Method to update the digital sensor data
    void updateDigitalSensors();

    // Method to update the LIDAR data
    void updateLidar();

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
