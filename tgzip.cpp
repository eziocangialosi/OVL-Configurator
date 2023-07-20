#include "tgzip.h"

tgzip::tgzip()
{

}

short tgzip::extractTar(std::string aSrcFile, std::string aDestPath){

    std::string str_args = "-xf";
    if(aSrcFile != ""){
        if(this->fileExsist(&aSrcFile)){
            str_args += aSrcFile;
            if(!this->fileExsist(&aDestPath)){
                str_args += " -C ";
                str_args += aDestPath;
                if(!this->runTar(&str_args)){
                    return 4;
                }
            }else{
                return 3;
            }
        }else{
            return 2;
        }
    }else{
        return 1;
    }
    return 0;
}

bool tgzip::fileExsist(std::string* apSrcFile)
{
    std::string cmdLine = "test -f ";
    cmdLine += *apSrcFile;
    return (system(cmdLine.c_str()) == 0);
}

bool tgzip::runTar(std::string* pArgs)
{
    std::string cmdLine = "tar ";
    cmdLine += *pArgs;
    return (system(cmdLine.c_str()) == 0);
}
