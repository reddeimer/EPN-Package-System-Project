#ifndef ROLEWINDOW_H
#define ROLEWINDOW_H

#include <QWidget>

namespace Ui {
class rolewindow;
}

class rolewindow : public QWidget
{
    Q_OBJECT

public:
    explicit rolewindow(QWidget *parent = nullptr);
    ~rolewindow();

private:
    Ui::rolewindow *ui;

private slots:
    void on_clientButton_clicked();
    void on_adminButton_clicked();
    void on_exitButton_clicked();
};

#endif // ROLEWINDOW_H
