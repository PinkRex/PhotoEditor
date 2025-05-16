#include "imageview.h"

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent) {
    setAcceptDrops(true);
}

void ImageView::setCroppingMode(bool enabled) {
    croppingMode = enabled;
    if (!enabled) {
        selectionRect = QRect();
        selecting = false;
        viewport()->update();
    }
}

QRect ImageView::getSelectionRect() const {
    return mapToScene(selectionRect).boundingRect().toRect();
}

void ImageView::mousePressEvent(QMouseEvent *event) {
    if (!croppingMode) {
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
    if (!croppingMode) {
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
    if (!croppingMode) {
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
        painter->setPen(QPen(Qt::red, 2));
        painter->drawRect(mapToScene(selectionRect).boundingRect());
    }
}

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
