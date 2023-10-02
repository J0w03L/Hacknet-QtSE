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

    for (Node::Computer computer : this->saveFile.computers)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(networkTree);

        item->setText(0, computer.ip);   // IP
        item->setText(1, computer.name);  // Name

        itemList.append(item);
    }

    networkTree->addTopLevelItems(itemList);
    return;
}

void SaveEditorWindow::networkTreeItemSelected(QTreeWidgetItem *item, int)
{
    int nodeIndex = this->ui->networkTree->currentIndex().row();

    if (updateComputerPanel(nodeIndex))
    {
        qDebug() << "Selected computer node:" << nodeIndex;
        selectedComputerIndex = nodeIndex;
    }

    return;
}

bool SaveEditorWindow::updateComputerPanel(int index)
{
    if (index >= this->saveFile.computers.length())
    {
        qDebug() << "Tried to updateComputerPanel with an index of" << index << "(len of saveFile.computers is" << saveFile.computers.length() << "); skipping!";
        return false;
    }

    Node::Computer node = this->saveFile.computers.at(index);

    ui->computerNameEdit->setText(node.name);
    ui->computerIPEdit->setText(node.ip);
    ui->computerIDEdit->setText(node.id);
    ui->computerTypeEdit->setCurrentIndex(node.type - 1);
    ui->computerSpecEdit->setCurrentIndex(node.spec.compare("mail") == 0 ? 1 : node.spec.compare("player") == 0 ? 2 : 0);

    ui->netmapXEdit->setValue(node.location.x);
    ui->netmapYEdit->setValue(node.location.y);
    ui->netmapVisibleEdit->setCheckState(node.known ? Qt::Checked : Qt::Unchecked);

    ui->securityLevelEdit->setValue(node.security.level);
    ui->securityTraceTimeEdit->setValue(node.security.traceTime);
    ui->securityProxyTimeEdit->setValue(node.security.proxyTime);
    ui->securityPortsToCrackEdit->setValue(node.security.portsToCrack);
    ui->securityAdminIPEdit->setText(node.security.adminIP);

    ui->adminTypeEdit->setCurrentIndex(node.admin.type.compare("basic") == 0 ? 1 : node.admin.type.compare("fast") ? 2 : 0);
    ui->adminResetPassEdit->setCheckState(node.admin.resetPass ? Qt::Checked : Qt::Unchecked);
    ui->adminSuperEdit->setCheckState(node.admin.isSuper ? Qt::Checked : Qt::Unchecked);

    ui->firewallComplexityEdit->setValue(node.firewall.complexity);
    ui->firewallSolutionEdit->setText(node.firewall.solution);
    ui->firewallDelayEdit->setValue(node.firewall.additionalDelay);

    return true;
}
