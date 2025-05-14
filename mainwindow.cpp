#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    currentImage = nullptr;
    qDebug() << CV_VERSION;
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

    // TODO: Add change resource EN <-> VN
    settingMenu = menuBar()->addMenu("&Setting");

    helpMenu = menuBar()->addMenu("&Help");

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

    // Alow Single File
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Supported Image Files (*.png *.jpg)"));
    QStringList filePaths;

    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
    }
}

void MainWindow::saveImageAs() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Information", "Nothing to save!");
        return;
    }

    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Image As ...");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Images (*.png *.jpg)"));

    QStringList fileNames;

    if(dialog.exec()) {
        QString savePath = dialog.selectedFiles().at(0);
        fileNames = dialog.selectedFiles();
        static QRegularExpression re(".+\\.(png|bmp|jpg)");
        if(re.match(fileNames.at(0)).hasMatch()) {
            currentImage->pixmap().save(fileNames.at(0));
            QMessageBox::information(this, "Save Successful", "The image has been saved successfully at: " + savePath);
        } else {
            QMessageBox::information(this, "Save Failed", "The image could not be saved. Please check the file name and format (PNG, JPG).");
        }
    }
}

void MainWindow::zoomInImage() {
    imageView->scale(1.2, 1.2);
}

void MainWindow::zoomOutImage() {
    imageView->scale(0.8, 0.8);
}

void MainWindow::previousImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    // Take files only and sort by name
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    // Get the index of the current files (after remove extension)
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index > 0) {
        showImage(dir.absoluteFilePath(fileNames.at(index - 1)));
    } else {
        QMessageBox::information(this, "Navigation", "This is the first image in the folder. There are no previous images to display.");
    }
}

void MainWindow::nextImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    // Take files only and sort by name
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    // Get the index of the current files (after remove extension)
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index < fileNames.length() - 1) {
        showImage(dir.absoluteFilePath(fileNames.at(index + 1)));
    } else {
        QMessageBox::information(this, "Navigation", "This is the last image in the folder. There are no more images to display.");
    }
}

void MainWindow::about() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About");

    QString text =
        "<b>Photo Editor</b><br>"
        "Developed by Nguyen Minh Duc<br><br>"
        "This application was developed as a mini project for the VDT2025 program.<br>"
        "Github Repository: <a href='https://github.com/PinkRex/PhotoEditor.git'>Pinkrex/PhotoEditor</a>";

    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);
    msgBox.setText(text);
    msgBox.exec();
}

void MainWindow::rotateImage() {
    // if (currentImage == nullptr) {
    //     QMessageBox::information(this, "Information", "No image to edit.");
    //     return;
    // }

    // QPixmap pixmap = currentImage->pixmap();
    // QImage image = pixmap.toImage();
    // image = image.convertToFormat(QImage::Format_RGB888);

    // cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());

    // double angle = 45.0;
    // double scale = 1.0;
    // cv::Point2f center = cv::Point(mat.cols/2, mat.rows/2);
    // cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, angle, scale);
    // cv::Mat result;
    // cv::warpAffine(mat, result, rotateMatrix, mat.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    // QImage image_edited(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    // pixmap = QPixmap::fromImage(image_edited);
    // imageScene->clear();
    // imageView->resetTransform();
    // currentImage = imageScene->addPixmap(pixmap);
    // imageScene->update();
    // imageView->setSceneRect(pixmap.rect());
    // QString status = QString("(eddited image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
    // imageStatusLabel->setText(status);



        if(currentImage == nullptr) {
            QMessageBox::information(this, "Information", "No image to edit.");
            return;
        }
        QPixmap pixmap = currentImage->pixmap();
        QImage image = pixmap.toImage();
        image = image.convertToFormat(QImage::Format_RGB888);
        cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());
        cv::Mat tmp;
        cv::blur(mat, tmp, cv::Size(8, 8));
        mat = tmp;
        QImage image_blurred(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(image_blurred);
        imageScene->clear();
        imageView->resetTransform();
        currentImage = imageScene->addPixmap(pixmap);
        imageScene->update();
        imageView->setSceneRect(pixmap.rect());
        QString status = QString("(editted image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
        imageStatusLabel->setText(status);
}
