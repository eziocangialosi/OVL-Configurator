#ifndef TASKS_H
#define TASKS_H

#include <QObject>
#include "ovl_conf.h"

class searchBoard_Task : public QObject
{
        Q_OBJECT

    public slots:
        void runSearch(ovl_conf* apUtils); //Task to run the board search
    signals:
        void searchFinished(const short nbrSuitBoards);//Signal emitted when the search is finished
};

#endif // TASKS_H
