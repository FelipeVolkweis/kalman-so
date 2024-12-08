#ifndef WORLD_DATA_HH
#define WORLD_DATA_HH

#include <QHash>

#include <SSLLogClient/types/balldata.hh>
#include <SSLLogClient/types/robotdata.hh>

class WorldData {
public:
    WorldData() {}

private:
    QHash<int, RobotData> blueTeam_;
    QHash<int, RobotData> yellowTeam_;
    BallData ball_;
};

#endif // WORLD_DATA_HH