#include "adminwindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <fstream>
// #include <string>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
using namespace std;

loginwindow::loginwindow(UserType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
    , userType(type)
{
    ui->setupUi(this);

    QString basePath = QCoreApplication::applicationDirPath();

    if (userType == Administrador) {
        ui->registerButton->hide();
        dataFile = basePath + "/data/admins.txt";
    } else {
        ui->registerButton->show();
        dataFile = basePath + "/data/clientes.txt";
    }
    qDebug() << "Ruta final del archivo: " << dataFile;
}

loginwindow::~loginwindow()
{
    delete ui;
}

bool loginwindow::validateLogin(const QString &cedula, const QString &password) {
    qDebug() << "Intentando abrir:" << dataFile;
    fstream file(dataFile.toStdString(), ios::in);

    if (!file.is_open()) {
        qDebug() << "NO SE PUDO ABRIR EL ARCHIVO";
        ui->messageLabel->setText("No se pudo abrir el archivo");
        return false;
    }

    qDebug() << "Archivo abierto correctamente";

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
    // ui ->messageLabel->setText("Probando...");
    QString cedula = ui ->cedulaEdit ->text();
    QString password = ui ->passwordEdit->text();

    if (cedula.isEmpty() || password.isEmpty()) {
        ui ->messageLabel->setText("Ingrese cédula y contraseña: ");
        return;
    }

    if (!validateLogin(cedula, password)) {
        ui->messageLabel->setText("Usuario incorrecto");
        return;
    }

    if(userType == Administrador) {
        adminwindow *admin = new adminwindow();
        admin->show();
        this->close();
    }

}

void loginwindow::on_registerButton_clicked() {
    ui->messageLabel->setText("Registro aún no implementado");
}
