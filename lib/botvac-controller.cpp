#include "botvac-controller.hpp"

// Constructor
BotvacController::BotvacController() : LibSerial::SerialStream() {
    std::setlocale(LC_ALL, "C");
}

// Method to initialize the robot for further operation
void BotvacController::initialize(std::string serialPort) {
    if (!IsOpen()) {
        Open(serialPort);
        if (!IsOpen()) {
            std::cout << "Unable to open serial port." << std::endl;
            std::exit(1);
        }
        SetBaudRate(LibSerial::BaudRate::BAUD_115200);
        *this << "TestMode On" << std::endl;
        std::getline(*this, input);
        *this << "SetLED BacklightOff" << std::endl;
        std::getline(*this, input);
        *this << "SetLED ButtonOff" << std::endl;
        std::getline(*this, input);
        *this << "SetLED SpotOff" << std::endl;
        std::getline(*this, input);
        *this << "SetLDSRotation On" << std::endl;
        std::getline(*this, input);
        sleep(1);
    }
}

// Method to shutdown the robot
void BotvacController::shutdown() {
    if (IsOpen()) {
        *this << "SetSystemMode Shutdown" << std::endl;
        std::getline(*this, input);
        Close();
    }
}

// Method to get pitch in degrees
float BotvacController::getPitch() {
    float returnValue = 0;
    if (IsOpen()) {
        *this << "GetAccel" << std::endl;
        for (int i = 0; i < 8; i++) {
            std::getline(*this, input);
            if (i == 2) {
                returnValue = std::stof(input.substr(15));
            }
        }
    }
    return returnValue;
}

// Method to get roll in degrees
float BotvacController::getRoll() {
    float returnValue = 0;
    if (IsOpen()) {
        *this << "GetAccel" << std::endl;
        for (int i = 0; i < 8; i++) {
            std::getline(*this, input);
            if (i == 3) {
                returnValue = std::stof(input.substr(14));
            }
        }
    }
    return returnValue;
}

// Method to get charge in percent
int BotvacController::getCharge() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetCharger" << std::endl;
        for (int i = 0; i < 17; i++) {
            std::getline(*this, input);
            if (i == 2) {
                returnValue = std::stoi(input.substr(12));
            }
        }
    }
    return returnValue;
}

// Method to get left magnet sensor reading
int BotvacController::getLeftMagnetSensor() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetAnalogSensors" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::getline(*this, input);
            if (i == 11) {
                returnValue = std::stoi(input.substr(18));
            }
        }
    }
    return returnValue;
}

// Method to get right magnet sensor reading
int BotvacController::getRightMagnetSensor() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetAnalogSensors" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::getline(*this, input);
            if (i == 12) {
                returnValue = std::stoi(input.substr(19));
            }
        }
    }
    return returnValue;
}

// Method to get wall sensor distance in mm
int BotvacController::getWallSensor() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetAnalogSensors" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::getline(*this, input);
            if (i == 13) {
                returnValue = std::stoi(input.substr(14));
            }
        }
    }
    return returnValue;
}

// Method to get left drop sensor distance in mm
int BotvacController::getLeftDropSensor() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetAnalogSensors" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::getline(*this, input);
            if (i == 14) {
                returnValue = std::stoi(input.substr(18));
            }
        }
    }
    return returnValue;
}

// Method to get right drop sensor distance in mm
int BotvacController::getRightDropSensor() {
    int returnValue = 0;
    if (IsOpen()) {
        *this << "GetAnalogSensors" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::getline(*this, input);
            if (i == 15) {
                returnValue = std::stoi(input.substr(19));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left wheel is extended
bool BotvacController::isLeftWheelExtended() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 4) {
                returnValue = (bool) std::stoi(input.substr(25));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right wheel is extended
bool BotvacController::isRightWheelExtended() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 5) {
                returnValue = (bool) std::stoi(input.substr(26));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left front bumper is extended
bool BotvacController::isLeftFrontBumperPressed() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 7) {
                returnValue = (bool) std::stoi(input.substr(10));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right front bumper is extended
bool BotvacController::isRightFrontBumperPressed() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 10) {
                returnValue = (bool) std::stoi(input.substr(10));
            }
        }
    }
    return returnValue;
}

