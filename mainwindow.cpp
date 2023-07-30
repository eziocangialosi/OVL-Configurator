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

    this->sB_gprs_pin = new digitSpinBox(this->ui->tab_essential);
    this->sB_gprs_pin->setGeometry(680,300,60,30);
    this->sB_gprs_pin->setMinimum(0);
    this->sB_gprs_pin->setMaximum(9999);
    this->sB_gprs_pin->setDisabled(true);
}

MainWindow::~MainWindow()
{
    threadBoardResearch.quit();
    threadBoardResearch.wait();

    threadDlSketch.quit();
    threadDlSketch.wait();
    this->config.clearDir();
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

    pDlSketch_Task = new dlSketch_Task();
    pDlSketch_Task->moveToThread(&threadDlSketch);
    connect(&threadDlSketch, &QThread::finished, pDlSketch_Task, &QObject::deleteLater);
    connect(this, &MainWindow::runDlSketch, pDlSketch_Task, &dlSketch_Task::runDownload);
    connect(pDlSketch_Task, &dlSketch_Task::downloadFinished, this, &MainWindow::onDlSketchFinished);
    threadDlSketch.start();

    USBwatcher = new QFileSystemWatcher(this);
    USBwatcher->addPath("/dev");
    connect(USBwatcher, SIGNAL(directoryChanged(QString)), this, SLOT(onDevicesChange(QString)));

    this->ui->tabSettings->setDisabled(true);

    this->ui->cB_board->addItem("No Boards Found");
    this->searchBoards();
    emit(this->runDlSketch(WORK_DIR,REPO_URL,SKETCH_DIR));

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

void MainWindow::onDlSketchFinished(const short errorCode){
//    QMessageBox::information(this,"dl finished",QString::number(errorCode),QMessageBox::Ok);
//    qDebug()<<errorCode;
}

void MainWindow::on_cB_board_currentIndexChanged(int index)
{
    if(index == 0){
        this->ui->tabSettings->setDisabled(true);
//        this->ui->tabSettings->setCurrentIndex(0);
    }else{
        this->ui->tabSettings->setEnabled(true);
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1){
        this->sB_gprs_pin->setEnabled(true);
    }else{
        this->sB_gprs_pin->setDisabled(true);
        this->sB_gprs_pin->setValue(0);
    }
}

