#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QString>

// Enumeración de tipos de usuario (C++ moderno)
enum class UserType {
    Cliente,
    Administrador
};

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(UserType type, QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onRegisterConfirmButtonClicked();

private:
    Ui::LoginWindow *ui;

    UserType userType;
    QString dataFilePath;

    bool validateLogin(const QString &cedula, const QString &password);
    void resetToLoginView();
    void openNextWindow();
};

#endif // LOGINWINDOW_H
