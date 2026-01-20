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
    explicit adminwindow(QWidget *parent = nullptr);
    ~adminwindow();

private:
    Ui::adminwindow *ui;
    void loadPackages();

private slots:
    void on_searchButton_clicked();
    void on_updateStatusButton_clicked();
    void on_filterButton_clicked();
};

#endif // ADMINWINDOW_H
