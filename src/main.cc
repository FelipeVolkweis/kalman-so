#include <QApplication>
#include <QDebug>

#include <SSLLogClient/SSLLogClient.hh>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (argc != 2) {
        qDebug() << "Usage: " << argv[0] << " <log file>";
        return 1;
    }

    QString logFile(argv[1]);
    SSLLogClient client;
    client
        .readFrom(logFile)
        .show()
        .start();

    return app.exec();
}