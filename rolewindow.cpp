#include "rolewindow.h"
#include "ui_rolewindow.h"
#include "loginwindow.h"

RoleWindow::RoleWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RoleWindow)
{
    ui->setupUi(this);
}

RoleWindow::~RoleWindow()
{
    delete ui;
}

void RoleWindow::onClientButtonClicked()
{
    LoginWindow *login = new LoginWindow(UserType::Cliente);
    login->show();
    close();
}

void RoleWindow::onAdminButtonClicked()
{
    LoginWindow *login = new LoginWindow(UserType::Administrador);
    login->show();
    close();
}

void RoleWindow::onExitButtonClicked()
{
    close();
}
