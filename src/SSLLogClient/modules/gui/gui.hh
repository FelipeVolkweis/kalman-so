#ifndef GUI_HH
#define GUI_HH

#include <QMutex>
#include <QPainter>
#include <QVector>
#include <QWidget>

#include <SSLLogClient/types/balldata.hh>
#include <SSLLogClient/types/robotdata.hh>

class GUI : public QWidget {
public:
    void setBlueTeam(QVector<RobotData> &blueTeam);
    void setYellowTeam(QVector<RobotData> &yellowTeam);
    void setBall(BallData &ball);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMutex mutex_;
    QVector<RobotData> blueTeam_;
    QVector<RobotData> yellowTeam_;
    BallData ball_;
};

#endif // GUI_HH