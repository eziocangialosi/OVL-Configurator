#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QThread>

#include "homewindow.h"
#include "ovl_requester.h"
#include "ovl_conf.h"
#include "tasks.h"

#define LB_USERNAME "Connected as : "

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread threadBoardResearch;
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_request_show();
        void onDevicesChange(QString);
        void onBoardSearchFinished(const short nbrSuitBoards);
    signals:
        void runSearchBoards(ovl_conf* apUtils);//Used to run the task to search boards in a thread
    private:
        Ui::MainWindow *ui;
        homewindow *h_win;
        ovl_requester api;
        ovl_conf config;
        QFileSystemWatcher *USBwatcher;
        searchBoard_Task *pSearchBoard_Task;
        short suitBoards = 0;
        short oldSuitBoards = -1;
        bool boardTaskRunning = false;

        void searchBoards();

};
#endif // MAINWINDOW_H
