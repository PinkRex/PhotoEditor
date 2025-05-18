#ifndef IMAGE_HISTORY_MANAGER_H
#define IMAGE_HISTORY_MANAGER_H

#include <deque>
#include <QDebug>
#include <opencv2/opencv.hpp>

class ImageHistoryManager {
public:
    static void push(cv::Mat &image);
    static cv::Mat undo(cv::Mat &currentImage);
    static cv::Mat redo(cv::Mat &currentImage);
    static bool canUndo();
    static bool canRedo();
    static void clearRedo();

private:
    static const size_t MAX_HISTORY = 5;
    static std::deque<cv::Mat> undoStack;
    static std::deque<cv::Mat> redoStack;
};

#endif // IMAGE_HISTORY_MANAGER_H
