#ifndef SSLLOGCLIENT_HH
#define SSLLOGCLIENT_HH

#include <QThread>
#include <QTimer>

#include <SSLLogClient/modules/gui/gui.hh>
#include <SSLLogClient/modules/kalmanfilter/kalmanfilter.hh>
#include <SSLLogClient/modules/logreader/logreader.hh>
#include <SSLLogClient/modules/tracker/trackeddetection.hh>
#include <SSLLogClient/types/worlddata.hh>

class SSLLogClient : public QObject {
    Q_OBJECT
public:
    SSLLogClient();
    ~SSLLogClient();

    SSLLogClient &readFrom(QString fileName);
    SSLLogClient &show();
    void start();
    void stop();

private:
    TrackedDetection trackedDetection_;
    GUI *gui_;
    LogReader *logReader_;
    QThread *logReaderThread_;
    WorldData world_;

    bool isRunning_;
    QTimer *timer_;

    void gameLoop();
};

#endif // SSLLOGCLIENT_HH