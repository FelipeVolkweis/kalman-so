#include <SSLLogClient/modules/tracker/trackeddetection.hh>

TrackedDetection::TrackedDetection() {}

void TrackedDetection::process(SSL_DetectionFrame detection) {
    for (int i = 0; i < detection.robots_blue_size(); i++) {
        SSL_DetectionRobot robot = detection.robots_blue(i);
        Data data;
        data.x = robot.x();
        data.y = robot.y();
        data.theta = robot.orientation();

        if (!blueTeam_.contains(robot.robot_id())) {
            blueTeam_.insert(robot.robot_id(), std::make_shared<Object>());
        }
        blueTeam_[robot.robot_id()]->setData(detection.camera_id(), data);
    }

    for (int i = 0; i < detection.robots_yellow_size(); i++) {
        SSL_DetectionRobot robot = detection.robots_yellow(i);
        Data data;
        data.x = robot.x();
        data.y = robot.y();
        data.theta = robot.orientation();

        if (!yellowTeam_.contains(robot.robot_id())) {
            yellowTeam_.insert(robot.robot_id(), std::make_shared<Object>());
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
}