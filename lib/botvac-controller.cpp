#include "botvac-controller.hpp"

// Helper method send commands and receive responses the same way regardless of the connection mode
void BotvacController::sendCommand(std::string command) {
    std::stringstream empty;
    std::string line;
    response.swap(empty);
    if (useNetwork) {
        try {
            curlpp::Forms data;
            data.push_back(new curlpp::FormParts::Content("command", command));
            network.setOpt(new curlpp::options::HttpPost(data));
            response << network;
        } catch (...) {
            std::cout << "Robot failed to execute the command " << command << '.' << std::endl;
            std::exit(1);
        }
    } else {
        char character;
        serial << command << std::endl;
        while ((character = serial.get()) != '\x1a') {
            response << character;
        }
    }
    std::getline(response, line);
    if (line != command + '\r') {
        std::cout << "Robot failed to execute the command " << command << '.' << std::endl;
        std::exit(1);
    }
}

// Constructors
BotvacController::BotvacController() {
    std::setlocale(LC_ALL, "C");
}

BotvacController::BotvacController(std::string device, bool useNetwork) : BotvacController() {
    connect(device, useNetwork);
}

BotvacController::BotvacController(int minPointDistance) : BotvacController() {
    setMinPointDistance(minPointDistance);
}

BotvacController::BotvacController(float inaccuracyFilterRatio) : BotvacController() {
    setInaccuracyFilterRatio(inaccuracyFilterRatio);
}

BotvacController::BotvacController(int minPointDistance, float inaccuracyFilterRatio) : BotvacController() {
    setMinPointDistance(minPointDistance);
    setInaccuracyFilterRatio(inaccuracyFilterRatio);
}

BotvacController::BotvacController(int minPointDistance, std::string device, bool useNetwork) : BotvacController() {
    setInaccuracyFilterRatio(inaccuracyFilterRatio);
    connect(device, useNetwork);
}

BotvacController::BotvacController(float inaccuracyFilterRatio, std::string device, bool useNetwork) : BotvacController() {
    setInaccuracyFilterRatio(inaccuracyFilterRatio);
    connect(device, useNetwork);
}

BotvacController::BotvacController(int minPointDistance, float inaccuracyFilterRatio, std::string device, bool useNetwork) : BotvacController() {
    setMinPointDistance(minPointDistance);
    setInaccuracyFilterRatio(inaccuracyFilterRatio);
    connect(device, useNetwork);
}

// Getter
bool BotvacController::isConnected() {
    return connected;
}

bool BotvacController::usesNetwork() {
    return useNetwork;
}

float BotvacController::getPitch() {
    return pitch;
}

float BotvacController::getRoll() {
    return roll;
}

int BotvacController::getCharge() {
    return charge;
}

int BotvacController::getLeftMagnetStrength() {
    return leftMagnetStrength;
}

int BotvacController::getRightMagnetStrength() {
    return rightMagnetStrength;
}

int BotvacController::getWallDistance() {
    return wallDistance;
}

int BotvacController::getLeftDropDistance() {
    return leftDropDistance;
}

int BotvacController::getRightDropDistance() {
    return rightDropDistance;
}

bool BotvacController::isLeftWheelExtended() {
    return leftWheelExtended;
}

bool BotvacController::isRightWheelExtended() {
    return rightWheelExtended;
}

bool BotvacController::isLeftFrontBumperPressed() {
    return leftFrontBumperPressed;
}

bool BotvacController::isRightFrontBumperPressed() {
    return rightFrontBumperPressed;
}

bool BotvacController::isLeftSideBumperPressed() {
    return leftSideBumperPressed;
}

bool BotvacController::isRightSideBumperPressed() {
    return rightSideBumperPressed;
}

// Method the get LIDAR scan as a relative x, y coordinate vector
std::vector<std::vector<int>> BotvacController::getLidarScan() {
    return scan;
}

// Method the get LIDAR map as an absolute x, y coordinate vector
std::vector<std::vector<int>> BotvacController::getLidarMap() {
    return map;
}

int BotvacController::getX() {
    return x;
}

int BotvacController::getY() {
    return y;
}

int BotvacController::getAngle() {
    return angle;
}

int BotvacController::getMinPointDistance() {
    return minPointDistance;
}

int BotvacController::getInaccuracyFilterRatio() {
    return inaccuracyFilterRatio;
}

// Setter
void BotvacController::setMinPointDistance(int distance) {
    if (distance < 0) {
        distance = 0;
    }
    minPointDistance = distance;
}

