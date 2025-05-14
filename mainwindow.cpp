#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    currentImage = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI(){
    this->resize(800, 600);

    // Init Menubar
    fileMenu = menuBar()->addMenu("&File");
    viewMenu = menuBar()->addMenu("&View");
    editMenu = menuBar()->addMenu("&Edit");
    helpMenu = menuBar()->addMenu("&Help");

    // TODO: Add change resource EN <-> VN
    settingMenu = menuBar()->addMenu("&Setting");

    // Init Toolbar
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("View");
    editToolBar = addToolBar("Edit");

    // Init Image Display Area
    imageScene = new QGraphicsScene(this);
    imageView = new QGraphicsView(imageScene);
    setCentralWidget(imageView);

    // Init Image Information Display
    imageStatusBar = statusBar();
    QWidget *container = new QWidget(imageStatusBar);

    QWidget* leftSpacer = new QWidget();
    QWidget* rightSpacer = new QWidget();
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    imageStatusLabel = new QLabel(imageStatusBar);
    imageStatusLabel->setAlignment(Qt::AlignCenter);
    imageStatusLabel->setText("The image infomation will be shown here!");
    imageStatusLabel->setStyleSheet("QLabel { padding: 2px; }");
    imageStatusLabel->setMinimumHeight(30);
    QFont font = imageStatusLabel->font();
    font.setItalic(true);
    imageStatusLabel->setFont(font);

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(leftSpacer);
    layout->addWidget(imageStatusLabel);
    layout->addWidget(rightSpacer);
    imageStatusBar->addPermanentWidget(container, 1);

    // Create Actions
    createActions();

    // Group File menu & File toolbar
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAsAction);
    fileToolBar->addAction(exitAction);

    // Group View menu & View toolbar
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);
    viewMenu->addAction(previousImageAction);
    viewMenu->addAction(nextImageAction);

    viewToolBar->addAction(zoomInAction);
    viewToolBar->addAction(zoomOutAction);
    viewToolBar->addAction(previousImageAction);
    viewToolBar->addAction(nextImageAction);

    // Group Edit menu & Edit toolbar
    editMenu->addAction(rotateImageAction);
    editToolBar->addAction(rotateImageAction);

    // Group Help menu
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions() {
    // Init Action Text
    openAction = new QAction("&Open", this);
    saveAsAction = new QAction("&Save As", this);
    exitAction = new QAction("&Exit", this);
    zoomInAction = new QAction("&Zoom In", this);
    zoomOutAction = new QAction("&Zoom Out", this);
    previousImageAction = new QAction("&Previous Image", this);
    nextImageAction = new QAction("&Next Image", this);
    rotateImageAction = new QAction("&Rotate Image", this);
    aboutAction = new QAction("&About", this);

    // Setup Action Slot
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveImageAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomInImage()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOutImage()));
    connect(previousImageAction, SIGNAL(triggered(bool)), this, SLOT(previousImage()));
    connect(nextImageAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    connect(rotateImageAction, SIGNAL(triggered(bool)), this, SLOT(rotateImage()));
}

void MainWindow::showImage(QString path) {
    // Remove Previous Image
    imageScene->clear();
    imageView->resetTransform();

    // Load Image
    QPixmap image(path);
    currentImage = imageScene->addPixmap(image);
    imageScene->update();
    imageView->setSceneRect(image.rect());

    // Display Image Infomation
    QString status = QString("%1, %2x%3, %4 Bytes")
                         .arg(path)
                         .arg(image.width())
                         .arg(image.height())
                         .arg(QFile(path).size());
    imageStatusLabel->setText(status);
    currentImagePath = path;
}

void MainWindow::openImage() {
    // Open File Explorer
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");

    // Alow Multiple Files
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Supported Image Files (*.png *.jpg)"));
    QStringList filePaths;

    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
    }
}

void MainWindow::saveImageAs() {

}

void MainWindow::zoomInImage() {

}

void MainWindow::zoomOutImage() {

}

void MainWindow::previousImage() {

}

void MainWindow::nextImage() {

}

void MainWindow::about() {

}

void MainWindow::rotateImage() {

}
