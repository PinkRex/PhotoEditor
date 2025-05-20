#ifndef PLUGINUNLOADDIALOG_H
#define PLUGINUNLOADDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <mainwindow.h>

class PluginUnloadDialog : public QDialog {
    Q_OBJECT
public:
    PluginUnloadDialog(const QList<MainWindow::PluginInfo>& plugins, QWidget* parent = nullptr);
    QList<int> selectedIndexes() const;

private:
    QList<QCheckBox*> checkBoxes;
};

#endif // PLUGINUNLOADDIALOG_H
