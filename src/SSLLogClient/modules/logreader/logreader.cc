#include <QDebug>
#include <QtEndian>

#include <SSLLogClient/modules/logreader/logreader.hh>

LogReader::LogReader(QString fileName) : file_(fileName) {
    if (!file_.open(QIODevice::ReadOnly)) {
        qDebug() << "Não foi possível abrir o arquivo" << fileName;
    } else {
        qDebug() << "Arquivo aberto com sucesso" << fileName;
    }
}

void LogReader::read() {
    LogHeader header;
    LogMessage message;

    file_.read(reinterpret_cast<char *>(&(header.logType)), 12);
    char versionBuffer[sizeof(qint32)] = {0};
    file_.read(versionBuffer, sizeof(qint32));
    header.version = qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(versionBuffer));

    while (!file_.atEnd()) {
        char receiverTimestampBuffer[sizeof(qint64)] = {0};
        file_.read(receiverTimestampBuffer, sizeof(qint64));
        message.receiverTimestamp =
            qFromBigEndian<qint64>(reinterpret_cast<const uchar *>(receiverTimestampBuffer));

        char messageTypeBuffer[sizeof(qint32)] = {0};
        file_.read(messageTypeBuffer, sizeof(qint32));
        message.messageType =
            qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(messageTypeBuffer));

        char sizeOfMessageBuffer[sizeof(qint32)] = {0};
        file_.read(sizeOfMessageBuffer, sizeof(qint32));
        message.sizeOfMessage =
            qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(sizeOfMessageBuffer));

        message.protobufMessage = file_.read(message.sizeOfMessage);

        switch (message.messageType) {
        case LogTypes::MESSAGE_SSL_VISION_2010:
        case LogTypes::MESSAGE_SSL_VISION_2014: {
            SSL_WrapperPacket wrapper;
            wrapper.ParseFromArray(message.protobufMessage.data(), message.sizeOfMessage);
            if (wrapper.has_detection()) {
                detectionMutex_.lock();
                detection_ = wrapper.detection();
                detectionMutex_.unlock();
            }
            if (wrapper.has_geometry()) {
                geometryMutex_.lock();
                geometry_ = wrapper.geometry();
                geometryMutex_.unlock();
            }
            break;
        }
        default:
            break;
        }
    }

    emit finished();
}

SSL_DetectionFrame LogReader::getDetection() {
    QMutexLocker locker(&detectionMutex_);
    return detection_;
}

SSL_GeometryData LogReader::getGeometry() {
    QMutexLocker locker(&geometryMutex_);
    return geometry_;
}