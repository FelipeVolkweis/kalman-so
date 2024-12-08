#include <SSLLogClient/SSLLogClient.hh>

#define FREQUENCY 60.0

SSLLogClient::SSLLogClient() : isRunning_(false), gui_(nullptr), logReader_(nullptr) {}

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
    gui_ = new GUI();
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
    BallData ball{trackedDetection_.getBall().X, trackedDetection_.getBall().Y};
    gui_->setBall(ball);

    QVector<RobotData> blueTeam;
    for (const auto &blueRobot : trackedDetection_.getBlueTeam()) {
        RobotData guiRobot{blueRobot.get()->X, blueRobot.get()->Y, blueRobot.get()->THETA,
                           blueRobot.get()->id};
        blueTeam.push_back(guiRobot);
    }
    gui_->setBlueTeam(blueTeam);

    QVector<RobotData> yellowTeam;
    for (const auto &yellowRobot : trackedDetection_.getYellowTeam()) {
        RobotData guiRobot{yellowRobot.get()->X, yellowRobot.get()->Y, yellowRobot.get()->THETA,
                           yellowRobot.get()->id};
        yellowTeam.push_back(guiRobot);
    }
    gui_->setYellowTeam(yellowTeam);
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