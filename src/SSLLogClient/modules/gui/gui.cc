#include <QMutexLocker>
#include <QPainter>
#include <QtMath>

#include <SSLLogClient/modules/gui/gui.hh>

void GUI::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Enable anti-aliasing for smoother rendering
    painter.setRenderHint(QPainter::Antialiasing);

    // Lock the mutex for thread safety
    QMutexLocker locker(&mutex_);

    // Define field dimensions in millimeters
    const double fieldWidth_mm = 9000.0;
    const double fieldHeight_mm = 6000.0;

    // Get widget dimensions
    int widgetWidth = this->width();
    int widgetHeight = this->height();

    // Calculate scale factors to fit the field within the widget while maintaining aspect ratio
    double scaleX = static_cast<double>(widgetWidth) / fieldWidth_mm;
    double scaleY = static_cast<double>(widgetHeight) / fieldHeight_mm;
    double scale = qMin(scaleX, scaleY);

    // Calculate margins to center the field if the aspect ratios do not match
    double scaledFieldWidth = fieldWidth_mm * scale;
    double scaledFieldHeight = fieldHeight_mm * scale;
    double marginX = (widgetWidth - scaledFieldWidth) / 2.0;
    double marginY = (widgetHeight - scaledFieldHeight) / 2.0;

    // Apply translation to center the field in the widget
    painter.translate(marginX + scaledFieldWidth / 2.0, marginY + scaledFieldHeight / 2.0);

    // Apply scaling
    painter.scale(scale, -scale); // Flip Y-axis to have positive Y upwards

    // Draw the background field
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);
    QRectF fieldRect(-fieldWidth_mm / 2.0, -fieldHeight_mm / 2.0, fieldWidth_mm, fieldHeight_mm);
    painter.drawRect(fieldRect);

    // Optional: Draw field borders
    painter.setPen(QPen(Qt::white, 10.0 / scale)); // Reduced border thickness
    painter.drawRect(fieldRect);

    // Function to draw robots
    auto drawRobots = [&](const QVector<RobotData> &team, QColor color) {
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 2.0 / scale)); // Reduced outline thickness

        for (const auto &robot : team) {
            painter.save(); // Save the current state

            // Translate to the robot's position
            painter.translate(robot.x, robot.y);

            // Rotate based on the robot's orientation
            painter.rotate(qRadiansToDegrees(robot.theta));

            // Draw the robot as a circle with a line indicating orientation
            double robotRadius_mm = 90.0;
            painter.drawEllipse(QPointF(0, 0), robotRadius_mm, robotRadius_mm); // Robot body
            painter.drawLine(QPointF(0, 0), QPointF(0, 2 * robotRadius_mm));    // Orientation line

            // Draw the robot's ID above the robot
            painter.setPen(Qt::white); // Text color
            QFont font = painter.font();
            font.setPointSizeF(12.0 / scale); // Adjust font size based on scale
            painter.setFont(font);
            QRectF textRect(-robotRadius_mm, -robotRadius_mm - 50.0, 2 * robotRadius_mm, 100.0);
            painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignBottom,
                             QString::number(robot.id));

            painter.restore(); // Restore to the previous state
        }
    };

    // **Drawing Blue Team Robots**
    drawRobots(blueTeam_, Qt::blue);

    // **Drawing Yellow Team Robots**
    drawRobots(yellowTeam_, Qt::yellow);

    // Draw the ball
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(Qt::black, 1.0 / scale)); // Further reduced outline thickness
    double ballRadius_mm = 21.5;
    painter.drawEllipse(QPointF(ball_.x, ball_.y), ballRadius_mm, ballRadius_mm);
}

void GUI::setBlueTeam(QVector<RobotData> &blueTeam) {
    QMutexLocker locker(&mutex_);
    blueTeam_ = blueTeam;
}

void GUI::setYellowTeam(QVector<RobotData> &yellowTeam) {
    QMutexLocker locker(&mutex_);
    yellowTeam_ = yellowTeam;
}

void GUI::setBall(BallData &ball) {
    QMutexLocker locker(&mutex_);
    ball_ = ball;
}