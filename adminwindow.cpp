#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <QTableWidgetItem>
#include <QDebug>
using namespace std;

adminwindow::adminwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminwindow)
{
    ui->setupUi(this);
    loadPackages();
}

adminwindow::~adminwindow()
{
    delete ui;
}

void adminwindow::loadPackages() {
    ifstream file ("data/paquetes.txt");

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir paquetes.txt carga paquetes";
        return;
    }

    ui->packagesTable->setRowCount(0);

    string line;
    while(getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if(fields.size() != 5) {
            continue;
        }

        int row = ui->packagesTable->rowCount();
        ui->packagesTable->insertRow(row);

        for (int col = 0; col < 5; col++) {
            ui->packagesTable->setItem(
                row,
                col,
                new QTableWidgetItem(QString::fromStdString(fields[col]))
                );
        }
    }

    file.close();
}

void adminwindow::on_searchButton_clicked() {
    QString cedulaBuscada = ui->packageEdit->text().trimmed();

    if(cedulaBuscada.isEmpty()) {
        ui->resultLabel->setText("Ingrese un ID");
        return;
    }

    ifstream file ("data/paquetes.txt");

    if(!file.is_open()) {
        qDebug() << "No se pudo abrir paquetes.txt busqueda paquetes";
        return;
    }

    string line;
    bool encontrado = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() != 5) {
            continue;
        }

        if (QString::fromStdString(fields[2]) == cedulaBuscada) {
            QString resultado =
                "Cédula: " + QString::fromStdString(fields[0]) + "\n" +
                "Cliente: " + QString::fromStdString(fields[1]) + "\n" +
                "ID: " + QString::fromStdString(fields[2]) + "\n" +
                "Estado: " + QString::fromStdString(fields[3]) + "\n" +
                "Fecha: " + QString::fromStdString(fields[4]);
            ui->resultLabel->setText(resultado);
            encontrado = true;
            break;
        }
    }
    file.close();

    if(!encontrado) {
        ui->resultLabel->setText("Paquete no encontrado");
    }
}
