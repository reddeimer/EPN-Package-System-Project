#include "rolewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rolewindow w;
    w.show();
    return a.exec();
}
