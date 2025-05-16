#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QPluginLoader>
#include <QHBoxLayout>
#include <QDebug>
#include <QInputDialog>
#include <QMouseEvent>
#include <ImageView.h>
#include <Helper.h>
#include <QPluginLoader>
#include <QMap>
#include <PhotoEditorPluginInterface.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void InitUI();
    void CreateActions();
    void ShowImage(QString path);
    void UpdateView(QPixmap pixmap);
    void LoadPlugins();

private slots:
    void OpenImage();
    void SaveImageAs();
    void ZoomInImage();
    void ZoomOutImage();
    void PreviousImage();
    void NextImage();
    void About();
    void RotateImage();
    void ResizeImage();
    void CropImage();
    void PluginPerform();

private:
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *settingMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *editToolBar;

    QGraphicsScene *imageScene;
    ImageView *imageView;

    QStatusBar *imageStatusBar;
    QLabel *imageStatusLabel;

    QGraphicsPixmapItem *currentImage;

    QString currentImagePath;

    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *previousImageAction;
    QAction *nextImageAction;
    QAction *aboutAction;
    QAction *rotateImageAction;
    QAction *resizeImageAction;
    QAction *cropImageAction;

    QMap<QString, PhotoEditorPluginInterface*> editPlugins;

    double currentAngle = 0.0;
    cv::Mat originalImage;
    cv::Mat editedImage;
};
#endif // MAINWINDOW_H
