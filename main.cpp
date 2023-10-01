#include "saveeditorwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SaveEditorWindow w;
    w.show();
    return a.exec();
}
