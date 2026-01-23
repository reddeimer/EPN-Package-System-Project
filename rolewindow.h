#ifndef ROLEWINDOW_H
#define ROLEWINDOW_H

#include <QWidget>

namespace Ui {
class RoleWindow;
}

class RoleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RoleWindow(QWidget *parent = nullptr);
    ~RoleWindow();

private slots:
    void onClientButtonClicked();
    void onAdminButtonClicked();
    void onExitButtonClicked();

private:
    Ui::RoleWindow *ui;
};

#endif // ROLEWINDOW_H
