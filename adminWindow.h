#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void onSearchButtonClicked();
    void onUpdateStatusButtonClicked();
    void onFilterButtonClicked();

private:
    Ui::AdminWindow *ui;

    void loadPackages();
};

#endif // ADMINWINDOW_H
