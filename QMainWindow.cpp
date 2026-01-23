#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rolewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Conexión explícita para mayor claridad C++
    connect(ui->logoutButton, &QPushButton::clicked,
            this, &MainWindow::onLogoutButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogoutButtonClicked()
{
    RoleWindow *roleWindow = new RoleWindow();
    roleWindow->show();
    close();
}
