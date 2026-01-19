#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <fstream>
#include <string>
// #include <QDebug>
using namespace std;

loginwindow::loginwindow(UserType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
    , userType(type)
{
    ui->setupUi(this);

    if (userType == Cliente) {
        dataFile = "clientes.txt";
    } else {
        dataFile = "admins.txt";
    }
}

loginwindow::~loginwindow()
{
    delete ui;
}

bool loginwindow::validateLogin(const QString &cedula, const QString &password) {
    fstream file(dataFile.toStdString(), ios::in);

    if (!file.is_open()) {
        ui->messageLabel->setText("No se pudo abrir el archivo");
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(';');
        if (pos == string::npos) {
            continue;
        }
        string fileCedula = line.substr(0, pos);
        string filePassword = line.substr(pos+1);

        if (fileCedula == cedula.toStdString() && filePassword == password.toStdString()) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
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
    // if (cedula == "123" && password == "123") {
    //     ui ->messageLabel->setText("Login correcto");
    // } else {
    //     ui->messageLabel->setText("Usuario incorrecto");
    // }
    if (validateLogin(cedula, password)) {
        ui->messageLabel->setText("Login correcto");
    } else {
        ui->messageLabel->setText("Usuario incorrecto");
    }

}

void loginwindow::on_registerButton_clicked() {
    ui->messageLabel->setText("Registro aún no implementado");
}
