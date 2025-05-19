#include "ScreenshotCropper.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

ScreenshotCropper::ScreenshotCropper(const QPixmap &screenshot, QWidget *parent)
    : QWidget(parent), m_screenshot(screenshot) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);
    show();
}

void ScreenshotCropper::mousePressEvent(QMouseEvent *event) {
    m_startPoint = event->pos();
    m_endPoint = m_startPoint;
    m_isCropping = true;
    update();
}

void ScreenshotCropper::mouseMoveEvent(QMouseEvent *event) {
    if (m_isCropping) {
        m_endPoint = event->pos();
        update();
    }
}

void ScreenshotCropper::mouseReleaseEvent(QMouseEvent *event) {
    m_endPoint = event->pos();
    m_isCropping = false;
    update();

    QRect selected = getNormalizedRect();
    if (selected.width() > 5 && selected.height() > 5) {
        QPixmap cropped = m_screenshot.copy(selected);

        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString filePath = QString("C:/Users/ACER/OneDrive/Documents/Testing/ScreenShots/cropped_%1.png").arg(timestamp);
        cropped.save(filePath);
        QMessageBox::information(this, "Save", "The image has been saved successfully at: " + filePath);
        QImage croppedImage = cropped.toImage().convertToFormat(QImage::Format_BGR888);
        cv::Mat mat(croppedImage.height(), croppedImage.width(), CV_8UC3, (void*)croppedImage.bits(), croppedImage.bytesPerLine());
        cv::Mat matCopy = mat.clone();

        emit croppedMatReady(matCopy);
    }

    close();
}

void ScreenshotCropper::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_screenshot);

    if (m_isCropping || !getNormalizedRect().isNull()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(QColor(255, 0, 0, 50));
        painter.drawRect(getNormalizedRect());
    }
}

QRect ScreenshotCropper::getNormalizedRect() const {
    int x1 = qMin(m_startPoint.x(), m_endPoint.x());
    int y1 = qMin(m_startPoint.y(), m_endPoint.y());
    int x2 = qMax(m_startPoint.x(), m_endPoint.x());
    int y2 = qMax(m_startPoint.y(), m_endPoint.y());
    return QRect(QPoint(x1, y1), QPoint(x2, y2));
}
