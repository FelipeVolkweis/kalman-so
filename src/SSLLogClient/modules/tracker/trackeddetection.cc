#include <SSLLogClient/modules/tracker/trackeddetection.hh>

TrackedDetection::TrackedDetection(WorldData &worldData) : worldData_(worldData) {}

void TrackedDetection::process(SSL_DetectionFrame detection) {
    for (int i = 0; i < detection.robots_blue_size(); i++) {
        SSL_DetectionRobot robot = detection.robots_blue(i);
        Data data;
        data.x = robot.x();
        data.y = robot.y();
        data.theta = robot.orientation();
        data.id = robot.robot_id();

        if (!blueTeam_.contains(robot.robot_id())) {
            blueTeam_.insert(robot.robot_id(),
                             std::make_shared<Object>(robot.robot_id(), Colors::BLUE));
        }
        blueTeam_[robot.robot_id()]->setData(detection.camera_id(), data);
    }

    for (int i = 0; i < detection.robots_yellow_size(); i++) {
        SSL_DetectionRobot robot = detection.robots_yellow(i);
        Data data;
        data.x = robot.x();
        data.y = robot.y();
        data.theta = robot.orientation();
        data.id = robot.robot_id();

        if (!yellowTeam_.contains(robot.robot_id())) {
            yellowTeam_.insert(robot.robot_id(),
                               std::make_shared<Object>(robot.robot_id(), Colors::YELLOW));
        }
        yellowTeam_[robot.robot_id()]->setData(detection.camera_id(), data);
    }

    if (detection.balls_size() != 0) {
        Data data;
        data.x = detection.balls(0).x();
        data.y = detection.balls(0).y();
        data.theta = 0;
        ball_.setData(detection.camera_id(), data);
    }

    for (auto &robot : blueTeam_) {
        robot->process();
    }

    for (auto &robot : yellowTeam_) {
        robot->process();
    }

    if (detection.balls_size() != 0)
        ball_.process();

    for (auto &robot : blueTeam_) {
        worldData_.setBlueRobotPosition(robot.get()->id, robot.get()->X, robot.get()->Y,
                                        robot.get()->THETA);
        if (!robot.get()->isValid) {
            worldData_.setBlueRobotInvalid(robot.get()->id);
        }
    }

    for (auto &robot : yellowTeam_) {
        worldData_.setYellowRobotPosition(robot.get()->id, robot.get()->X, robot.get()->Y,
                                          robot.get()->THETA);
        if (!robot.get()->isValid) {
            worldData_.setYellowRobotInvalid(robot.get()->id);
        }
    }

    worldData_.setBallPosition(ball_.X, ball_.Y);
    if (!ball_.isValid) {
        worldData_.setBallInvalid();
    }
}