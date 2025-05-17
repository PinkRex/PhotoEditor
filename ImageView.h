#pragma once
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QImageReader>
#include <QUrl>
#include <QDebug>

class ImageView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    QRect getSelectionRect() const;
    void toggleDrawingMode(bool enabled);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *envet) override;
    void dropEvent(QDropEvent *event) override;

private:
    QPoint origin;
    QRect selectionRect;
    bool selecting = false;
    bool drawingMode = false;

signals:
    void imageDropped(const QString &filePath);

};
