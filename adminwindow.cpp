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

void adminwindow::loadPackages()
{
    ifstream file("data/paquetes.txt");

    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    ui->packagesTable->setRowCount(0);

    string line;
    while (getline(file, line)) {

        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        // Ahora se esperan al menos 7 campos
        if (fields.size() < 7) {
            continue;
        }

        int row = ui->packagesTable->rowCount();
        ui->packagesTable->insertRow(row);

        // Mostrar los 7 campos en la tabla
        for (int col = 0; col < 7; ++col) {
            ui->packagesTable->setItem(
                row,
                col,
                new QTableWidgetItem(QString::fromStdString(fields[col]))
            );
        }
    }

    file.close();
}


void adminwindow::on_filterButton_clicked()
{
    QString filtro = ui->filterEdit->text().trimmed();
    int filas = ui->packagesTable->rowCount();

    for (int row = 0; row < filas; ++row) {

        QTableWidgetItem *itemCliente =
            ui->packagesTable->item(row, 1);

        if (!itemCliente) {
            continue;
        }

        bool visible =
            itemCliente->text().contains(filtro, Qt::CaseInsensitive) ||
            filtro.isEmpty();

        ui->packagesTable->setRowHidden(row, !visible);
    }
}
