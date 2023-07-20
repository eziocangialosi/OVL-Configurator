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

class dlSketch_Task : public QObject
{
        Q_OBJECT

    public slots:
        void runDownload(std::string aDir, std::string aUrl, std::string aSketchPath); //Task to run download
    signals:
        void downloadFinished(const short errorCode);//Signal emitted when the download is finished
    private:
        bool createDir(std::string *apDir);
        bool downloadArchive(twget *apDownloader, std::string* apUrl);
        bool extractSketch(std::string *apSrcPath, std::string *apDestPath);
        bool renameSketchDir(std::string *apCrntPath, std::string *apNewPath);
};

#endif // TASKS_H
