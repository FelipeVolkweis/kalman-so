#ifndef BALL_HH
#define BALL_HH

#include <SSLLogClient/modules/tracker/object.hh>

class Ball : public Object {
public:
    Ball() : Object() {
        type = ObjectTypes::ObjectType::BALL;
    }
};

#endif // BALL_HH