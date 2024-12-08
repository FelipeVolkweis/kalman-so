#include <QMutexLocker>
#include <QPainter>
#include <QtMath>

#include <SSLLogClient/modules/gui/gui.hh>

GUI::GUI(WorldData &worldData, QWidget *parent) : QWidget(parent), worldData_(worldData) {}

void GUI::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QMutexLocker locker(&mutex_);

    const double fieldWidth_mm = 9000.0;
    const double fieldHeight_mm = 6000.0;

    int widgetWidth = this->width();
    int widgetHeight = this->height();

    double scaleX = static_cast<double>(widgetWidth) / fieldWidth_mm;
    double scaleY = static_cast<double>(widgetHeight) / fieldHeight_mm;
    double scale = qMin(scaleX, scaleY);

    double scaledFieldWidth = fieldWidth_mm * scale;
    double scaledFieldHeight = fieldHeight_mm * scale;
    double marginX = (widgetWidth - scaledFieldWidth) / 2.0;
    double marginY = (widgetHeight - scaledFieldHeight) / 2.0;

    painter.translate(marginX + scaledFieldWidth / 2.0, marginY + scaledFieldHeight / 2.0);

    // Apply scaling
    painter.scale(scale, -scale); // Flip Y-axis to have positive Y upwards

    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);
    QRectF fieldRect(-fieldWidth_mm / 2.0, -fieldHeight_mm / 2.0, fieldWidth_mm, fieldHeight_mm);
    painter.drawRect(fieldRect);

    painter.setPen(QPen(Qt::white, 10.0 / scale)); // Reduced border thickness
    painter.drawRect(fieldRect);

    // Function to draw robots
    auto drawRobots = [&](QColor color) {
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 2.0 / scale)); // Reduced outline thickness

        QVector<int> availableRobots;
        if (color == Qt::blue) {
            availableRobots = worldData_.getBlueActiveRobots();
        } else {
            availableRobots = worldData_.getYellowActiveRobots();
        }
        for (auto id : availableRobots) {
            RobotData robot;
            if (color == Qt::blue) {
                robot = worldData_.getBlueRobot(id);
            } else {
                robot = worldData_.getYellowRobot(id);
            }
            painter.save(); // Save the current state

            painter.translate(robot.x, robot.y);

            painter.rotate(qRadiansToDegrees(robot.theta));

            double robotRadius_mm = 90.0;
            painter.drawEllipse(QPointF(0, 0), robotRadius_mm, robotRadius_mm); // Robot body
            painter.drawLine(QPointF(0, 0), QPointF(0, 2 * robotRadius_mm));    // Orientation line

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
    drawRobots(Qt::blue);

    // **Drawing Yellow Team Robots**
    drawRobots(Qt::yellow);

    // Draw the ball
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(Qt::black, 1.0 / scale));
    double ballRadius_mm = 21.5;
    auto data = worldData_.getBall();
    painter.drawEllipse(QPointF(data.x, data.y), ballRadius_mm, ballRadius_mm);
}