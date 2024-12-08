#ifndef KALMANFILTER_HH
#define KALMANFILTER_HH

#include <QPair>

#include <eigen3/Eigen/Dense>

class KalmanFilter {
public:
    int STATE_SIZE = 6;
    int MEAS_SIZE = 3;

    int X = 0;
    int Y = 1;
    int THETA = 2;
    int VX = 3;
    int VY = 4;
    int OMEGA = 5;

    KalmanFilter();
    void predict();
    void update(Eigen::MatrixXf meas);
    void update(float x, float y, float theta);
    void update(float x, float y);

    float getXYUncertainty() const;
    float getThetaUncertainty() const;
    float getVUncertainty() const;
    float getOmegaUncertainty() const;

    QPair<float, float> getPosition() const;
    QPair<float, float> getVelocity() const;
    float getOrientation() const;
    float getAngularVelocity() const;
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