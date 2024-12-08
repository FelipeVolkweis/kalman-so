#ifndef WORLD_DATA_HH
#define WORLD_DATA_HH

#include <QHash>
#include <QMutex>

#include <SSLLogClient/types/balldata.hh>
#include <SSLLogClient/types/robotdata.hh>

class WorldData {
public:
    WorldData();

    void setBlueRobotPosition(int id, float x, float y, float theta);
    void setBlueRobotVelocity(int id, float vx, float vy, float vtheta);

    void setYellowRobotPosition(int id, float x, float y, float theta);
    void setYellowRobotVelocity(int id, float vx, float vy, float vtheta);

    void setBallPosition(float x, float y);
    void setBallVelocity(float vx, float vy);

    void setBlueRobotInvalid(int id);
    void setYellowRobotInvalid(int id);

    void setBallInvalid();

    const RobotData &getBlueRobot(int id);
    const RobotData &getYellowRobot(int id);
    const BallData &getBall();

    QVector<int> getBlueActiveRobots();
    QVector<int> getYellowActiveRobots();

private:
    QHash<int, RobotData> blueTeam_;
    QHash<int, RobotData> yellowTeam_;
    BallData ball_;

    QMutex ballMutex_;
    QMutex blueTeamMutex_;
    QMutex yellowTeamMutex_;
};

#endif // WORLD_DATA_HH