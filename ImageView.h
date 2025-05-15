#pragma once
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

class ImageView : public QGraphicsView {
public:
    explicit ImageView(QWidget *parent = nullptr);

    QRect getSelectionRect() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

private:
    QPoint origin;
    QRect selectionRect;
    bool selecting = false;
};
