#include "tarduinocli.h"

tArduinoCLI::tArduinoCLI(){}

tArduinoCLI::tArduinoCLI(std::string aSketchPath, std::string aFQBN, std::string aPort)
{
    this->sketchPath = aSketchPath;
    this->usedFQBN = aFQBN;
    this->usedPort = aPort;
}

bool tArduinoCLI::buildSketch()
{
    bool flag = !this->usedFQBN.empty();
    if(flag){
        if(!this->sketchPath.empty()){
            std::string cmdLine = CMD_EXE;
            cmdLine += "compile --fqbn ";
            cmdLine += this->usedFQBN;
            cmdLine += " ";
            cmdLine += this->sketchPath;
            if(system(cmdLine.c_str()) != 0){
                flag = false;
            }
        }else{
            flag = false;
        }
    }
    return flag;
}

bool tArduinoCLI::installCore(std::string desiredCore)
{
    std::string cmdLine = CMD_EXE;
    cmdLine += "core install ";
    cmdLine += desiredCore;
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

bool tArduinoCLI::uploadSketch()
{
    bool flag = (!this->usedPort.empty() && !this->usedFQBN.empty() && !this->sketchPath.empty());
    if(flag){
        std::string cmdLine = CMD_EXE;
        cmdLine += "upload -p ";
        cmdLine += usedPort;
        cmdLine += " --fqbn ";
        cmdLine += usedFQBN;
        cmdLine += " ";
        cmdLine += sketchPath;
        if(system(cmdLine.c_str()) != 0){
            flag = false;
        }
    }
    return flag;
}

short tArduinoCLI::searchConnectedBoard()
{
    std::string cmdLine = CMD_EXE;
    cmdLine += "board list >> ";
    cmdLine += TMP_BOARD_CLI;
    if(system(cmdLine.c_str()) != 0){
        return -1;
    }else{
        short boardNbr;
        parseBoardFile(&boardNbr);
        std::string rmLine = "rm ";
        rmLine += TMP_BOARD_CLI;
        system(rmLine.c_str());
        return boardNbr;
    }
}

void tArduinoCLI::parseBoardFile(short *nbrBoards)
{
    std::ifstream fileOpen(TMP_BOARD_CLI);
    std::ofstream fileOut(TMP_BOARD_LIST);

    bool flag = (fileOpen.is_open() && fileOut.is_open());
    if(flag){
        char strLine[255];
        fileOpen.getline(strLine,255);
        std::string stdStrLine = strLine;
        unsigned int protocolSeek = stdStrLine.find("Protocol");
        unsigned int boardSeek = stdStrLine.find("Board");
        unsigned int fqbnSeek = stdStrLine.find("FQBN");
        *nbrBoards = 0;
        char lineData[255];
        do{
            fileOpen.getline(lineData,255);
            if(fileOpen.good() && lineData[0] != '\0'){
                unsigned int i = 0;
                for(i = 0; i < protocolSeek; i++){
                    fileOut.put(lineData[i]);
                }
                fileOut.put('(');
                for(i = boardSeek; i < fqbnSeek-1; i++){
                    fileOut.put(lineData[i]);
                }
                fileOut.put(')');
                fileOut.put('\n');
                *nbrBoards+=1;
            }
        }while(!fileOpen.eof());
    }else{
        *nbrBoards = -1;
    }
    fileOpen.close();
    fileOut.close();
}

void tArduinoCLI::setSketchPath(std::string aPath){
    this->sketchPath = aPath;
}

void tArduinoCLI::setUsedFQBN(std::string aFQBN){
    this->usedFQBN = aFQBN;
}

void tArduinoCLI::setUsedPort(std::string aPort){
    this->usedPort = aPort;
}

bool tArduinoCLI::updateLibrariesIndex(){
    std::string cmdLine = CMD_EXE;
    cmdLine += "lib update-index";
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

bool tArduinoCLI::installLibrary(std::string libraryName){
    std::string cmdLine = CMD_EXE;
    cmdLine += "lib install ";
    cmdLine += libraryName;
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

bool tArduinoCLI::addParamToConfigFile(std::string parameter,std::string value){
    std::string cmdLine = CMD_EXE;
    cmdLine += "config add ";
    cmdLine += parameter;
    cmdLine += " ";
    cmdLine += value;
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

bool tArduinoCLI::addBoardJsonURL(std::string jsonURL){
    return this->addParamToConfigFile("board_manager.additional_urls", jsonURL);
}

bool tArduinoCLI::createConfigFile(){
    std::string cmdLine = CMD_EXE;
    cmdLine += "config init";
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

bool tArduinoCLI::updateCoreIndex(){
    std::string cmdLine = CMD_EXE;
    cmdLine += "core update-index";
    if(system(cmdLine.c_str()) != 0){
        return false;
    }
    return true;
}

tArduinoCLI::~tArduinoCLI(){
    std::string cmdLine = "rm ";
    cmdLine += TMP_BOARD_LIST;
    system(cmdLine.c_str());
}
