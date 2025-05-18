#include "ImageHistoryManager.h"

std::deque<cv::Mat> ImageHistoryManager::undoStack;
std::deque<cv::Mat> ImageHistoryManager::redoStack;

bool ImageHistoryManager::canUndo() {
    return !undoStack.empty();
}

bool ImageHistoryManager::canRedo() {
    return !redoStack.empty();
}

void ImageHistoryManager::clearRedo() {
    redoStack.clear();
}

void ImageHistoryManager::push(cv::Mat &image) {
    if (undoStack.size() == MAX_HISTORY) {
        undoStack.pop_front();
    }
    undoStack.push_back(image.clone());
    clearRedo();
}

cv::Mat ImageHistoryManager::undo(cv::Mat &currentImage) {
    if (canUndo()) {
        if (redoStack.size() == MAX_HISTORY) {
            redoStack.pop_front();
        }
        redoStack.push_back(currentImage.clone());

        cv::Mat top = undoStack.back();
        undoStack.pop_back();
        return top;
    }
    return currentImage;
}

cv::Mat ImageHistoryManager::redo(cv::Mat &currentImage) {
    if (canRedo()) {
        if (undoStack.size() == MAX_HISTORY) {
            undoStack.pop_front();
        }
        undoStack.push_back(currentImage.clone());

        cv::Mat top = redoStack.back();
        redoStack.pop_back();
        return top;
    }
    return currentImage;
}
