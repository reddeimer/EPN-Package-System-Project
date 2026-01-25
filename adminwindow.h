#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

namespace Ui {
class adminwindow;
}

class adminwindow : public QWidget
{
    Q_OBJECT

public:
    explicit adminwindow(const QString &dataPath, QWidget *parent = nullptr);
    ~adminwindow();

private:
    Ui::adminwindow *ui;
    QString dataPath;
    void cargarMisPaquetes();
    void cargarSolicitudes();
    bool eliminarDeArchivo(const QString &ruta, int columnaId, const QString &idBuscado);


private slots:
    void on_searchButton_clicked();
    void on_updateStatusButton_clicked();
    void on_filterButton_clicked();
    void on_loadButton_clicked();
    void on_eliminarButton_clicked();
};

#endif // ADMINWINDOW_H
