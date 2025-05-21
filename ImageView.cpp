#include "imageview.h"

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent) {
    setAcceptDrops(true);
}

void ImageView::toggleDrawingMode(bool enabled) {
    drawingMode = enabled;
    if (!enabled) {
        selectionRect = QRect();
        selecting = false;
        viewport()->update();
    }
}

QRect ImageView::getSelectionRect() const {
    return mapToScene(selectionRect).boundingRect().toRect();
}

void ImageView::setCropMode(bool enabled) {
    cropMode = enabled;
}

//======================================================//
// Handle draw the box outline
//======================================================//

void ImageView::mousePressEvent(QMouseEvent *event) {
    if (!drawingMode) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if (event->button() == Qt::LeftButton) {
        origin = event->pos();
        selectionRect = QRect();
        selecting = true;
        viewport()->update();
    }
    QGraphicsView::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event) {
    if (!drawingMode) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if (selecting) {
        selectionRect = QRect(origin, event->pos()).normalized();
        viewport()->update();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event) {
    if (!drawingMode) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if (event->button() == Qt::LeftButton && selecting) {
        selecting = false;
        viewport()->update();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void ImageView::drawForeground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);
    if (!selectionRect.isNull()) {
        QPen pen = QPen(Qt::red, 2);
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawRect(mapToScene(selectionRect).boundingRect());
    }
}

//======================================================//
// Handle Drag and Drop
//======================================================//

void ImageView::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        if (urlList.size() == 1) {
            QString fileName = urlList.at(0).toLocalFile();
            QImageReader reader(fileName);
            if (reader.canRead()) {
                event->acceptProposedAction();
            }
        }
    }
}

void ImageView::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void ImageView::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
    event->acceptProposedAction();
}

void ImageView::dropEvent(QDropEvent *event) {
    QList<QUrl> urlList = event->mimeData()->urls();
    if (!urlList.isEmpty()) {
        QString filePath = urlList.first().toLocalFile();
        emit imageDropped(filePath);
    }
}
