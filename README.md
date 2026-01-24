void AdminWindow::loadPackages()
{
    // Abrimos el archivo que contiene los paquetes
    std::ifstream file("data/paquetes.txt");

    // Verificamos que el archivo se haya abierto correctamente
    if (!file.is_open()) {
        qDebug() << "No se pudo abrir data/paquetes.txt";
        return;
    }

    // Limpiamos la tabla antes de volver a cargar datos
    ui->packagesTable->setRowCount(0);

    std::string line;

    // Leemos el archivo línea por línea
    while (std::getline(file, line)) {

        // Usamos stringstream para separar los campos por ';'
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        // Extraemos cada campo de la línea
        while (std::getline(ss, field, ';')) {
            fields.push_back(field);
        }

        // Si no tiene exactamente 5 campos, se ignora la línea
        if (fields.size() != 5) {
            continue;
        }

        // Insertamos una nueva fila en la tabla
        int row = ui->packagesTable->rowCount();
        ui->packagesTable->insertRow(row);

        // Llenamos cada columna de la fila
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

    // Cerramos el archivo
    file.close();
}
