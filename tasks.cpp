#include "tasks.h"

void searchBoard_Task::runSearch(ovl_conf* apUtils)
{
    short nbrBoards;
    nbrBoards = apUtils->searchSuitableBoards();
    emit(this->searchFinished(nbrBoards));

}
