#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &cedula, const QString &nombre, const QString &dataPath, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buscarButton_clicked();

    void on_requestButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    QString cedulaCliente;
    QString nombreCliente;
    QString dataPath;
    void cargarMisPaquetes();
};
#endif // MAINWINDOW_H
