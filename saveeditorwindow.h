#ifndef SAVEEDITORWINDOW_H
#define SAVEEDITORWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <iostream>
#include "savefile.h"
#include "nodes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SaveEditorWindow; }
QT_END_NAMESPACE

class SaveEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    SaveEditorWindow(QWidget *parent = nullptr);
    ~SaveEditorWindow();

public slots:
    void networkTreeItemSelected(QTreeWidgetItem *item, int);

private:
    Ui::SaveEditorWindow *ui;
    SaveFile saveFile;
    int selectedComputerIndex = -1;

    bool updateComputerPanel(int index);

    // events
    void on_actionAbout_triggered();
    void on_actionOpenSave_triggered();
    void on_actionWriteSave_triggered();
};
#endif // SAVEEDITORWINDOW_H
