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
    qDebug() << currentAngle;
}

class CustomGraphicsView : public QGraphicsView {
public:
    explicit CustomGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent), selecting(false) {}

    QRect getSelectionRect() const {
        return mapToScene(selectionRect).boundingRect().toRect();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            origin = event->pos();
            selectionRect = QRect();
            selecting = true;
            viewport()->update();
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (selecting) {
            selectionRect = QRect(origin, event->pos()).normalized();
            viewport()->update();
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && selecting) {
            selecting = false;
            viewport()->update();
        }
        QGraphicsView::mouseReleaseEvent(event);
    }

    void drawForeground(QPainter *painter, const QRectF &rect) override {
        Q_UNUSED(rect);
        if (!selectionRect.isNull()) {
            painter->setPen(QPen(Qt::red, 2));
            painter->drawRect(mapToScene(selectionRect).boundingRect());
        }
    }

private:
    QPoint origin;
    QRect selectionRect;
    bool selecting;
};


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
    imageView = new ImageView(this);
    imageView->setScene(imageScene);
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
    editMenu->addAction(resizeImageAction);
    editMenu->addAction(cropImageAction);

    editToolBar->addAction(rotateImageAction);
    editToolBar->addAction(resizeImageAction);
    editToolBar->addAction(cropImageAction);

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
    aboutAction = new QAction("&About", this);
    rotateImageAction = new QAction("&Rotate Image", this);
    resizeImageAction = new QAction("&Resize Image", this);
    cropImageAction = new QAction("&Crop Image", this);

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
    connect(resizeImageAction, SIGNAL(triggered(bool)), this, SLOT(resizeImage()));
    connect(cropImageAction, &QAction::triggered, this, &MainWindow::cropImage);
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
        "Developed by Nguyễn Minh Đức<br><br>"
        "This application was developed as a mini project for the VDT2025 program.<br>"
        "Github Repository: <a href='https://github.com/PinkRex/PhotoEditor.git'>Pinkrex/PhotoEditor</a>";

    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);
    msgBox.setText(text);
    msgBox.exec();
}

QPixmap CvMatToQpixmap(cv::Mat matImage) {
    QImage image_edited;

    if (matImage.channels() == 4) {
        cv::cvtColor(matImage, matImage, cv::COLOR_BGRA2RGBA);
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGBA8888).copy();
    } else if (matImage.channels() == 3) {
        cv::cvtColor(matImage, matImage, cv::COLOR_BGR2RGB);
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888).copy();
    } else if (matImage.channels() == 1) {
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_Grayscale8).copy();
    } else {
        image_edited = QImage();
    }

    return QPixmap::fromImage(image_edited);
}

void MainWindow::rotateImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Information", "No image to edit.");
        return;
    }

    currentAngle += 45.0;
    if (currentAngle >= 360.0) {
        currentAngle -= 360.0;
    }

    cv::Mat mat = cv::imread(currentImagePath.toUtf8().constData(), cv::IMREAD_COLOR);
    if (mat.empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(currentImagePath);
        QMessageBox::warning(this, "Error", message);
        return;
    }

    double scale = 1.0;
    cv::Point2f center = cv::Point(mat.cols / 2.0, mat.rows / 2.0);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, currentAngle, scale);

    cv::Rect2f boundingBox = cv::RotatedRect(cv::Point2f(), mat.size(), currentAngle).boundingRect2f();
    rotateMatrix.at<double>(0, 2) += boundingBox.width / 2.0 - mat.cols / 2.0;
    rotateMatrix.at<double>(1, 2) += boundingBox.height / 2.0 - mat.rows / 2.0;

    cv::Mat result;
    cv::warpAffine(mat, result, rotateMatrix, boundingBox.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    QPixmap pixmap = CvMatToQpixmap(result);

    imageScene->clear();
    imageView->resetTransform();
    currentImage = imageScene->addPixmap(pixmap);
    imageScene->update();
    imageView->setSceneRect(pixmap.rect());

    QString status = QString("(eddited image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
    imageStatusLabel->setText(status);
}

void MainWindow::resizeImage() {
    if (currentImage == nullptr) {
        QMessageBox::warning(this, "Warning", "No image to edit.");
        return;
    }

    bool okWidth, okHeight;
    int width = QInputDialog::getInt(this, "Resize Image", "Enter new width:", 25, 1, 10000, 1, &okWidth);
    if (!okWidth) return;

    int height = QInputDialog::getInt(this, "Resize Image", "Enter new height:", 25, 1, 10000, 1, &okHeight);
    if (!okHeight) return;

    cv::Mat mat = cv::imread(currentImagePath.toUtf8().constData(), cv::IMREAD_COLOR);
    if (mat.empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(currentImagePath);
        QMessageBox::warning(this, "Error", message);
        return;
    }

    cv::Mat destImage;
    cv::resize(mat, destImage, cv::Size(width, height));

    QPixmap pixmap = CvMatToQpixmap(destImage);

    imageScene->clear();
    imageView->resetTransform();
    currentImage = imageScene->addPixmap(pixmap);
    imageScene->update();
    imageView->setSceneRect(pixmap.rect());
    QString status = QString("(eddited image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
    imageStatusLabel->setText(status);
}

void MainWindow::cropImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Information", "No image to crop.");
        return;
    }

    // Get Cropped Area
    QRect selection = imageView->getSelectionRect();
    if (selection.isNull() || selection.width() == 0 || selection.height() == 0) {
        QMessageBox::information(this, "Information", "No selection made for cropping.");
        return;
    }

    // Crop Image
    QPixmap originalPixmap = currentImage->pixmap();
    QPixmap croppedPixmap = originalPixmap.copy(selection);

    imageScene->clear();
    currentImage = imageScene->addPixmap(croppedPixmap);
    imageScene->update();
    imageView->setSceneRect(croppedPixmap.rect());
    QString status = QString("(eddited image), %1x%2").arg(croppedPixmap.width()).arg(croppedPixmap.height());
    imageStatusLabel->setText(status);
}
