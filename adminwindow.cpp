#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <QTableWidgetItem>
#include <QDebug>
#include<QFile>
#include <QMessageBox>
using namespace std;

adminwindow::adminwindow(const QString &dataPath, QWidget *parent)
    : QWidget(parent)
    , dataPath(dataPath)
    , ui(new Ui::adminwindow)
{
    ui->setupUi(this);
    ui->packagesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    cargarMisPaquetes();
    cargarSolicitudes();
}

adminwindow::~adminwindow()
{
    delete ui;
}

void adminwindow::cargarMisPaquetes() {
    ui->packagesTable->setRowCount(0);

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

        ui->packagesTable->insertRow(row);
        for (int col = 0; col < 7; col++) {
            ui->packagesTable->setItem(row, col, new QTableWidgetItem(f[col]));
        };
        row++;
    }
    file.close();
}

void adminwindow::cargarSolicitudes() {

    QFile file(dataPath + "/solicitudes.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir solicitudes.txt";
        return;
    }

    ui->solicitudesTable->clearContents();
    ui->solicitudesTable->setRowCount(0);
    ui->solicitudesTable->setColumnCount(4);
    ui->solicitudesTable->setHorizontalHeaderLabels({
        "Cédula", "ID Paquete", "Fecha", "Estado"
    });

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");

        if (parts.size() < 4)
            continue;

        int row = ui->solicitudesTable->rowCount();
        ui->solicitudesTable->insertRow(row);

        for (int col = 0; col < 4; ++col) {
            ui->solicitudesTable->setItem(
                row, col,
                new QTableWidgetItem(parts[col].trimmed())
                );
        }
    }

    file.close();
}

bool adminwindow::eliminarDeArchivo(const QString &ruta, int columnaId, const QString &idBuscado)
{
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QStringList nuevasLineas;
    QTextStream in(&file);
    bool eliminado = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");

        if (parts.size() > columnaId &&
            parts[columnaId].trimmed() == idBuscado) {
            eliminado = true;
            continue; // saltar línea (eliminar)
        }

        nuevasLineas << line;
    }

    file.close();

    if (!eliminado)
        return false;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;

    QTextStream out(&file);
    for (const QString &l : nuevasLineas)
        out << l << "\n";

    file.close();
    return true;
}


void adminwindow::on_loadButton_clicked()
{
    ui->packagesTable->setRowCount(0);

    QFile file(dataPath + "/paquetes.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir parquetes.txt para lectura";
        return;
    }

    if (ui->cedulaEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Llene el campo primero");
        return;
    }

    QTextStream in(&file);
    int row = 0;

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");

        if (fields.size() < 7) {
            continue;
        }
        if (fields[0] == ui->cedulaEdit->text()) {
            ui->packagesTable->insertRow(row);

            for (int col = 0; col < 7; col++) {
                ui->packagesTable->setItem(row, col, new QTableWidgetItem(fields[col].trimmed()));
            }
        }
    }
    file.close();
}

void adminwindow::on_searchButton_clicked(){
    QString idBuscado = ui->packageEdit->text().trimmed();

    if (idBuscado.isEmpty()) {
        ui->resultLabel->setText("Ingrese un ID");
        return;
    }
    QString dataFile = (dataPath + "/paquetes.txt");

    ifstream file(dataFile.toStdString());

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    string line;
    bool encontrado = false;

    while (getline(file, line)) {

        stringstream ss(line);
        vector<string> fields;
        string field;

        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() < 7) {
            continue;
        }

        if (QString::fromStdString(fields[2]) == idBuscado) {

            ui->resultLabel->setText(
                "Cédula: " + QString::fromStdString(fields[0]) + "\n" +
                "Cliente: " + QString::fromStdString(fields[1]) + "\n" +
                "ID: " + QString::fromStdString(fields[2]) + "\n" +
                "Estado: " + QString::fromStdString(fields[3]) + "\n" +
                "Fecha: " + QString::fromStdString(fields[4]) + "\n" +
                "Paquete: " + QString::fromStdString(fields[5]) + "\n" +
                "Cantidad: " + QString::fromStdString(fields[6])
            );

            encontrado = true;
            break;
        }
    }

    file.close();

    if (!encontrado) {
        ui->resultLabel->setText("Paquete no encontrado");
    }
}

void adminwindow::on_updateStatusButton_clicked()
{
    QString idBuscado = ui->manageEdit->text().trimmed();
    QString nuevoEstado = ui->statusComboBox->currentText();

    if (idBuscado.isEmpty()) {
        ui->manageMessageLabel->setText("Ingrese un ID");
        return;
    }

    QString dataFile = dataPath + "/paquetes.txt";
    ifstream file(dataFile.toStdString());

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir paquetes.txt gestión de paquetes";
        return;
    }

    vector<vector<string>> paquetes;
    string line;
    bool encontrado = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() < 7) {
            paquetes.push_back(fields);
            continue;
        }

        if (QString::fromStdString(fields[2]) == idBuscado) {
            fields[3] = nuevoEstado.toStdString();
            encontrado = true;
        }

        paquetes.push_back(fields);
    }

    file.close();

    if (!encontrado) {
        ui->manageMessageLabel->setText("Paquete no encontrado");
        return;
    }

    ofstream outFile(dataFile.toStdString(), ios::trunc);

    for (const auto &p : paquetes) {
        if (p.size() < 7) continue;

        outFile << p[0] << ";" << p[1] << ";" << p[2] << ";"
                << p[3] << ";" << p[4] << ";" << p[5] << ";" << p[6] << "\n";
    }

    outFile.close();

    ui->manageMessageLabel->setText("Estado actualizado correctamente");
    cargarMisPaquetes();
}

void adminwindow::on_filterButton_clicked()
{
    QString filtro = ui->filterEdit->text().trimmed();

    if (filtro.isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingrese datos en el campo.");
        return;
    }

    QFile file (dataPath + "/clientes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir clientes.txt, boton filtro";
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString lineas = in.readLine();
        QStringList partes = lineas.split(";");

        if(partes.size() < 5) {
            continue;
        }

        if (partes[0] == filtro) {
            ui->filterLabel->setText("Cedula: " + partes[0] + "\n" +
                                     "Apellido: " +partes[2] + "\n" +
                                     "Nombre: " + partes[3] + "\n" +
                                     "Correo: " + partes[4] + "\n");
        }
    }
}



void adminwindow::on_eliminarButton_clicked()
{
    QString idBuscado = ui->eliminarEdit->text().trimmed();

    if (idBuscado.isEmpty()) {
        ui->eliminarLabel->setText("Ingrese un ID");
        return;
    }

    bool eliminadoPaquete = eliminarDeArchivo(dataPath + "/paquetes.txt", 2, idBuscado);

    bool eliminadaSolicitud = eliminarDeArchivo(dataPath + "/solicitudes.txt", 1, idBuscado);

    if (eliminadoPaquete && eliminadaSolicitud) {
        ui->eliminarLabel->setText("Paquete eliminado correctamente");
        cargarMisPaquetes();
        cargarSolicitudes();
    } else {
        ui->eliminarLabel->setText("ID no encontrado");
    }
}

