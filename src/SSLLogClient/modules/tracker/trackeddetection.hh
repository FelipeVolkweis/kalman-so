#ifndef TRACKEDDETECTION_HH
#define TRACKEDDETECTION_HH

#include <3rdparty/proto/pb/ssl_vision_detection.pb.h>

#include <QHash>

#include <SSLLogClient/modules/tracker/object.hh>
#include <SSLLogClient/types/worlddata.hh>

typedef int RobotID;
typedef QHash<RobotID, std::shared_ptr<Object>> Team;

class TrackedDetection {
public:
    TrackedDetection(WorldData &worldData);

    void process(SSL_DetectionFrame detectionFrame);

    const Team &getBlueTeam() const {
        return blueTeam_;
    }

    const Team &getYellowTeam() const {
        return yellowTeam_;
    }

    const Object &getBall() const {
        return ball_;
    }

private:
    Team blueTeam_;
    Team yellowTeam_;
    Object ball_;

    WorldData &worldData_;
};

#endif // TRACKEDDETECTION_HH