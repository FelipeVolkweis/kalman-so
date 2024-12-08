#include <SSLLogClient/modules/tracker/object.hh>

#define NORMALIZE_ANGLE(angle) atan2f(sinf(angle), cosf(angle))

Object::Object(int id, int teamNumber) : id(id), teamNumber(teamNumber) {
    X = Y = THETA = 0;
    VX = VY = OMEGA = 0;
    isValid = false;
}

Object::Object() : Object(-1, -1) {}

bool Object::process() {
    for (auto cameraID : kalmanFilters_.keys()) {
        KalmanFilter &kf = kalmanFilters_[cameraID];
        Lifetime &lifetime = lifetimes_[cameraID];
        lifetime.downBeat();

        if (lifetime.isDead()) {
            kalmanFilters_.remove(cameraID);
            kalmanFilters_.insert(cameraID, KalmanFilter());
            continue;
        }

        if (!lifetime.isHealthy()) {
            kf.predict();
            continue;
        }

        kf.predict();
        kf.update(data_[cameraID].x, data_[cameraID].y, data_[cameraID].theta);
    }
    mergeData();

    return isValid;
}

void Object::setData(CameraID cameraID, Data data) {
    data_[cameraID] = data;
    if (!kalmanFilters_.contains(cameraID)) {
        kalmanFilters_.insert(cameraID, KalmanFilter());
    }
    if (!lifetimes_.contains(cameraID)) {
        lifetimes_.insert(cameraID, Lifetime());
    }
    lifetimes_[cameraID].upBeat();
}

void Object::mergeData() {
    if (lifetimes_.size() == 0 || isEveryOneDead()) {
        dataMutex_.lock();
        isValid = false;
        dataMutex_.unlock();
        return;
    }

    float totalXYUncertainty = 0;
    float totalThetaUncertainty = 0;
    float totalVUncertainty = 0;
    float totalOmegaUncertainty = 0;

    for (auto cameraID : kalmanFilters_.keys()) {
        const KalmanFilter &kf = kalmanFilters_.value(cameraID);
        const Lifetime &lifetime = lifetimes_.value(cameraID);

        if (lifetime.isDead()) {
            continue;
        }

        float unc = 1.0f / lifetime.getHeartbeat();
        totalXYUncertainty += powf(kf.getXYUncertainty() * unc, -MERGE_POWER);
        totalThetaUncertainty += powf(kf.getThetaUncertainty() * unc, -MERGE_POWER);
        totalVUncertainty += powf(kf.getVUncertainty() * unc, -MERGE_POWER);
        totalOmegaUncertainty += powf(kf.getOmegaUncertainty() * unc, -MERGE_POWER);
    }

    float x = 0, y = 0, theta = 0;
    float vx = 0, vy = 0, omega = 0;
    float thetaOffset = THETA;

    for (auto cameraID : kalmanFilters_.keys()) {
        const KalmanFilter &kf = kalmanFilters_.value(cameraID);
        const Lifetime &lifetime = lifetimes_.value(cameraID);

        if (lifetime.isDead()) {
            continue;
        }

        float unc = 1.0f / lifetime.getHeartbeat();
        x += kf.getPosition().first * powf(kf.getXYUncertainty() * unc, -MERGE_POWER);
        y += kf.getPosition().second * powf(kf.getXYUncertainty() * unc, -MERGE_POWER);

        float o = NORMALIZE_ANGLE(kf.getOrientation() - thetaOffset);
        theta += o * powf(kf.getThetaUncertainty() * unc, -MERGE_POWER);
        omega += kf.getAngularVelocity() * powf(kf.getOmegaUncertainty() * unc, -MERGE_POWER);
    }

    dataMutex_.lock();
    X = x / totalXYUncertainty;
    Y = y / totalXYUncertainty;
    theta = NORMALIZE_ANGLE(theta / totalThetaUncertainty + thetaOffset);
    THETA = theta;
    VX = vx / totalVUncertainty;
    VY = vy / totalVUncertainty;
    OMEGA = omega / totalOmegaUncertainty;
    isValid = true;
    dataMutex_.unlock();
}

bool Object::isEveryOneDead() {
    for (const Lifetime &lifetime : lifetimes_) {
        if (!lifetime.isDead()) {
            return false;
        }
    }
    return true;
}