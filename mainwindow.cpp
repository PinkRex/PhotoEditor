#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UIInitializer.h"
#include "Helper.h"
#include "ImageHistoryManager.h"
#include "ScreenshotCropper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UIInitializer::InitUI(this);
    currentImage = nullptr;
    UIInitializer::LoadPlugins(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowImage(QString path) {
    originalImage = cv::imread(path.toUtf8().constData(), cv::IMREAD_COLOR);
    editedImage = originalImage.clone();
    currentImagePath = path;

    QPixmap pixmap = Helper::CvMatToQPixmap(editedImage);
    Helper::UpdateView(this, editedImage);
    QString status = QString("%1, %2x%3, %4 Bytes").arg(path).arg(pixmap.width()).arg(pixmap.height()).arg(QFile(path).size());
    statusText = status;
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

void MainWindow::CropScreen() {
    this->hide();

    QTimer::singleShot(300, this, [=]() {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) return;

        QPixmap pixmap = screen->grabWindow(0);
        ScreenshotCropper *screenshot = new ScreenshotCropper(pixmap);
        this->show();
        connect(screenshot, &ScreenshotCropper::croppedMatReady, this, &MainWindow::handleCroppedScreen);
    });
}

void MainWindow::handleCroppedScreen(const cv::Mat &mat) {
    editedImage = mat;
    Helper::UpdateView(this, editedImage);
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

void MainWindow::Undo() {
    editedImage = ImageHistoryManager::undo(editedImage);
    Helper::UpdateView(this, editedImage);
}

void MainWindow::Redo() {
    editedImage = ImageHistoryManager::redo(editedImage);
    Helper::UpdateView(this, editedImage);
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
    if (!Helper::CheckImageValid(this)) {
        return;
    }

    ImageHistoryManager::push(editedImage);

    currentAngle += 90.0;
    if (currentAngle >= 360.0) {
        currentAngle -= 270.0;
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

    Helper::UpdateView(this, result);
}

void MainWindow::ResizeImage() {
    if (!Helper::CheckImageValid(this)) {
        return;
    }


    QPixmap OriginalPixmap = Helper::CvMatToQPixmap(editedImage);
    bool okWidth, okHeight;
    int width = QInputDialog::getInt(this, "Resize Image", "Enter new width:", OriginalPixmap.width(), 1, 10000, 1, &okWidth);
    if (!okWidth) return;

    int height = QInputDialog::getInt(this, "Resize Image", "Enter new height:", OriginalPixmap.height(), 1, 10000, 1, &okHeight);
    if (!okHeight) return;

    ImageHistoryManager::push(editedImage);

    cv::Mat destImage;
    cv::resize(editedImage, destImage, cv::Size(width, height));
    editedImage = destImage;

    Helper::UpdateView(this, destImage);
}

void MainWindow::CropImage() {
    if (!Helper::CheckImageValid(this)) {
        return;
    }

    ImageHistoryManager::push(editedImage);

    if (!croppingMode) {
        Helper::ToggleCropMode(this, true, false);
        QString status = QString("Draw a selection rectangle to crop.");
        imageStatusLabel->setText(status);
        return;
    }

    QRect selection = imageView->getSelectionRect();
    if (selection.isNull() || selection.width() == 0 || selection.height() == 0) {
        QMessageBox::information(this, "Information", "No selection made for cropping.");
        Helper::ToggleCropMode(this, false, false);
        imageStatusLabel->setText(statusText);
        return;
    }

    int imageWidth = editedImage.cols;
    int imageHeight = editedImage.rows;
    if (selection.right() >= imageWidth || selection.bottom() >= imageHeight || selection.x() < 0 || selection.y() < 0) {
        QMessageBox::warning(this, "Warning", "Selection area is outside the image bounds.");
        Helper::ToggleCropMode(this, false, false);
        return;
    }
    hasSelection = true;

    cv::Rect croppedArea(selection.x(), selection.y(), selection.width(), selection.height());
    cv::Mat croppedMat = editedImage(croppedArea).clone();
    editedImage = croppedMat;

    Helper::UpdateView(this, editedImage);
    Helper::ToggleCropMode(this, false, false);
}

void MainWindow::PluginPerform() {
    if (!Helper::CheckImageValid(this)) {
        return;
    }

    ImageHistoryManager::push(editedImage);

    QAction *active_action = qobject_cast<QAction*>(sender());
    PhotoEditorPluginInterface *plugin_ptr = editPlugins[active_action->text()];
    if(!plugin_ptr) {
        QMessageBox::information(this, "Information", "No plugin is found.");
        return;
    }

    plugin_ptr->edit(editedImage, editedImage, this);

    Helper::UpdateView(this, editedImage);
}
