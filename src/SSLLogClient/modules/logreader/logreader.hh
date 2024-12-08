#ifndef LOGREADER_HH
#define LOGREADER_HH

#include <3rdparty/proto/pb/ssl_vision_detection.pb.h>
#include <3rdparty/proto/pb/ssl_vision_geometry.pb.h>
#include <3rdparty/proto/pb/ssl_vision_wrapper.pb.h>

#include <QFile>
#include <QMutex>
#include <QString>

// Frequência com que os dados serão "postados" pelo leitor de logs
// afeta a velocidade com que novas atualizações são mostradas na GUI
#define DETECTION_FREQUENCY 60 // Hz

// Explicado melhor em
// https://ssl.robocup.org/game-logs/
namespace LogTypes {
enum {
    MESSAGE_BLANK = 0,
    MESSAGE_UNKNOWN = 1,
    MESSAGE_SSL_VISION_2010 = 2,
    MESSAGE_SSL_REFBOX_2013 = 3,
    MESSAGE_SSL_VISION_2014 = 4,
    MESSAGE_SSL_VISION_TRACKER_2020 = 5,
    MESSAGE_SSL_INDEX_2021 = 6
};
}

struct LogHeader {
    char logType[12]; // SSL_LOG_FILE (12 bytes)
    qint32 version;
};

struct LogMessage {
    qint64 receiverTimestamp;
    qint32 messageType;
    qint32 sizeOfMessage;
    QByteArray protobufMessage;
};

class LogReader : public QObject {
    Q_OBJECT
public:
    LogReader(QString fileName);
    void read();

    SSL_DetectionFrame getDetection();
    SSL_GeometryData getGeometry();

signals:
    void finished();

private:
    QMutex detectionMutex_;
    SSL_DetectionFrame detection_;
    QMutex geometryMutex_;
    SSL_GeometryData geometry_;

    QFile file_;
};

#endif