void BotvacController::setInaccuracyFilterRatio(float ratio) {
    if (ratio < 0) {
        ratio = 0;
    }
    inaccuracyFilterRatio = ratio;
}

// Method to connect to the robot
void BotvacController::connect(std::string device, bool useNetwork) {
    if (!connected) {
        if (useNetwork) {
            network.setOpt(new curlpp::options::Url(device));
            network.setOpt(new curlpp::options::UserPwd("btvcbrdg:btvcbrdg"));
        } else {
            serial.Open(device);
            if (!serial.IsOpen()) {
                std::cout << "Unable to open serial port." << std::endl;
                std::exit(1);
            }
            serial.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
        }
        connected = true;
        this->useNetwork = useNetwork;
        this->device = device;
        sendCommand("TestMode On");
        sendCommand("SetLED BacklightOff");
        sendCommand("SetLED ButtonOff");
        sendCommand("SetLED SpotOff");
        sendCommand("SetLDSRotation On");
        sleep(1);
    }
}

// Method to disconnect the robot
void BotvacController::disconnect() {
    if (connected) {
        sendCommand("SetLED BacklightOn");
        sendCommand("SetLED ButtonGreen");
        sendCommand("SetLED SpotOn");
        sendCommand("SetLDSRotation Off");
        sendCommand("ClearFiles");
        sendCommand("TestMode Off");
        if (useNetwork) {
            network.reset();
        } else {
            serial.Close();
        }
        connected = false;
        useNetwork = false;
        device = "";
        pitch = 0;
        roll = 0;
        charge = 0;
        leftMagnetStrength = 0;
        rightMagnetStrength = 0;
        wallDistance = 0;
        leftDropDistance = 0;
        rightDropDistance = 0;
        leftWheelExtended = false;
        rightWheelExtended = false;
        leftFrontBumperPressed = false;
        rightFrontBumperPressed = false;
        leftSideBumperPressed = false;
        rightSideBumperPressed = false;
        scan.clear();
        map.clear();
        x = 0;
        y = 0;
        angle = 0;
    }
}

// Method to update the accelerometer data
void BotvacController::updateAccelerometer() {
    if (connected) {
        std::string line;
        sendCommand("GetAccel");
        for (int i = 0; i < 7; i++) {
            std::getline(response, line);
            switch (i) {
                case 1:
                    pitch = std::stof(line.substr(15));
                    break;
                case 2:
                    roll = std::stof(line.substr(14));
                    break;
            }
        }
    }
}

// Method to update the charge data
void BotvacController::updateCharge() {
    if (connected) {
        std::string line;
        sendCommand("GetCharger");
        for (int i = 0; i < 16; i++) {
            std::getline(response, line);
            switch (i) {
                case 1:
                    charge = std::stoi(line.substr(12));
                    break;
            }
        }
    }
}

// Method to update the analog sensor data
void BotvacController::updateAnalogSensors() {
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            switch (i) {
                case 10:
                    leftMagnetStrength = std::stoi(line.substr(18));
                    break;
                case 11:
                    rightMagnetStrength = std::stoi(line.substr(19));
                    break;
                case 12:
                    wallDistance = std::stoi(line.substr(14));
                    break;
                case 13:
                    leftDropDistance = std::stoi(line.substr(18));
                    break;
                case 14:
                    rightDropDistance = std::stoi(line.substr(19));
                    break;
            }
        }
    }
}

// Method to update the digital sensor data
void BotvacController::updateDigitalSensors() {
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            switch (i) {
                case 3:
                    leftWheelExtended = (bool) std::stoi(line.substr(25));
                    break;
                case 4:
                    rightWheelExtended = (bool) std::stoi(line.substr(26));
                    break;
                case 5:
                    leftSideBumperPressed = (bool) std::stoi(line.substr(9));
                    break;
                case 6:
                    leftFrontBumperPressed = (bool) std::stoi(line.substr(10));
                    break;
                case 8:
                    rightSideBumperPressed = (bool) std::stoi(line.substr(9));
                    break;
                case 9:
                    rightFrontBumperPressed = (bool) std::stoi(line.substr(10));
                    break;
            }
        }
    }
}

