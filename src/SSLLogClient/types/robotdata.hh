#ifndef ROBOTDATA_HH
#define ROBOTDATA_HH

namespace Colors {
enum Color { UNKNOWN, BLUE, YELLOW };
}

struct RobotData {
    float x, y, theta;
    float vx, vy, vtheta;
    Colors::Color color;
    int id;
};

#endif // ROBOTDATA_HH