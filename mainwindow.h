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
    QMenu*& getFileMenu() { return fileMenu; }
    QMenu*& getViewMenu() { return viewMenu; }
    QMenu*& getEditMenu() { return editMenu; }
    QMenu*& getHelpMenu() { return helpMenu; }

    QToolBar*& getFileToolBar() { return fileToolBar; }
    QToolBar*& getViewToolBar() { return viewToolBar; }
    QToolBar*& getEditToolBar() { return editToolBar; }

    QGraphicsScene*& getImageScene() { return imageScene; }
    ImageView*& getImageView() { return imageView; }

    QStatusBar*& getImageStatusBar() { return imageStatusBar; }
    QLabel*& getImageStatusLabel() { return imageStatusLabel; }

    QAction*& getOpenAction() { return openAction; }
    QAction*& getSaveAsAction() { return saveAsAction; }
    QAction*& getExitAction() { return exitAction; }
    QAction*& getZoomInAction() { return zoomInAction; }
    QAction*& getZoomOutAction() { return zoomOutAction; }
    QAction*& getPreviousImageAction() { return previousImageAction; }
    QAction*& getNextImageAction() { return nextImageAction; }
    QAction*& getAboutAction() { return aboutAction; }
    QAction*& getRotateImageAction() { return rotateImageAction; }
    QAction*& getResizeImageAction() { return resizeImageAction; }
    QAction*& getCropImageAction() { return cropImageAction; }

    QMap<QString, PhotoEditorPluginInterface*>& getEditPlugins() { return editPlugins; }

public:
    Ui::MainWindow *ui;
    void InitUI();
    void CreateActions();
    void ShowImage(QString path);
    void UpdateView(QPixmap pixmap);
    void LoadPlugins();
    bool CheckImageValid();
    void ToggleCropMode(bool mode, bool draw);

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

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *editToolBar;

    QGraphicsScene *imageScene;
    ImageView *imageView;

    QStatusBar *imageStatusBar;
    QLabel *imageStatusLabel;

    QGraphicsPixmapItem *currentImage;

    QString currentImagePath;
    QString statusText;

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
    bool croppingMode = false;
    bool hasSelection = false;
};
#endif // MAINWINDOW_H
