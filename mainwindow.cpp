#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    h_win = new homewindow;
    h_win->show();
    h_win->rqInstAddr();
}

MainWindow::~MainWindow()
{
    delete ui;
}

