#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    h_win = new homewindow;
    h_win->setRequester(&this->api);
    h_win->show();
    if(!h_win->rqInstAddr()){
        this->close();
    }
    connect(h_win,&homewindow::sig_requestMainWindow,this,&MainWindow::on_request_show);
    QPixmap pix_logo(":/OVL-Config_logo_name_white.png");
    ui->lb_logo->setPixmap(pix_logo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_request_show(){
    this->h_win->close();
    this->show();
    this->setWindowState(Qt::WindowState::WindowActive);
    this->activateWindow();
}
