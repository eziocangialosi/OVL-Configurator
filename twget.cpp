#include "twget.h"

twget::twget()
{
    std::string cmdLine = "mkdir ";
    cmdLine += DEFAULT_DLPATH;
    system(cmdLine.c_str());
}

twget::twget(std::string* pPath)
{
    this->dlPath = *pPath;
}

bool twget::download(std::string *pURL)
{
    std::string cmdLine = "cd ";
    if(pURL->empty()){
        return false;
    }else{
        cmdLine += this->dlPath;
        cmdLine += " && wget ";
        cmdLine += *pURL;
    }
    if(system(cmdLine.c_str()) == 0){
        return true;
    }else{
        return false;
    }
}

twget::~twget()
{
    if(this->dlPath == DEFAULT_DLPATH){
        std::string cmdLine = "rm ";
        cmdLine += DEFAULT_DLPATH;
        cmdLine += "/* && rmdir ";
        cmdLine += DEFAULT_DLPATH;
        system(cmdLine.c_str());
    }
}