// Method to update the LIDAR data
void BotvacController::updateLidar() {
    if (connected) {
        std::string line;
        sendCommand("GetLDSScan");
        std::getline(response, line);
        scan.clear();
        for (int i = 0; i < 360; i++) {
            int distance;
            std::vector<int> coordinates;
            int inaccuracyFilter;
            bool unique = true;
            std::getline(response, line);
            line.erase(0, line.find(',') + 1);
            distance = std::stoi(line.substr(0, line.find(',')));
            if (distance > 7500 || distance == 0) {
                continue;
            }
            coordinates.push_back(std::round(distance * std::sin(-i * (M_PI / 180.0))));
            coordinates.push_back(std::round((distance * std::cos(-i * (M_PI / 180.0))) - 92.5));
            scan.push_back(coordinates);
            coordinates.clear();
            coordinates.push_back(std::round(x + (distance * std::sin((-i + angle) * (M_PI / 180.0))) + (-92.5 * std::sin(angle * (M_PI / 180.0)))));
            coordinates.push_back(std::round(y + (distance * std::cos((-i + angle) * (M_PI / 180.0))) + (-92.5 * std::cos(angle * (M_PI / 180.0)))));
            inaccuracyFilter = std::round(std::sqrt(std::pow(coordinates[0] - x, 2) + std::pow(coordinates[1] - y, 2)) * inaccuracyFilterRatio);
            for (int j = 0; j < map.size(); j++) {
                if (coordinates[0] >= map[j][0] - (minPointDistance + inaccuracyFilter) && coordinates[0] <= map[j][0] + (minPointDistance + inaccuracyFilter) && coordinates[1] >= map[j][1] - (minPointDistance + inaccuracyFilter) && coordinates[1] <= map[j][1] + (minPointDistance + inaccuracyFilter)) {
                    unique = false;
                }
            }
            if (unique) {
                map.push_back(coordinates);
            }
        }
        std::getline(response, line);
    }
}

// Method to move the robot
void BotvacController::moveRobot(int distance, int speed) {
    if (connected) {
        std::string line;
        if (distance < -10000) {
            distance = -10000;
        } else if (distance > 10000) {
            distance = 10000;
        }
        if (speed < 1) {
            speed = 1;
        } else if (speed > 350) {
            speed = 350;
        }
        sendCommand("SetMotor LWheelDist " + std::to_string(distance) + " RWheelDist " + std::to_string(distance) + " Speed " + std::to_string(speed));
        x += std::round(distance * std::sin(angle * (M_PI / 180.0)));
        y += std::round(distance * std::cos(angle * (M_PI / 180.0)));
        sleep(std::ceil(std::abs(distance) / (float) speed));
    }
}

void BotvacController::rotateRobot(int angle, int speed) {
    if (connected) {
        int distance;
        std::string line;
        if (angle < -359) {
            angle = -359;
        } else if (angle > 359) {
            angle = 359;
        }
        if (speed < 1) {
            speed = 1;
        } else if (speed > 350) {
            speed = 350;
        }
        distance = std::round(angle * ((250.0 * M_PI) / 360));
        sendCommand("SetMotor LWheelDist " + std::to_string(distance) + " RWheelDist " + std::to_string(-1 * distance) + " Speed " + std::to_string(speed));
        this->angle = (this->angle + angle + 360) % 360;
        sleep(std::ceil(std::abs(distance) / (float) speed));
    }
}

// Method to control the brush
void BotvacController::controlBrush(int rpm) {
    if (connected) {
        std::string line;
        if (rpm < 0) {
            rpm = 0;
        } else if (rpm > 10000) {
            rpm = 10000;
        }
        sendCommand("SetMotor Brush RPM " + std::to_string(rpm));
        std::getline(response, line);
    }
}

// Method to control the vacuum motor
void BotvacController::controlVacuum(int dutyCycle) {
    if (connected) {
        std::string line;
        if (dutyCycle < 0) {
            dutyCycle = 0;
        } else if (dutyCycle > 100) {
            dutyCycle = 100;
        }
        sendCommand("SetMotor VacuumOn VacuumSpeed " + std::to_string(dutyCycle));
        std::getline(response, line);
    }
}

// Method to turn the side brush on
void BotvacController::turnSideBrushOn() {
    if (connected) {
        std::string line;
        sendCommand("SetMotor SideBrushOn SideBrushPower 5000");
        std::getline(response, line);
    }
}

// Method to turn the side brush off
void BotvacController::turnSideBrushOff() {
    if (connected) {
        std::string line;
        sendCommand("SetMotor SideBrushOff");
        std::getline(response, line);
    }
}
