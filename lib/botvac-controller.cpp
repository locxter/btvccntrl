#include "botvac-controller.hpp"

// Helper method send commands and receive responses the same way regardless of the connection mode
void BotvacController::sendCommand(std::string command) {
    std::stringstream empty;
    std::string line;
    response.swap(empty);
    if (useNetwork) {
        try {
            curlpp::Forms formParts;
            formParts.push_back(new curlpp::FormParts::Content("command", command));
            network.setOpt(new curlpp::options::HttpPost(formParts));
            response << network;
        } catch (...) {
            std::cout << "Did not receive a response for command: " << command << std::endl;
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
}

// Constructors
BotvacController::BotvacController() {
    std::setlocale(LC_ALL, "C");
}

BotvacController::BotvacController(std::string device, bool useNetwork) {
    std::setlocale(LC_ALL, "C");
    connect(device, useNetwork);
}

// Getter
bool BotvacController::isConnected() {
    return connected;
}

bool BotvacController::usesNetwork() {
    return useNetwork;
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

// Method to get pitch in degrees
float BotvacController::getPitch() {
    float returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAccel");
        for (int i = 0; i < 7; i++) {
            std::getline(response, line);
            if (i == 1) {
                returnValue = std::stof(line.substr(15));
            }
        }
    }
    return returnValue;
}

// Method to get roll in degrees
float BotvacController::getRoll() {
    float returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAccel");
        for (int i = 0; i < 7; i++) {
            std::getline(response, line);
            if (i == 2) {
                returnValue = std::stof(line.substr(14));
            }
        }
    }
    return returnValue;
}

// Method to get charge in percent
int BotvacController::getCharge() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetCharger");
        for (int i = 0; i < 16; i++) {
            std::getline(response, line);
            if (i == 1) {
                returnValue = std::stoi(line.substr(12));
            }
        }
    }
    return returnValue;
}

// Method to get left magnet sensor reading
int BotvacController::getLeftMagnetSensor() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            if (i == 10) {
                returnValue = std::stoi(line.substr(18));
            }
        }
    }
    return returnValue;
}

// Method to get right magnet sensor reading
int BotvacController::getRightMagnetSensor() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            if (i == 11) {
                returnValue = std::stoi(line.substr(19));
            }
        }
    }
    return returnValue;
}

// Method to get wall sensor distance in mm
int BotvacController::getWallSensor() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            if (i == 12) {
                returnValue = std::stoi(line.substr(14));
            }
        }
    }
    return returnValue;
}

// Method to get left drop sensor distance in mm
int BotvacController::getLeftDropSensor() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            if (i == 13) {
                returnValue = std::stoi(line.substr(18));
            }
        }
    }
    return returnValue;
}

// Method to get right drop sensor distance in mm
int BotvacController::getRightDropSensor() {
    int returnValue = 0;
    if (connected) {
        std::string line;
        sendCommand("GetAnalogSensors");
        for (int i = 0; i < 15; i++) {
            std::getline(response, line);
            if (i == 14) {
                returnValue = std::stoi(line.substr(19));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left wheel is extended
bool BotvacController::isLeftWheelExtended() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 3) {
                returnValue = (bool) std::stoi(line.substr(25));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right wheel is extended
bool BotvacController::isRightWheelExtended() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 4) {
                returnValue = (bool) std::stoi(line.substr(26));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left front bumper is extended
bool BotvacController::isLeftFrontBumperPressed() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 6) {
                returnValue = (bool) std::stoi(line.substr(10));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right front bumper is extended
bool BotvacController::isRightFrontBumperPressed() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 9) {
                returnValue = (bool) std::stoi(line.substr(10));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left side bumper is extended
bool BotvacController::isLeftSideBumperPressed() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 5) {
                returnValue = (bool) std::stoi(line.substr(9));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right side bumper is extended
bool BotvacController::isRightSideBumperPressed() {
    bool returnValue = false;
    if (connected) {
        std::string line;
        sendCommand("GetDigitalSensors");
        for (int i = 0; i < 11; i++) {
            std::getline(response, line);
            if (i == 8) {
                returnValue = (bool) std::stoi(line.substr(9));
            }
        }
    }
    return returnValue;
}

// Method the get LIDAR scan as a relative x, y coordinate vector
std::vector<std::vector<int>> BotvacController::getLidarScan() {
    std::vector<std::vector<int>> returnValue;
    if (connected) {
        std::string line;
        sendCommand("GetLDSScan");
        std::getline(response, line);
        for (int i = 0; i < 360; i++) {
            int distance;
            std::vector<int> coordinates;
            std::getline(response, line);
            line.erase(0, line.find(',') + 1);
            distance = std::stoi(line.substr(0, line.find(',')));
            if (distance > 6000 || distance == 0) {
                continue;
            }
            coordinates.push_back(std::round(distance * std::cos((i + 90) * (M_PI / 180.0))));
            coordinates.push_back(std::round((distance * std::sin((i + 90) * (M_PI / 180.0))) - 92.5));
            returnValue.push_back(coordinates);
        }
        std::getline(response, line);
    }
    return returnValue;
}

// Method the get LIDAR map as an absolute x, y coordinate vector
std::vector<std::vector<int>> BotvacController::getLidarMap() {
    if (connected) {
        std::string line;
        sendCommand("GetLDSScan");
        std::getline(response, line);
        for (int i = 0; i < 360; i++) {
            int distance;
            std::vector<int> coordinates;
            int inaccuracyFilter;
            bool unique = true;
            std::getline(response, line);
            line.erase(0, line.find(',') + 1);
            distance = std::stoi(line.substr(0, line.find(',')));
            if (distance > 6000 || distance == 0) {
                continue;
            }
            coordinates.push_back(std::round((distance * std::cos((i + 90 - angle) * (M_PI / 180.0))) + (-92.5 * std::sin(angle * (M_PI / 180.0)))) + x);
            coordinates.push_back(std::round((distance * std::sin((i + 90 - angle) * (M_PI / 180.0))) + (-92.5 * std::cos(angle * (M_PI / 180.0)))) + y);
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
    return map;
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
        //sendCommand("SetSystemMode Shutdown");
        if (useNetwork) {
            network.reset();
        } else {
            serial.Close();
        }
        connected = false;
        useNetwork = false;
        device = "";
        x = 0;
        y = 0;
        angle = 0;
        map.clear();
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
        sleep(std::ceil(std::abs(distance) / (float) speed) + 1);
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
        distance = std::round(angle * ((245.0 * M_PI) / 360));
        sendCommand("SetMotor LWheelDist " + std::to_string(distance) + " RWheelDist " + std::to_string(-1 * distance) + " Speed " + std::to_string(speed));
        this->angle = (this->angle + angle + 360) % 360;
        sleep(std::ceil(std::abs(distance) / (float) speed) + 1);
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
        sleep(1);
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
        sleep(1);
    }
}

// Method to turn the side brush on
void BotvacController::turnSideBrushOn() {
    if (connected) {
        std::string line;
        sendCommand("SetMotor SideBrushOn SideBrushPower 5000");
        std::getline(response, line);
        sleep(1);
    }
}

// Method to turn the side brush off
void BotvacController::turnSideBrushOff() {
    if (connected) {
        std::string line;
        sendCommand("SetMotor SideBrushOff");
        std::getline(response, line);
        sleep(1);
    }
}