#include "loginwindow.h"
#include "ui_loginwindow.h"
// #include <QDebug>

loginwindow::loginwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
{
    ui->setupUi(this);
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_loginButton_clicked() {
    // qDebug() << "LOGIN PRESIONADO";
    ui ->messageLabel->setText("Probando...");
    QString cedula = ui ->cedulaEdit ->text();
    QString password = ui ->passwordEdit->text();

    if (cedula.isEmpty() || password.isEmpty()) {
        ui ->messageLabel->setText("Ingrese cédula y contraseña: ");
        return;
    }

    // Prueba
    if (cedula == "123" && password == "123") {
        ui ->messageLabel->setText("Login correcto");
    } else {
        ui->messageLabel->setText("Usuario incorrecto");
    }
}

void loginwindow::on_registerButton_clicked() {
    ui->messageLabel->setText("Registro aún no implementado");
}
