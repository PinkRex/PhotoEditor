#include "PluginUnloadDialog.h"

PluginUnloadDialog::PluginUnloadDialog(const QList<MainWindow::PluginInfo>& plugins, QWidget* parent) : QDialog(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    for (const MainWindow::PluginInfo& plugin : plugins) {
        QCheckBox* cb = new QCheckBox(plugin.name, this);
        checkBoxes.append(cb);
        layout->addWidget(cb);
    }
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &PluginUnloadDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &PluginUnloadDialog::reject);
    layout->addWidget(buttons);
}

QList<int> PluginUnloadDialog::selectedIndexes() const {
    QList<int> indexes;
    for (int i = 0; i < checkBoxes.size(); ++i) {
        if (checkBoxes[i]->isChecked()) {
            indexes.append(i);
        }
    }
    return indexes;
}
