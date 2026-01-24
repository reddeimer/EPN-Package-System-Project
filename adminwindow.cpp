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
void AdminWindow::loadPackages()
{
    std::ifstream file("data/paquetes.txt");

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    ui->packagesTable->setRowCount(0);

    std::string line;
    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() != 5) {
            continue;
        }

        int row = ui->packagesTable->rowCount();
        ui->packagesTable->insertRow(row);

        for (int col = 0; col < 5; ++col) {
            ui->packagesTable->setItem(
                row,
                col,
                new QTableWidgetItem(
                    QString::fromStdString(fields[col])
                )
            );
        }
    }

    file.close();
}

void AdminWindow::onSearchButtonClicked()
{
    const QString idBuscado = ui->packageEdit->text().trimmed();

    if (idBuscado.isEmpty()) {
        ui->resultLabel->setText("Ingrese un ID");
        return;
    }

    std::ifstream file("data/paquetes.txt");

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    std::string line;
    bool encontrado = false;

    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() != 5) {
            continue;
        }

        if (QString::fromStdString(fields[2]) == idBuscado) {
            ui->resultLabel->setText(
                "Cédula: " + QString::fromStdString(fields[0]) + "\n" +
                "Cliente: " + QString::fromStdString(fields[1]) + "\n" +
                "ID: " + QString::fromStdString(fields[2]) + "\n" +
                "Estado: " + QString::fromStdString(fields[3]) + "\n" +
                "Fecha: " + QString::fromStdString(fields[4])
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

void AdminWindow::onUpdateStatusButtonClicked()
{
    const QString idBuscado = ui->packageEdit->text().trimmed();
    const QString nuevoEstado = ui->statusComboBox->currentText();

    if (idBuscado.isEmpty()) {
        ui->manageMessageLabel->setText("Ingrese un ID");
        return;
    }

    std::ifstream file("data/paquetes.txt");

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    std::vector<std::vector<std::string>> paquetes;
    std::string line;
    bool encontrado = false;

    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() != 5) {
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

    std::ofstream outFile("data/paquetes.txt", std::ios::trunc);

    for (const auto &p : paquetes) {
        outFile << p[0] << ";"
                << p[1] << ";"
                << p[2] << ";"
                << p[3] << ";"
                << p[4] << "\n";
    }

    outFile.close();

    ui->manageMessageLabel->setText("Estado actualizado correctamente");
    loadPackages();
}

void AdminWindow::onFilterButtonClicked()
{
    const QString filtro = ui->filterEdit->text().trimmed();
    const int totalRows = ui->packagesTable->rowCount();

    for (int row = 0; row < totalRows; ++row) {

        QTableWidgetItem *itemCliente =
            ui->packagesTable->item(row, 1);

        if (!itemCliente) {
            continue;
        }

        const bool visible =
            itemCliente->text().contains(filtro, Qt::CaseInsensitive) ||
            filtro.isEmpty();

        ui->packagesTable->setRowHidden(row, !visible);
    }
}

