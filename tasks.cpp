#include "tasks.h"

void searchBoard_Task::runSearch(ovl_conf* apUtils)
{
    short nbrBoards;
    nbrBoards = apUtils->searchSuitableBoards();
    emit(this->searchFinished(nbrBoards));

}

void dlSketch_Task::runDownload(std::string aDir, std::string aUrl, std::string aSketchPath)
{
    short errorCode = 0;
    twget downloader(&aDir);
    if(!this->createDir(&aDir)){
        errorCode = 1;
    }else{
        if(!this->downloadArchive(&downloader, &aUrl)){
            errorCode = 2;
        }else{
            std::string archivePath = aDir + ARCHIVE_NAME;
            if(!this->extractSketch(&archivePath, &aDir)){
                errorCode = 3;
            }else{
                std::string aCrntPath = (aSketchPath + "*");
                if(!this->renameSketchDir(&aCrntPath, &aSketchPath)){
                    errorCode = 4;
                }
            }
        }
    }
    emit(this->downloadFinished(errorCode));
}

bool dlSketch_Task::downloadArchive(twget* apDownloader, std::string* apUrl)
{
    return apDownloader->download(apUrl);
}

bool dlSketch_Task::createDir(std::string* apDir)
{
    std::string cmdLine = "test -d ";
    cmdLine += *apDir;
    if(system(cmdLine.c_str()) != 0){
        std::string cmdLine = "mkdir ";
        cmdLine += *apDir;
        return (system(cmdLine.c_str()) == 0);
    }
    return true;
}

bool dlSketch_Task::extractSketch(std::string* apSrcPath, std::string* apDestPath)
{
    tgzip extractor;
    return (extractor.extractTar(*apSrcPath,*apDestPath) == 0);
}

bool dlSketch_Task::renameSketchDir(std::string *apCrntPath, std::string *apNewPath){
    std::string cmdLine = "mv ";
    cmdLine += *apCrntPath;
    cmdLine += " ";
    cmdLine += *apNewPath;
    return (system(cmdLine.c_str()) == 0);
}
