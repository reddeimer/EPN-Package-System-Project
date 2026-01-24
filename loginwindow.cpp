#include "adminwindow.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QFile>
using namespace std;

loginwindow::loginwindow(UserType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginwindow)
    , userType(type)
{
    ui->setupUi(this);

    QString basePath = QCoreApplication::applicationDirPath();
    QDir dir(basePath + "/data");
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    dataPath = basePath + "/data";

    if (userType == Administrador) {
        ui->registerButton->hide();
        ui->regCedulaEdit->hide();
        ui->regNombreEdit->hide();
        ui->regApellidoEdit->hide();
        ui->regMailEdit->hide();
        ui->regPasswordEdit->hide();
        ui->registerConfirmButton->hide();

        dataFile = basePath + "/data/admins.txt";
    } else {
        ui->registerButton->show();
        ui->regCedulaEdit->hide();
        ui->regNombreEdit->hide();
        ui->regApellidoEdit->hide();
        ui->regMailEdit->hide();
        ui->regPasswordEdit->hide();
        ui->registerConfirmButton->hide();
        dataFile = basePath + "/data/clientes.txt";
    }
}

loginwindow::~loginwindow()
{
    delete ui;
}

bool loginwindow::validateLogin(const QString &cedula, const QString &password) {
    qDebug() << "Intentando abrir:" << dataFile;
    QFile file(dataFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "NO SE PUDO ABRIR EL ARCHIVO";
        qDebug() << "No se pudo abrir el archivo, validar login";
        return false;
    }

    qDebug() << "Archivo abierto correctamente";

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList parte = line.split(";");

        if (parte.size() < 2) {
            continue;
        }
        QString fileCedula = parte[0].trimmed();
        QString filePassword = parte[1].trimmed();

        if (fileCedula == cedula && filePassword == password) {
            nombre = parte[1];
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
    } else {
        MainWindow *main = new MainWindow(cedula, nombre, dataPath);
        main->show();
        this->close();
    }
}

void loginwindow::volverLogin() {
    ui->cedulaEdit->show();
    ui->passwordEdit->show();
    ui->loginButton->show();
    ui->registerButton->show();
    ui->regCedulaEdit->hide();
    ui->regNombreEdit->hide();
    ui->regApellidoEdit->hide();
    ui->regMailEdit->hide();
    ui->regPasswordEdit->hide();
    ui->registerConfirmButton->hide();

    ui->regCedulaEdit->clear();
    ui->regNombreEdit->clear();
    ui->regApellidoEdit->clear();
    ui->regMailEdit->clear();
    ui->regPasswordEdit->clear();

}

void loginwindow::on_registerConfirmButton_clicked()
{
    QString cedula = ui->regCedulaEdit->text().trimmed();
    QString nombre = ui->regNombreEdit->text().trimmed();
    QString apellido = ui->regApellidoEdit->text().trimmed();
    QString correo = ui->regMailEdit->text().trimmed();
    QString password = ui->regPasswordEdit->text().trimmed();

    if (cedula.isEmpty() || nombre.isEmpty() || apellido.isEmpty() || correo.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Faltan llegar campos");
        return;
    }
    QFile file(dataFile);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo, registro";
        return;
    }

    QTextStream out(&file);
    out << cedula << ";"
        << password << ";"
        << apellido << ";"
        << nombre << ";"
        << correo << "\n";

    file.close();

    ui->messageLabel->setText("Registro exitoso. Ya puede iniciar sesión");

    volverLogin();
}

void loginwindow::on_registerButton_clicked() {
    ui->cedulaEdit->hide();
    ui->passwordEdit->hide();
    ui->loginButton->hide();
    ui->registerButton->hide();

    ui->messageLabel->setText("Complete el registro");
    ui->regCedulaEdit->show();
    ui->regNombreEdit->show();
    ui->regApellidoEdit->show();
    ui->regMailEdit->show();
    ui->regPasswordEdit->show();
    ui->registerConfirmButton->show();

}


