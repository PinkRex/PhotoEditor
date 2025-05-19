#ifndef HELPER_H
#define HELPER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsView>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "mainwindow.h"

class Helper {
public:
    static QPixmap CvMatToQPixmap(cv::Mat matImage);
    static void UpdateView(MainWindow *mainWindow, cv::Mat mat);
    static bool CheckImageValid(MainWindow *mainWindow);
    static void ToggleCropMode(MainWindow *mainWindow, bool mode, bool draw);
};

#endif // HELPER_H
