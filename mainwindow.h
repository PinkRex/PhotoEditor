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
#include <QPluginLoader>
#include <QTimer>
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
    QMenu*& getPluginMenu() { return pluginMenu; }
    QMenu*& getHelpMenu() { return helpMenu; }

    QToolBar*& getFileToolBar() { return fileToolBar; }
    QToolBar*& getViewToolBar() { return viewToolBar; }
    QToolBar*& getEditToolBar() { return editToolBar; }
    QToolBar*& getPluginToolBar() { return pluginToolBar; }

    QGraphicsScene*& getImageScene() { return imageScene; }
    ImageView*& getImageView() { return imageView; }

    QStatusBar*& getImageStatusBar() { return imageStatusBar; }
    QLabel*& getImageStatusLabel() { return imageStatusLabel; }

    QAction*& getOpenAction() { return openAction; }
    QAction*& getCropScreenAction() {return cropScreenAction; }
    QAction*& getSaveAsAction() { return saveAsAction; }
    QAction*& getExitAction() { return exitAction; }
    QAction*& getZoomInAction() { return zoomInAction; }
    QAction*& getZoomOutAction() { return zoomOutAction; }
    QAction*& getUndoAction() { return undoAction; }
    QAction*& getRedoAction() { return redoAction; }
    QAction*& getPreviousImageAction() { return previousImageAction; }
    QAction*& getNextImageAction() { return nextImageAction; }
    QAction*& getAboutAction() { return aboutAction; }
    QAction*& getRotateImageAction() { return rotateImageAction; }
    QAction*& getResizeImageAction() { return resizeImageAction; }
    QAction*& getCropImageAction() { return cropImageAction; }
    QAction*& getLoadPluginsAction() { return loadPluginsAction; }
    QAction*& getUnloadPluginAction() { return unloadPluginsAction; }

    QMap<QString, PhotoEditorPluginInterface*>& getEditPlugins() { return editPlugins; }
    QGraphicsPixmapItem*& getCurrentImage() { return currentImage; }

    QString getCurrentImagePath() { return currentImagePath; }
    QString getStatusText() { return statusText; }
    cv::Mat getOriginalImage() { return originalImage; }
    cv::Mat getEditedImage() { return editedImage; }
    bool& getCroppingMode() { return croppingMode; }
    bool& getHasSelection() { return hasSelection; }

public:
    Ui::MainWindow *ui;
    void ShowImage(QString path);
    struct PluginInfo {
        QString name;
        QString filePath;
        QPluginLoader* loader;
        PhotoEditorPluginInterface* instance;
    };
    QList<PluginInfo> loadedPlugins;

private slots:
    void OpenImage();
    void CropScreen();
    void SaveImageAs();
    void ZoomInImage();
    void ZoomOutImage();
    void Undo();
    void Redo();
    void PreviousImage();
    void NextImage();
    void About();
    void RotateImage();
    void ResizeImage();
    void CropImage();
    void PluginPerform();
    void handleCroppedScreen(const cv::Mat &mat);
    void LoadPlugins();
    void UnloadPlugins();

private:
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *pluginMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *editToolBar;
    QToolBar *pluginToolBar;

    QGraphicsScene *imageScene;
    ImageView *imageView;

    QStatusBar *imageStatusBar;
    QLabel *imageStatusLabel;

    QGraphicsPixmapItem *currentImage;

    QString currentImagePath;
    QString statusText;

    QAction *openAction;
    QAction *cropScreenAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *previousImageAction;
    QAction *nextImageAction;
    QAction *aboutAction;

    QAction *rotateImageAction;
    QAction *resizeImageAction;
    QAction *cropImageAction;

    QAction *loadPluginsAction;
    QAction *unloadPluginsAction;

    QMap<QString, PhotoEditorPluginInterface*> editPlugins;

    double currentAngle = 0.0;
    cv::Mat originalImage;
    cv::Mat editedImage;
    bool croppingMode = false;
    bool hasSelection = false;
};
#endif // MAINWINDOW_H
