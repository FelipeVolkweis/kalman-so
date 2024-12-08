#ifndef ROBOT_HH
#define ROBOT_HH

#include <SSLLogClient/modules/tracker/object.hh>

class Robot : public Object {
public:
    Robot(int id, int teamNumber) : Object(id, teamNumber) {
        type = ObjectTypes::ObjectType::ROBOT;
    }
};

#endif // ROBOT_HH