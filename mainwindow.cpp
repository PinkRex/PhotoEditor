#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    currentImage = nullptr;
    LoadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUI(){
    this->resize(800, 600);

    // Init Menubar
    fileMenu = menuBar()->addMenu("&File");
    viewMenu = menuBar()->addMenu("&View");
    editMenu = menuBar()->addMenu("&Edit");
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

    CreateActions();

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
    editMenu->addSeparator();

    editToolBar->addAction(rotateImageAction);
    editToolBar->addAction(resizeImageAction);
    editToolBar->addAction(cropImageAction);

    // Group Help menu
    helpMenu->addAction(aboutAction);
}

void MainWindow::CreateActions() {
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

    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(OpenImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(SaveImageAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(ZoomInImage()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(ZoomOutImage()));
    connect(previousImageAction, SIGNAL(triggered(bool)), this, SLOT(PreviousImage()));
    connect(nextImageAction, SIGNAL(triggered(bool)), this, SLOT(NextImage()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(About()));
    connect(rotateImageAction, SIGNAL(triggered(bool)), this, SLOT(RotateImage()));
    connect(resizeImageAction, SIGNAL(triggered(bool)), this, SLOT(ResizeImage()));
    connect(cropImageAction, SIGNAL(triggered(bool)), this, SLOT(CropImage()));
}

void MainWindow::UpdateView(QPixmap pixmap) {
    imageScene->clear();
    imageView->resetTransform();
    currentImage = imageScene->addPixmap(pixmap);
    imageScene->update();
    imageView->setSceneRect(pixmap.rect());
    QString status = QString("(eddited image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
    imageStatusLabel->setText(status);
}

void MainWindow::LoadPlugins() {
    QDir pluginsDir(QApplication::instance()->applicationDirPath() + "/plugins");
    QStringList nameFilters;
    nameFilters << "*.dll";
    QFileInfoList plugins = pluginsDir.entryInfoList(nameFilters, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach(QFileInfo plugin, plugins) {
        QPluginLoader pluginLoader(plugin.absoluteFilePath(), this);
        PhotoEditorPluginInterface *plugin_ptr = dynamic_cast<PhotoEditorPluginInterface*>(pluginLoader.instance());
        if(plugin_ptr) {
            QAction *action = new QAction(plugin_ptr->name());
            editMenu->addAction(action);
            editToolBar->addAction(action);
            editPlugins[plugin_ptr->name()] = plugin_ptr;
            connect(action, SIGNAL(triggered(bool)), this, SLOT(PluginPerform()));
        } else {
            qDebug() << "bad plugin: " << plugin.absoluteFilePath();
        }
    }
}

void MainWindow::ShowImage(QString path) {
    originalImage = cv::imread(path.toUtf8().constData(), cv::IMREAD_COLOR);
    editedImage = originalImage.clone();
    currentImagePath = path;

    QPixmap pixmap = Helper::CvMatToQPixmap(editedImage);
    UpdateView(pixmap);
    QString status = QString("%1, %2x%3, %4 Bytes")
                         .arg(path)
                         .arg(pixmap.width())
                         .arg(pixmap.height())
                         .arg(QFile(path).size());
    imageStatusLabel->setText(status);
}

void MainWindow::OpenImage() {
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Supported Image Files (*.png *.jpg)"));
    QStringList filePaths;

    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();
        ShowImage(filePaths.at(0));
    }
}

void MainWindow::SaveImageAs() {
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

void MainWindow::ZoomInImage() {
    imageView->scale(1.2, 1.2);
}

void MainWindow::ZoomOutImage() {
    imageView->scale(0.8, 0.8);
}

void MainWindow::PreviousImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index > 0) {
        ShowImage(dir.absoluteFilePath(fileNames.at(index - 1)));
    } else {
        QMessageBox::information(this, "Navigation", "This is the first image in the folder. There are no previous images to display.");
    }
}

void MainWindow::NextImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index < fileNames.length() - 1) {
        ShowImage(dir.absoluteFilePath(fileNames.at(index + 1)));
    } else {
        QMessageBox::information(this, "Navigation", "This is the last image in the folder. There are no more images to display.");
    }
}

void MainWindow::About() {
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

void MainWindow::RotateImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Information", "No image to edit.");
        return;
    }

    currentAngle += 90.0;
    if (currentAngle >= 360.0) {
        currentAngle -= 270.0;
    }

    if (editedImage.empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(currentImagePath);
        QMessageBox::warning(this, "Error", message);
        return;
    }

    double scale = 1.0;
    cv::Point2f center = cv::Point(editedImage.cols / 2.0, editedImage.rows / 2.0);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, currentAngle, scale);

    cv::Rect2f boundingBox = cv::RotatedRect(cv::Point2f(), editedImage.size(), currentAngle).boundingRect2f();
    rotateMatrix.at<double>(0, 2) += boundingBox.width / 2.0 - editedImage.cols / 2.0;
    rotateMatrix.at<double>(1, 2) += boundingBox.height / 2.0 - editedImage.rows / 2.0;

    cv::Mat result;
    cv::warpAffine(editedImage, result, rotateMatrix, boundingBox.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    editedImage = result.clone();
    QPixmap pixmap = Helper::CvMatToQPixmap(result);

    UpdateView(pixmap);
}

void MainWindow::ResizeImage() {
    bool okWidth, okHeight;
    int width = QInputDialog::getInt(this, "Resize Image", "Enter new width:", 25, 1, 10000, 1, &okWidth);
    if (!okWidth) return;

    int height = QInputDialog::getInt(this, "Resize Image", "Enter new height:", 25, 1, 10000, 1, &okHeight);
    if (!okHeight) return;

    if (editedImage.empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(currentImagePath);
        QMessageBox::warning(this, "Error", message);
        return;
    }

    cv::Mat destImage;
    cv::resize(editedImage, destImage, cv::Size(width, height));
    QPixmap pixmap = Helper::CvMatToQPixmap(destImage);
    editedImage = destImage;

    UpdateView(pixmap);
}

void MainWindow::CropImage() {
    QRect selection = imageView->getSelectionRect();
    if (selection.isNull() || selection.width() == 0 || selection.height() == 0) {
        QMessageBox::information(this, "Information", "No selection made for cropping.");
        return;
    }

    cv::Rect roi(selection.x(), selection.y(), selection.width(), selection.height());
    cv::Mat croppedMat = editedImage(roi).clone();
    editedImage = croppedMat;
    QPixmap pixmap = Helper::CvMatToQPixmap(editedImage);

    UpdateView(pixmap);
}

void MainWindow::PluginPerform() {
    if(currentImage == nullptr) {
        QMessageBox::information(this, "Information", "No image to edit.");
        return;
    }

    if (editedImage.empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(currentImagePath);
        QMessageBox::warning(this, "Error", message);
        return;
    }

    QAction *active_action = qobject_cast<QAction*>(sender());
    PhotoEditorPluginInterface *plugin_ptr = editPlugins[active_action->text()];
    if(!plugin_ptr) {
        QMessageBox::information(this, "Information", "No plugin is found.");
        return;
    }

    plugin_ptr->edit(editedImage, editedImage);
    QPixmap pixmap = Helper::CvMatToQPixmap(editedImage);

    UpdateView(pixmap);
}
