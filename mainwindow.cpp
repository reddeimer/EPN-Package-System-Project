#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QDate>
#include<QMessageBox>
#include<QHeaderView>

MainWindow::MainWindow(const QString &cedula, const QString &nombre, const QString &dataPath, QWidget *parent)
    : QMainWindow(parent)
    , cedulaCliente(cedula)
    , nombreCliente(nombre)
    , dataPath(dataPath)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->paquetesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->welcomeLabel->setText("Bienvenido, " + nombreCliente);
    cargarMisPaquetes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cargarMisPaquetes() {
    ui->paquetesTable->setRowCount(0);

    QFile file(dataPath + "/paquetes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in (&file);
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList f = line.split(";");

        if (f.size() < 7) {
            continue;
        }

        if (f[0].trimmed() == cedulaCliente) {
            ui->paquetesTable->insertRow(row);
            for (int col = 0; col < 7; col++) {
                ui->paquetesTable->setItem(row, col, new QTableWidgetItem(f[col]));
            };
            row++;
        }
    }
    file.close();
}

bool MainWindow::solicitudYaExiste(const QString &idPaquete) {
    QFile file(dataPath + "/solicitudes.txt");

    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir solicitudes.txt para validar";
        return false;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");

        if(fields.size() < 4) {
            continue;
        }

        QString fileID = fields[1].trimmed();
        QString estado = fields[3].trimmed();

        if(fileID == idPaquete && estado == "Pendiente") {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void MainWindow::on_buscarButton_clicked()
{
    QString id = ui->idBuscarEdit->text().trimmed();

    if(id.isEmpty()) {
        ui->resultadoLabel->setText("Ingrese un ID");
        return;
    }

    QFile file (dataPath + "/paquetes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList f = in.readLine().split(";");

        if (f.size() < 7) {
            continue;
        }
        if (f[2] == id && f[0] == cedulaCliente) {
            ui->resultadoLabel->setText("Nombre: " + f[5] + "\nEstado: " + f[3] + "\nFecha: " + f[4]);
            file.close();
            return;
        }
    }

    file.close();
    ui->resultadoLabel->setText("Paquete no encontrado");
}

void MainWindow::on_requestButton_clicked()
{
    QString id = ui->idEliminarEdit->text().trimmed();

    if(id.isEmpty()) {
        ui->messageLabel->setText("Ingrese un ID de paquete");
        return;
    }

    if(solicitudYaExiste(id)) {
        QMessageBox::warning(this, "Error", "Este paquete ya tiene una solicitud pendiente");
        return;
    }

    QFile file(dataPath + "/solicitudes.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        ui->messageLabel->setText("Error al enviar solicitud");
        return;
    }
    QTextStream out(&file);
    out << cedulaCliente << ";"
        << id << ";"
        << QDate::currentDate().toString("yyyy-MM-dd")
        << ";Pendiente\n";

    file.close();
    ui->messageLabel->setText("Solicitud enviada correctamente");
}

void MainWindow::on_registerButton_clicked()
{
    QString nombrePaquete = ui->nameEdit->text().trimmed().toLower();
    QString cantidad = ui->cuantitySpinBox->text().trimmed();

    if (nombrePaquete.isEmpty() || (cantidad.toInt() == 0)) {
        QMessageBox::warning(this, "Error", "Campos vacíos");
        return;
    }

    QFile file(dataPath + "/paquetes.txt");

    int ultimoId = 0;

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "No se pudo arir paquetes.txt";
            return;
        }

        QTextStream in (&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(";");

            if (parts.size() >= 3) {
                int id = parts[2].toInt();
                if (id > ultimoId) {
                    ultimoId = id;
                }
            }
        }
        file.close();
    }

    int nuevoId = ultimoId + 1;

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "No se pudo abrir parquetes.txt para lectura";
        return;
    }

    QTextStream out (&file);
    QString fecha = QDate::currentDate().toString("yyyy-MM-dd");
    QString estado = "REGISTRADO";

    out << cedulaCliente << ";"
        << nombreCliente << ";"
        << nuevoId << ";"
        << estado << ";"
        << fecha << ";"
        << nombrePaquete << ";"
        << cantidad << "\n";

    file.close();

    qDebug() << "Paquete registrado con ID: " << nuevoId;
    ui->nameEdit->setText("");
    ui->cuantitySpinBox->setValue(-1);
}


void MainWindow::on_refreshButton_clicked()
{
    cargarMisPaquetes();
}

