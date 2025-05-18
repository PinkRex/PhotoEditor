#include "Helper.h"

QPixmap Helper::CvMatToQPixmap(cv::Mat matImage) {
    QImage image_edited;

    if (matImage.channels() == 4) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGRA2RGBA);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGBA8888).copy();
    } else if (matImage.channels() == 3) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGR2RGB);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888).copy();
    } else if (matImage.channels() == 1) {
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_Grayscale8).copy();
    } else {
        image_edited = QImage();
    }

    return QPixmap::fromImage(image_edited);
}

void Helper::UpdateView(MainWindow *mainWindow, cv::Mat mat) {
    QPixmap pixmap = CvMatToQPixmap(mat);
    mainWindow->getImageScene()->clear();
    mainWindow->getImageView()->resetTransform();
    mainWindow->getCurrentImage() = mainWindow->getImageScene()->addPixmap(pixmap);
    mainWindow->getImageScene()->update();
    mainWindow->getImageView()->setSceneRect(pixmap.rect());
    QString status = QString("(eddited image), %1x%2").arg(pixmap.width()).arg(pixmap.height());
    mainWindow->getImageStatusLabel()->setText(status);
}

bool Helper::CheckImageValid(MainWindow *mainWindow) {
    if (mainWindow->getCurrentImage() == nullptr) {
        QMessageBox::information(mainWindow, "Information", "No image to edit.");
        return false;
    }

    if (mainWindow->getEditedImage().empty()) {
        QString message = QString("Failed to load the image at: %1\n\nPlease check the file name and path (avoid using special or non-ASCII characters).").arg(mainWindow->getCurrentImagePath());
        QMessageBox::warning(mainWindow, "Error", message);
        return false;
    }

    return true;
}

void Helper::ToggleCropMode(MainWindow *mainWindow, bool mode, bool draw) {
    mainWindow->getCroppingMode() = mode;
    mainWindow->getHasSelection() = draw;
    mainWindow->getImageView()->toggleDrawingMode(mainWindow->getCroppingMode());
    mainWindow->getImageView()->setCropMode(mainWindow->getCroppingMode());
}
