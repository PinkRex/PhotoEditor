#ifndef DRAWING_INTERFACE_H
#define DRAWING_INTERFACE_H

#include <functional>
#include <QRect>

class DrawingInterface {
public:
    virtual void enableSelectionMode(bool enabled) = 0;
    virtual void onSelectionFinished(std::function<void(QRect)> callback) = 0;
    virtual ~DrawingInterface() = default;
};

#endif // DRAWING_INTERFACE_H
