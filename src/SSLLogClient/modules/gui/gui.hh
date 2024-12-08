#ifndef GUI_HH
#define GUI_HH

#include <QMutex>
#include <QPainter>
#include <QVector>
#include <QWidget>

#include <SSLLogClient/types/balldata.hh>
#include <SSLLogClient/types/robotdata.hh>
#include <SSLLogClient/types/worlddata.hh>

class GUI : public QWidget {
public:
    GUI(WorldData &worldData, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMutex mutex_;
    WorldData &worldData_;
};

#endif // GUI_HH