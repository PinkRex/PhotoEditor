#include "UIInitializer.h"

void UIInitializer::InitUI(MainWindow *mainWindow) {
    mainWindow->resize(800, 600);

    mainWindow->getFileMenu() = mainWindow->menuBar()->addMenu("&File");
    mainWindow->getViewMenu() = mainWindow->menuBar()->addMenu("&View");
    mainWindow->getEditMenu() = mainWindow->menuBar()->addMenu("&Edit");
    mainWindow->getHelpMenu() = mainWindow->menuBar()->addMenu("&Help");

    mainWindow->getFileToolBar() = mainWindow->addToolBar("File");
    mainWindow->getViewToolBar() = mainWindow->addToolBar("View");
    mainWindow->getEditToolBar() = mainWindow->addToolBar("Edit");
    mainWindow->addToolBarBreak(Qt::TopToolBarArea);
    mainWindow->getPluginToolBar() = mainWindow->addToolBar("Plugin");

    mainWindow->getImageScene() = new QGraphicsScene(mainWindow);
    mainWindow->getImageView() = new ImageView(mainWindow);
    mainWindow->getImageView()->setScene(mainWindow->getImageScene());
    mainWindow->getImageView()->setObjectName("imageView");
    mainWindow->setCentralWidget(mainWindow->getImageView());
    mainWindow->connect(mainWindow->getImageView(), &ImageView::imageDropped, mainWindow, &MainWindow::ShowImage);

    // Init Image Information Display
    mainWindow->getImageStatusBar() = mainWindow->statusBar();
    QWidget *container = new QWidget(mainWindow->getImageStatusBar());

    QWidget* leftSpacer = new QWidget();
    QWidget* rightSpacer = new QWidget();
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mainWindow->getImageStatusLabel() = new QLabel(mainWindow->getImageStatusBar());
    mainWindow->getImageStatusLabel()->setAlignment(Qt::AlignCenter);
    mainWindow->getImageStatusLabel()->setText("The image infomation will be shown here!");
    mainWindow->getImageStatusLabel()->setStyleSheet("QLabel { padding: 2px; }");
    mainWindow->getImageStatusLabel()->setMinimumHeight(30);
    QFont font = mainWindow->getImageStatusLabel()->font();
    font.setItalic(true);
    mainWindow->getImageStatusLabel()->setFont(font);

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(leftSpacer);
    layout->addWidget(mainWindow->getImageStatusLabel());
    layout->addWidget(rightSpacer);
    mainWindow->getImageStatusBar()->addPermanentWidget(container, 1);

    CreateActions(mainWindow);

    // Group File menu & File toolbar
    mainWindow->getFileMenu()->addAction(mainWindow->getOpenAction());
    mainWindow->getFileMenu()->addAction(mainWindow->getSaveAsAction());
    mainWindow->getFileMenu()->addAction(mainWindow->getExitAction());

    mainWindow->getFileToolBar()->addAction(mainWindow->getOpenAction());
    mainWindow->getFileToolBar()->addAction(mainWindow->getSaveAsAction());
    mainWindow->getFileToolBar()->addAction(mainWindow->getExitAction());

    // Group View menu & View toolbar
    mainWindow->getViewMenu()->addAction(mainWindow->getZoomInAction());
    mainWindow->getViewMenu()->addAction(mainWindow->getZoomOutAction());
    mainWindow->getViewMenu()->addAction(mainWindow->getUndoAction());
    mainWindow->getViewMenu()->addAction(mainWindow->getRedoAction());
    mainWindow->getViewMenu()->addAction(mainWindow->getPreviousImageAction());
    mainWindow->getViewMenu()->addAction(mainWindow->getNextImageAction());

    mainWindow->getViewToolBar()->addAction(mainWindow->getZoomInAction());
    mainWindow->getViewToolBar()->addAction(mainWindow->getZoomOutAction());
    mainWindow->getViewToolBar()->addAction(mainWindow->getUndoAction());
    mainWindow->getViewToolBar()->addAction(mainWindow->getRedoAction());
    mainWindow->getViewToolBar()->addAction(mainWindow->getPreviousImageAction());
    mainWindow->getViewToolBar()->addAction(mainWindow->getNextImageAction());

    // Group Edit menu & Edit toolbar
    mainWindow->getEditMenu()->addAction(mainWindow->getRotateImageAction());
    mainWindow->getEditMenu()->addAction(mainWindow->getResizeImageAction());
    mainWindow->getEditMenu()->addAction(mainWindow->getCropImageAction());
    mainWindow->getEditMenu()->addSeparator();

    mainWindow->getEditToolBar()->addAction(mainWindow->getRotateImageAction());
    mainWindow->getEditToolBar()->addAction(mainWindow->getResizeImageAction());
    mainWindow->getEditToolBar()->addAction(mainWindow->getCropImageAction());

    // Group Help menu
    mainWindow->getHelpMenu()->addAction(mainWindow->getAboutAction());
}

