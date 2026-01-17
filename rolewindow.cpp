#include "rolewindow.h"
#include "ui_rolewindow.h"
#include "loginwindow.h"

rolewindow::rolewindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rolewindow)
{
    ui->setupUi(this);
}

rolewindow::~rolewindow()
{
    delete ui;
}

void rolewindow::on_clientButton_clicked() {
    loginwindow *login = new loginwindow(Cliente);
    login ->show();
    this->close();
}

void rolewindow::on_adminButton_clicked() {
    loginwindow *login = new loginwindow(Administrador);
    login->show();
    this->close();
}

void rolewindow::on_exitButton_clicked() {
    close();
}