// Method to check whether the left side bumper is extended
bool BotvacController::isLeftSideBumperPressed() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 6) {
                returnValue = (bool) std::stoi(input.substr(9));
            }
        }
    }
    return returnValue;
}

// Method to check whether the right side bumper is extended
bool BotvacController::isRightSideBumperPressed() {
    bool returnValue = false;
    if (IsOpen()) {
        *this << "GetDigitalSensors" << std::endl;
        for (int i = 0; i < 12; i++) {
            std::getline(*this, input);
            if (i == 9) {
                returnValue = (bool) std::stoi(input.substr(9));
            }
        }
    }
    return returnValue;
}

// Method the get LIDAR scan as a distance in mm vector
std::vector<int> BotvacController::getLidarScan() {
    std::vector<int> returnValue;
    if (IsOpen()) {
        *this << "GetLDSScan" << std::endl;
        std::getline(*this, input);
        std::getline(*this, input);
        for (int i = 0; i < 360; i++) {
            std::getline(*this, input);
            input.erase(0, input.find(',') + 1);
            returnValue.push_back(std::stoi(input.substr(0, input.find(','))));
        }
        std::getline(*this, input);
    }
    return returnValue;
}

// Method to move the robot
void BotvacController::moveRobot(int leftWheelDistance, int rightWheelDistance, int speed) {
    if (IsOpen()) {
        if (leftWheelDistance < -10000) {
            leftWheelDistance = -10000;
        } else if (leftWheelDistance > 10000) {
            leftWheelDistance = 10000;
        } else if (leftWheelDistance == 0) {
            leftWheelDistance = 1;
        }
        if (rightWheelDistance < -10000) {
            rightWheelDistance = -10000;
        } else if (rightWheelDistance > 10000) {
            rightWheelDistance = 10000;
        } else if (rightWheelDistance == 0) {
            rightWheelDistance = 1;
        }
        if (speed < 1) {
            speed = 1;
        } else if (speed > 350) {
            speed = 350;
        }
        *this << "SetMotor LWheelDist " << leftWheelDistance << " RWheelDist " << rightWheelDistance << " Speed " << speed << std::endl;
        std::getline(*this, input);
        sleep(std::ceil(std::max(std::abs(leftWheelDistance), std::abs(rightWheelDistance)) / (float) speed) + 1);
    }
}

// Method to control the brush
void BotvacController::controlBrush(int rpm) {
    if (IsOpen()) {
        if (rpm < 0) {
            rpm = 0;
        } else if (rpm > 10000) {
            rpm = 10000;
        }
        *this << "SetMotor Brush RPM " << rpm << std::endl;
        std::getline(*this, input);
        std::getline(*this, input);
        sleep(1);
    }
}

// Method to control the vacuum motor
void BotvacController::controlVacuum(int dutyCycle) {
    if (IsOpen()) {
        if (dutyCycle < 0) {
            dutyCycle = 0;
        } else if (dutyCycle > 100) {
            dutyCycle = 100;
        }
        *this << "SetMotor VacuumOn VacuumSpeed " << dutyCycle << std::endl;
        std::getline(*this, input);
        std::getline(*this, input);
        sleep(1);
    }
}

// Method to turn the side brush on
void BotvacController::turnSideBrushOn() {
    if (IsOpen()) {
        *this << "SetMotor SideBrushOn SideBrushPower 5000" << std::endl;
        std::getline(*this, input);
        std::getline(*this, input);
        sleep(1);
    }
}

// Method to turn the side brush off
void BotvacController::turnSideBrushOff() {
    if (IsOpen()) {
        *this << "SetMotor SideBrushOff" << std::endl;
        std::getline(*this, input);
        std::getline(*this, input);
        sleep(1);
    }
}