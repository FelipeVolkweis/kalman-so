#include <QDebug>

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

    file_.read(reinterpret_cast<char *>(&header), sizeof(header));
    file_.read(reinterpret_cast<char *>(&header), sizeof(header));

    while (!file_.atEnd()) {
        file_.read(reinterpret_cast<char *>(&message), sizeof(message));

        switch (message.messageType) {
        case LogTypes::MESSAGE_SSL_VISION_2010:
            detectionMutex_.lock();
            detection_.ParseFromArray(message.protobufMessage.data(), message.sizeOfMessage);
            detectionMutex_.unlock();
            break;
        case LogTypes::MESSAGE_SSL_VISION_2014:
            geometryMutex_.lock();
            geometry_.ParseFromArray(message.protobufMessage.data(), message.sizeOfMessage);
            geometryMutex_.unlock();
            break;
        default:
            break;
        }
    }
}

SSL_DetectionFrame LogReader::getDetection() {
    QMutexLocker locker(&detectionMutex_);
    return detection_;
}

SSL_GeometryData LogReader::getGeometry() {
    QMutexLocker locker(&geometryMutex_);
    return geometry_;
}