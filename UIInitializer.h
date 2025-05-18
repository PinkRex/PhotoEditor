#ifndef UI_INITIALIZER_H
#define UI_INITIALIZER_H

#include <mainwindow.h>
#include <QToolBar>
#include <QMenuBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QPluginLoader>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMouseEvent>
#include <QPluginLoader>
#include <QMap>
#include <QWidget>
#include <QApplication>

class UIInitializer {
public:
    static void InitUI(MainWindow *mainWindow);
    static void CreateActions(MainWindow *mainWindow);
    static void LoadPlugins(MainWindow *mainWindow);
};

#endif // UI_INITIALIZER_H
