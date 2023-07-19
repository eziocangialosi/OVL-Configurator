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
    threadBoardResearch.quit();
    threadBoardResearch.wait();
    delete ui;
}

void MainWindow::searchBoards()
{
    static bool haveBeenRun = false;
    if(haveBeenRun){
        this->oldSuitBoards = this->suitBoards;
    }
    haveBeenRun = true;

    if(!this->boardTaskRunning){
        this->boardTaskRunning = true;
        emit(this->runSearchBoards(&this->config));
    }
}

void MainWindow::on_request_show(){
    this->h_win->close();

    QString labelText = LB_USERNAME + this->api.getUserMail();
    QString windowTitle = "OVL-Configurator - " + this->api.getUserMail();
    this->ui->lb_username->setText(labelText);
    this->setWindowTitle(windowTitle);

    pSearchBoard_Task = new searchBoard_Task();
    pSearchBoard_Task->moveToThread(&threadBoardResearch);
    connect(&threadBoardResearch, &QThread::finished, pSearchBoard_Task, &QObject::deleteLater);
    connect(this, &MainWindow::runSearchBoards, pSearchBoard_Task, &searchBoard_Task::runSearch);
    connect(pSearchBoard_Task, &searchBoard_Task::searchFinished, this, &MainWindow::onBoardSearchFinished);
    threadBoardResearch.start();

    USBwatcher = new QFileSystemWatcher(this);
    USBwatcher->addPath("/dev");
    connect(USBwatcher, SIGNAL(directoryChanged(QString)), this, SLOT(onDevicesChange(QString)));

    this->ui->cB_board->addItem("No Boards Found");
    this->searchBoards();

    this->show();
    this->setWindowState(Qt::WindowState::WindowActive);
    this->activateWindow();
}

void MainWindow::onDevicesChange(QString)
{
    this->searchBoards();
}

void MainWindow::onBoardSearchFinished(const short nbrSuitBoards){
    this->suitBoards = nbrSuitBoards;
    if(this->suitBoards < 1){
        if(!(this->suitBoards < 0)){
            this->ui->cB_board->setDisabled(true);
            this->ui->cB_board->clear();
            this->ui->cB_board->addItem("No Boards Found");
        }else{
            QMessageBox::warning(this, "Error", "Error when trying to search connected boards...");
        }
    }else{
        std::ifstream fileBoards(SUIT_BRDS);
        if(this->suitBoards < this->oldSuitBoards){
            this->ui->cB_board->clear();
        }
        this->ui->cB_board->setEnabled(true);
        for(short i = 0; i < this->suitBoards; i++){
            char str[255];
            fileBoards.getline(str,255);
            if(i == 0){
                this->ui->cB_board->setItemText(0,"Please select a board");
                this->ui->cB_board->addItem(QString::fromStdString(str));
            }else{
                this->ui->cB_board->addItem(QString::fromStdString(str));
            }
        }
        fileBoards.close();
    }
    this->boardTaskRunning = false;
}
