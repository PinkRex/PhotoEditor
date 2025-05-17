#ifndef PHOTO_EDITOR_PLUGIN_INTERFACE_H
#define PHOTO_EDITOR_PLUGIN_INTERFACE_H

#include <QObject>
#include <QString>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

class PhotoEditorPluginInterface {
public:
    virtual ~PhotoEditorPluginInterface() {};
    virtual QString name() = 0;
    virtual void edit(const cv::Mat &input, cv::Mat &output, QWidget* parent) = 0;
};

#define PHOTO_EDITOR_PLUGIN_INTERFACE_IID "com.nguyenminhduc.photoeditorplugininterface"
Q_DECLARE_INTERFACE(PhotoEditorPluginInterface, PHOTO_EDITOR_PLUGIN_INTERFACE_IID)

#endif // PHOTO_EDITOR_PLUGIN_INTERFACE_H
