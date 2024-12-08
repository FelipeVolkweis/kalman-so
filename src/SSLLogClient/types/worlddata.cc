#include <SSLLogClient/types/worlddata.hh>

WorldData::WorldData() {}

void WorldData::setBlueRobotPosition(int id, float x, float y, float theta) {
    QMutexLocker locker(&blueTeamMutex_);
    blueTeam_[id].x = x;
    blueTeam_[id].y = y;
    blueTeam_[id].theta = theta;
    blueTeam_[id].valid = true;
}

void WorldData::setBlueRobotVelocity(int id, float vx, float vy, float vtheta) {
    QMutexLocker locker(&blueTeamMutex_);
    blueTeam_[id].vx = vx;
    blueTeam_[id].vy = vy;
    blueTeam_[id].vtheta = vtheta;
}

void WorldData::setYellowRobotPosition(int id, float x, float y, float theta) {
    QMutexLocker locker(&yellowTeamMutex_);
    yellowTeam_[id].x = x;
    yellowTeam_[id].y = y;
    yellowTeam_[id].theta = theta;
    yellowTeam_[id].valid = true;
}

void WorldData::setYellowRobotVelocity(int id, float vx, float vy, float vtheta) {
    QMutexLocker locker(&yellowTeamMutex_);
    yellowTeam_[id].vy = vx;
    yellowTeam_[id].vy = vy;
    yellowTeam_[id].vtheta = vtheta;
}

void WorldData::setBallPosition(float x, float y) {
    QMutexLocker locker(&ballMutex_);
    ball_.x = x;
    ball_.y = y;
    ball_.valid = true;
}

void WorldData::setBallVelocity(float vx, float vy) {
    QMutexLocker locker(&ballMutex_);
    ball_.vx = vx;
    ball_.vy = vy;
}

const RobotData &WorldData::getBlueRobot(int id) {
    QMutexLocker locker(&blueTeamMutex_);
    return blueTeam_[id];
}

const RobotData &WorldData::getYellowRobot(int id) {
    QMutexLocker locker(&yellowTeamMutex_);
    return yellowTeam_[id];
}

const BallData &WorldData::getBall() {
    QMutexLocker locker(&ballMutex_);
    return ball_;
}

void WorldData::setBlueRobotInvalid(int id) {
    QMutexLocker locker(&blueTeamMutex_);
    blueTeam_[id].valid = false;
}

void WorldData::setYellowRobotInvalid(int id) {
    QMutexLocker locker(&yellowTeamMutex_);
    yellowTeam_[id].valid = false;
}

void WorldData::setBallInvalid() {
    QMutexLocker locker(&ballMutex_);
    ball_.valid = false;
}

QVector<int> WorldData::getBlueActiveRobots() {
    QMutexLocker locker(&blueTeamMutex_);
    QVector<int> activeRobots;
    for (const auto &robot : blueTeam_) {
        if (robot.valid) {
            activeRobots.push_back(robot.id);
        }
    }

    return activeRobots;
}

QVector<int> WorldData::getYellowActiveRobots() {
    QMutexLocker locker(&yellowTeamMutex_);
    QVector<int> activeRobots;
    for (const auto &robot : yellowTeam_) {
        if (robot.valid) {
            activeRobots.push_back(robot.id);
        }
    }

    return activeRobots;
}