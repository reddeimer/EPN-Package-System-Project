#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "adminwindow.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

LoginWindow::LoginWindow(UserType type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , userType(type)
{
    ui->setupUi(this);

    // Crear carpeta data si no existe
    const QString basePath = QCoreApplication::applicationDirPath();
    QDir dataDir(basePath + "/data");

    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    // Configuración según tipo de usuario
    if (userType == UserType::Administrador) {
        ui->registerButton->hide();
        dataFilePath = basePath + "/data/admins.txt";
    } else {
        dataFilePath = basePath + "/data/clientes.txt";
    }

    resetToLoginView();

    // Conexiones explícitas (más legibles en C++)
    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginButtonClicked);

    connect(ui->registerButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterButtonClicked);

    connect(ui->registerConfirmButton, &QPushButton::clicked,
            this, &LoginWindow::onRegisterConfirmButtonClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::validateLogin(const QString &cedula, const QString &password)
{
    QFile file(dataFilePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine();
        const QStringList fields = line.split(";");

        if (fields.size() < 2) {
            continue;
        }

        if (fields[0].trimmed() == cedula &&
            fields[1].trimmed() == password) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void LoginWindow::onLoginButtonClicked()
{
    const QString cedula = ui->cedulaEdit->text().trimmed();
    const QString password = ui->passwordEdit->text().trimmed();

    if (cedula.isEmpty() || password.isEmpty()) {
        ui->messageLabel->setText("Ingrese cédula y contraseña");
        return;
    }

    if (!validateLogin(cedula, password)) {
        ui->messageLabel->setText("Credenciales incorrectas");
        return;
    }

    openNextWindow();
}

void LoginWindow::openNextWindow()
{
    if (userType == UserType::Administrador) {
        auto *adminWindow = new AdminWindow();
        adminWindow->show();
    } else {
        auto *mainWindow = new MainWindow();
        mainWindow->show();
    }

    close();
}

void LoginWindow::onRegisterButtonClicked()
{
    ui->cedulaEdit->hide();
    ui->passwordEdit->hide();
    ui->loginButton->hide();
    ui->registerButton->hide();

    ui->regCedulaEdit->show();
    ui->regNombreEdit->show();
    ui->regApellidoEdit->show();
    ui->regMailEdit->show();
    ui->regPasswordEdit->show();
    ui->registerConfirmButton->show();

    ui->messageLabel->setText("Complete el registro");
}

void LoginWindow::onRegisterConfirmButtonClicked()
{
    const QString cedula   = ui->regCedulaEdit->text().trimmed();
    const QString nombre   = ui->regNombreEdit->text().trimmed();
    const QString apellido = ui->regApellidoEdit->text().trimmed();
    const QString correo   = ui->regMailEdit->text().trimmed();
    const QString password = ui->regPasswordEdit->text().trimmed();

    if (cedula.isEmpty() || nombre.isEmpty() ||
        apellido.isEmpty() || correo.isEmpty() ||
        password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Todos los campos son obligatorios");
        return;
    }

    QFile file(dataFilePath);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "No se pudo guardar el registro");
        return;
    }

    QTextStream out(&file);
    out << cedula << ";"
        << password << ";"
        << apellido << ";"
        << nombre << ";"
        << correo << "\n";

    file.close();

    ui->messageLabel->setText("Registro exitoso");
    resetToLoginView();
}

void LoginWindow::resetToLoginView()
{
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
