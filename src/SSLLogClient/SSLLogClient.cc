#include <SSLLogClient/SSLLogClient.hh>

#define FREQUENCY 60.0

SSLLogClient::SSLLogClient()
    : isRunning_(false), gui_(nullptr), logReader_(nullptr), trackedDetection_(world_) {}

SSLLogClient::~SSLLogClient() {
    if (gui_) {
        delete gui_;
    }
    if (logReader_) {
        delete logReader_;
    }
}

SSLLogClient &SSLLogClient::readFrom(QString fileName) {
    logReader_ = new LogReader(fileName);
    return *this;
}

SSLLogClient &SSLLogClient::show() {
    gui_ = new GUI(world_);
    gui_->resize(640, 480);
    gui_->setWindowTitle("SSL Log Client");
    gui_->show();

    return *this;
}

void SSLLogClient::start() {
    if (!logReader_) {
        return;
    }
    if (!gui_) {
        return;
    }

    isRunning_ = true;
    logReaderThread_ = new QThread(this);
    logReader_->moveToThread(logReaderThread_);
    connect(logReaderThread_, &QThread::started, logReader_, &LogReader::read);
    connect(logReader_, &LogReader::finished, logReaderThread_, &QThread::quit);
    connect(logReader_, &LogReader::finished, logReader_, &LogReader::deleteLater);
    connect(logReaderThread_, &QThread::finished, logReaderThread_, &QThread::deleteLater);

    // Start the thread
    logReaderThread_->start();

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &SSLLogClient::gameLoop);
    timer_->start(1000.0 / FREQUENCY);
}

void SSLLogClient::gameLoop() {
    trackedDetection_.process(logReader_->getDetection());
    gui_->update();
}

void SSLLogClient::stop() {
    if (!isRunning_) {
        return;
    }
    timer_->stop();
    isRunning_ = false;
    if (logReaderThread_) {
        logReaderThread_->quit();
        logReaderThread_->wait();
    }
}