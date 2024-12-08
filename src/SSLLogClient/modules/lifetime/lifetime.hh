#ifndef LIFETIME_HH
#define LIFETIME_HH

#define MAX_HEALTH 20
#define UNHEALTHY_THRESHOLD 14

class Lifetime {
public:
    Lifetime() : _heartbeat(0) {}

    void downBeat();
    void upBeat();
    bool isHealthy() const;
    bool isDead() const;
    int getHeartbeat() const;

private:
    int _heartbeat;
};

#endif // LIFETIME_HH