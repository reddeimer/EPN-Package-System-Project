#include <QApplication>
#include "rolewindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    RoleWindow ventanaInicial;
    ventanaInicial.show();

    return application.exec();
}
