#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    h_win = new homewindow;
    h_win->show();
    if(!h_win->rqInstAddr()){
        this->close();
    }
    connect(h_win,&homewindow::sig_requestMainWindow,this,&MainWindow::on_request_show);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_request_show(){
    this->show();
}