void UIInitializer::CreateActions(MainWindow *mainWindow) {
    mainWindow->getOpenAction() = new QAction("&Open", mainWindow);
    mainWindow->getSaveAsAction() = new QAction("&Save As", mainWindow);
    mainWindow->getExitAction() = new QAction("&Exit", mainWindow);
    mainWindow->getZoomInAction() = new QAction("&Zoom In", mainWindow);
    mainWindow->getZoomOutAction() = new QAction("&Zoom Out", mainWindow);
    mainWindow->getUndoAction() = new QAction("&Undo", mainWindow);
    mainWindow->getRedoAction() = new QAction("&Redo", mainWindow);
    mainWindow->getPreviousImageAction() = new QAction("&Previous Image", mainWindow);
    mainWindow->getNextImageAction() = new QAction("&Next Image", mainWindow);
    mainWindow->getAboutAction() = new QAction("&About", mainWindow);
    mainWindow->getRotateImageAction() = new QAction("&Rotate", mainWindow);
    mainWindow->getResizeImageAction() = new QAction("&Resize", mainWindow);
    mainWindow->getCropImageAction() = new QAction("&Crop", mainWindow);

    mainWindow->connect(mainWindow->getOpenAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(OpenImage()));
    mainWindow->connect(mainWindow->getSaveAsAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(SaveImageAs()));
    mainWindow->connect(mainWindow->getExitAction(), SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    mainWindow->connect(mainWindow->getZoomInAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(ZoomInImage()));
    mainWindow->connect(mainWindow->getZoomOutAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(ZoomOutImage()));
    mainWindow->connect(mainWindow->getUndoAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(Undo()));
    mainWindow->connect(mainWindow->getRedoAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(Redo()));
    mainWindow->connect(mainWindow->getPreviousImageAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(PreviousImage()));
    mainWindow->connect(mainWindow->getNextImageAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(NextImage()));
    mainWindow->connect(mainWindow->getAboutAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(About()));
    mainWindow->connect(mainWindow->getRotateImageAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(RotateImage()));
    mainWindow->connect(mainWindow->getResizeImageAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(ResizeImage()));
    mainWindow->connect(mainWindow->getCropImageAction(), SIGNAL(triggered(bool)), mainWindow, SLOT(CropImage()));
}

void UIInitializer::LoadPlugins(MainWindow *mainWindow) {
    QDir pluginsDir(QApplication::instance()->applicationDirPath() + "/plugins");
    QStringList nameFilters;
    nameFilters << "*.dll";
    QFileInfoList plugins = pluginsDir.entryInfoList(nameFilters, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach(QFileInfo plugin, plugins) {
        QPluginLoader pluginLoader(plugin.absoluteFilePath(), mainWindow);
        PhotoEditorPluginInterface *plugin_ptr = dynamic_cast<PhotoEditorPluginInterface*>(pluginLoader.instance());
        if(plugin_ptr) {
            QAction *action = new QAction(plugin_ptr->name());
            mainWindow->getEditMenu()->addAction(action);
            mainWindow->getPluginToolBar()->addAction(action);
            mainWindow->getEditPlugins()[plugin_ptr->name()] = plugin_ptr;
            mainWindow->connect(action, SIGNAL(triggered(bool)), mainWindow, SLOT(PluginPerform()));
        } else {
            qDebug() << "bad plugin: " << plugin.absoluteFilePath();
        }
    }
}
