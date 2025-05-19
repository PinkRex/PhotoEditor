#ifndef SCREENSHOTCROPPER_H
#define SCREENSHOTCROPPER_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

class ScreenshotCropper : public QWidget {
    Q_OBJECT

public:
    explicit ScreenshotCropper(const QPixmap &screenshot, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPixmap m_screenshot;
    QPoint m_startPoint;
    QPoint m_endPoint;
    bool m_isCropping = false;

    QRect getNormalizedRect() const;

signals:
    void croppedMatReady(const cv::Mat &mat);
};

#endif // SCREENSHOTCROPPER_H
