#include "ovl_conf.h"

short ovl_conf::searchSuitableBoards()
{
    short nbrBoards;
    nbrBoards=arduino.searchConnectedBoard();
    if(nbrBoards < 1){
        return nbrBoards;
    }else{
        return catchSuitableBoards(nbrBoards);
    }
}

short ovl_conf::catchSuitableBoards(short nbrTotal)
{
    std::ifstream fileOpen(TMP_BOARD_LIST);
    std::ofstream fileOutput(SUIT_BRDS);
    if(!fileOpen.is_open() && !fileOutput.is_open()){
        return -1;
    }
    char strLine[255];
    short suitBoards = 0;
    for(short i = 0; i < nbrTotal; i++){
        fileOpen.getline(strLine,255);
        std::string stdStrLine = strLine;
        if(stdStrLine.find(usedBoard)){
            fileOutput << stdStrLine << "\n";
            suitBoards++;
        }
    }
    return suitBoards;
}
