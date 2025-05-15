#include "imageview.h"

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent) {}

QRect ImageView::getSelectionRect() const {
    return mapToScene(selectionRect).boundingRect().toRect();
}

void ImageView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        origin = event->pos();
        selectionRect = QRect();
        selecting = true;
        viewport()->update();
    }
    QGraphicsView::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event) {
    if (selecting) {
        selectionRect = QRect(origin, event->pos()).normalized();
        viewport()->update();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event) {
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
