#ifndef OBJECT_HH
#define OBJECT_HH

#include <QHash>
#include <QMutex>

#include <SSLLogClient/modules/kalmanfilter/kalmanfilter.hh>
#include <SSLLogClient/modules/lifetime/lifetime.hh>

#define MERGE_POWER 1.5f

namespace ObjectTypes {
enum ObjectType { BALL = 0, ROBOT = 1 };
}

typedef int CameraID;

struct Data {
    float x, y, theta;
};

class Object {
public:
    Object(int id, int teamNumber);
    Object();

    float X, Y, THETA;
    float VX, VY, OMEGA;
    ObjectTypes::ObjectType type;
    int id;
    int teamNumber;
    bool isValid;

    void setData(CameraID cameraID, Data data);
    bool process();

private:
    QHash<CameraID, KalmanFilter> kalmanFilters_;
    QHash<CameraID, Lifetime> lifetimes_;
    QHash<CameraID, Data> data_;

    void mergeData();
    bool isEveryOneDead();

    QMutex dataMutex_;
};

#endif // OBJECT_HH