#ifndef TWGET_H
#define TWGET_H

#define DEFAULT_DLPATH "/tmp/WGETtool"

#include <string>

class twget
{
    public:
        twget();
        twget(std::string* pPath);
        bool download(std::string* pURL); //Download from the given URL
        ~twget();
    private:
        std::string dlPath = DEFAULT_DLPATH;
        std::string filePath;
};

#endif // TWGET_H
