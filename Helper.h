#ifndef HELPER_H
#define HELPER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Helper {
public:
    static QPixmap CvMatToQPixmap(cv::Mat matImage);
};

#endif // HELPER_H
