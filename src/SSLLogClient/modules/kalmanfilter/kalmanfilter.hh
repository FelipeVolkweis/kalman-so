#ifndef KALMANFILTER_HH
#define KALMANFILTER_HH

#include <eigen3/Eigen/Dense>

#include <QPair>

class KalmanFilter {
public:
    const int STATE_SIZE = 6;
    const int MEAS_SIZE = 3;

    const int X = 0;
    const int Y = 1;
    const int THETA = 2;
    const int VX = 3;
    const int VY = 4;
    const int OMEGA = 5;

    KalmanFilter();
    void predict();
    void update(Eigen::MatrixXf meas);
    QPair<float, float> getPosition();
    QPair<float, float> getVelocity();
    float getOrientation();
    float getAngularVelocity();
    Eigen::MatrixXf getMean();
    Eigen::MatrixXf getCov();
    Eigen::MatrixXf getMeas();
private:
    Eigen::MatrixXf A_;
    Eigen::MatrixXf C_;
    Eigen::MatrixXf R_;
    Eigen::MatrixXf Q_;
    Eigen::MatrixXf mean_;
    Eigen::MatrixXf cov_;
    Eigen::MatrixXf meas_;
};

#endif // KALMANFILTER_HH