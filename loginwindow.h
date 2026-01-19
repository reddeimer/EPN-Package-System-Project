#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class loginwindow;
}

enum UserType {
    Cliente,
    Administrador
};

class loginwindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginwindow(UserType type, QWidget *parent = nullptr);
    ~loginwindow();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::loginwindow *ui;
    UserType userType;
    QString dataFile;
    bool validateLogin(const QString &cedula, const QString &password);
};

#endif // LOGINWINDOW_H
