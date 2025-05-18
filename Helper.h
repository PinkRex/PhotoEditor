#ifndef HELPER_H
#define HELPER_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <mainwindow.h>

class Helper {
public:
    static QPixmap CvMatToQPixmap(cv::Mat matImage);
    static void UpdateView(MainWindow *mainWindow, cv::Mat mat);
    static bool CheckImageValid(MainWindow *mainWindow);
    static void ToggleCropMode(MainWindow *mainWindow, bool mode, bool draw);
};

#endif // HELPER_H
