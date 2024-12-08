#include <SSLLogClient/modules/lifetime/lifetime.hh>

void Lifetime::downBeat() {
    if (_heartbeat > 0) {
        _heartbeat--;
    }
};

void Lifetime::upBeat() {
    if (_heartbeat < MAX_HEALTH) {
        _heartbeat += 2;
    }
};

bool Lifetime::isHealthy() const {
    return _heartbeat > UNHEALTHY_THRESHOLD;
};

bool Lifetime::isDead() const {
    return _heartbeat == 0;
};

int Lifetime::getHeartbeat() const {
    return _heartbeat;
};