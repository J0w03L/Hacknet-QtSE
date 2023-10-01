#include "saveeditorwindow.h"
#include "./ui_saveeditorwindow.h"
#include "aboutwindow.h"

SaveEditorWindow::SaveEditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SaveEditorWindow)
{
    ui->setupUi(this);

    // Find networkTree.
    QTreeWidget *networkTree = ui->networkTree;

    // Link up actions and stuff.
    connect(ui->actionAbout, &QAction::triggered, this, &SaveEditorWindow::on_actionAbout_triggered);
    connect(ui->actionOpenSave, &QAction::triggered, this, &SaveEditorWindow::on_actionOpenSave_triggered);
    connect(networkTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(networkTreeItemSelected(QTreeWidgetItem*, int)));
}

SaveEditorWindow::~SaveEditorWindow()
{
    delete ui;
}

void SaveEditorWindow::on_actionAbout_triggered()
{
    qDebug() << "Showing about window.";
    AboutWindow *about = new AboutWindow(this);
    about->show();
    return;
}

void SaveEditorWindow::on_actionOpenSave_triggered()
{
    qDebug() << "Showing open save file dialog.";
    QFileDialog openDialog(this);

    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setNameFilter(tr("Hacknet Savefiles (*.xml)"));
    openDialog.setViewMode(QFileDialog::Detail);

    if (!openDialog.exec())
    {
        qDebug() << "openDialog cancelled.";
        return;
    }

    QStringList fileNames = openDialog.selectedFiles();
    QString fileName = fileNames.first();
    qDebug() << "Got path:" << fileName;

    int result = this->saveFile.open(fileName);
    if (!result)
    {
        qDebug() << "Failed to open savefile! (code:" << result << ").";
        return;
    }

    // Update the network tree.
    QTreeWidget *networkTree = ui->networkTree;
    networkTree->clear();

    QList<QTreeWidgetItem*> itemList;
    size_t computerIndex = 0;

    for (QPair<QString, QString> computer : this->saveFile.computers)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(networkTree);

        item->setText(0, computer.first);   // IP
        item->setText(1, computer.second);  // Name

        itemList.append(item);
    }

    networkTree->addTopLevelItems(itemList);
    return;
}

void SaveEditorWindow::networkTreeItemSelected(QTreeWidgetItem *item, int)
{
    int node = this->ui->networkTree->currentIndex().row();
    qDebug() << "Selected computer node:" << node;
    return;
}
