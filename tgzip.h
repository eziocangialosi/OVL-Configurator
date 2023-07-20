#ifndef TGZIP_H
#define TGZIP_H

#include <string>

class tgzip
{
    public:
        tgzip();
        short extractTar(std::string aSrcFile, std::string aDestPath);
    private:
        bool fileExsist(std::string* apSrcFile);
        bool runTar(std::string *pArgs);
};

#endif // TGZIP_H
