#include <SSLLogClient/modules/kalmanfilter/kalmanfilter.hh>

#define DELTA_T 0.0016 // 16 ms
#define SIGMA 0.1

KalmanFilter::KalmanFilter() {
    A_ = Eigen::MatrixXf(STATE_SIZE, STATE_SIZE);
    A_ << 1, 0, 0, DELTA_T, 0, 0,
          0, 1, 0, 0, DELTA_T, 0,
          0, 0, 1, 0, 0, DELTA_T,
          0, 0, 0, 1, 0, 0,
          0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1;
    
    C_ = Eigen::MatrixXf(MEAS_SIZE, STATE_SIZE);
    C_ << 1, 0, 0, 0, 0, 0,
          0, 1, 0, 0, 0, 0,
          0, 0, 1, 0, 0, 0;

    float delta_t1 = DELTA_T;
    float delta_t2 = (1.0/2.0) * DELTA_T * DELTA_T;
    float delta_t3 = (1.0/3.0) * DELTA_T * DELTA_T * DELTA_T;

    R_ = Eigen::MatrixXf(STATE_SIZE, STATE_SIZE);
    R_ << delta_t3, 0, 0, delta_t2, 0, 0,
          0, delta_t3, 0, 0, delta_t2, 0,
          0, 0, delta_t3, 0, 0, delta_t2,
          0, 0, 0, delta_t1, 0, 0,
          0, 0, 0, 0, delta_t1, 0,
          0, 0, 0, 0, 0, delta_t1;
    R_ *= (SIGMA * SIGMA);
    
    Q_ = Eigen::MatrixXf(MEAS_SIZE, MEAS_SIZE);
    Q_ << 0.1, 0, 0,
          0, 0.1, 0,
          0, 0, 0.1;
    
    mean_ = Eigen::MatrixXf(STATE_SIZE, 1);
    mean_ << 0, 0, 0, 0, 0, 0;

    cov_ = Eigen::MatrixXf(STATE_SIZE, STATE_SIZE);
    cov_ << 0.1, 0, 0, 0, 0, 0,
            0, 0.1, 0, 0, 0, 0,
            0, 0, 0.1, 0, 0, 0,
            0, 0, 0, 0.1, 0, 0,
            0, 0, 0, 0, 0.1, 0,
            0, 0, 0, 0, 0, 0.1;
    
    meas_ = Eigen::MatrixXf(MEAS_SIZE, 1);
    meas_ << 0, 0, 0;
}

void KalmanFilter::predict() {
    mean_ = A_ * mean_;
    cov_ = A_ * cov_ * A_.transpose() + R_;
}

void KalmanFilter::update(Eigen::MatrixXf meas) {
    Eigen::MatrixXf y = meas - C_ * mean_;
    Eigen::MatrixXf S = C_ * cov_ * C_.transpose() + Q_;
    Eigen::MatrixXf K = cov_ * C_.transpose() * S.inverse();

    mean_ = mean_ + K * y;
    cov_ = (Eigen::MatrixXf::Identity(STATE_SIZE, STATE_SIZE) - K * C_) * cov_;
}

Eigen::MatrixXf KalmanFilter::getMean() {
    return mean_;
}

Eigen::MatrixXf KalmanFilter::getCov() {
    return cov_;
}

Eigen::MatrixXf KalmanFilter::getMeas() {
    return meas_;
}

QPair<float, float> KalmanFilter::getPosition() {
    return QPair<float, float>(mean_(X), mean_(Y));
}

QPair<float, float> KalmanFilter::getVelocity() {
    return QPair<float, float>(mean_(VX), mean_(VY));
}

float KalmanFilter::getOrientation() {
    return mean_(THETA);
}

float KalmanFilter::getAngularVelocity() {
    return mean_(OMEGA);
}