#ifndef TARDUINOCLI_H
#define TARDUINOCLI_H

#include <string>
#include <string.h>
#include <fstream>

#define CMD_EXE "arduino-cli " //Please let the space before the bracket, it's usefull for CLI
#define TMP_BOARD_CLI "/tmp/board.cli"
#define TMP_BOARD_LIST "/tmp/board.list"

class tArduinoCLI
{
    public:
        tArduinoCLI(std::string aSketchPath, std::string aFQBN, std::string aPort);
        tArduinoCLI();
        ~tArduinoCLI();
        bool buildSketch();//Build the sketch
        bool installLibrary(std::string libraryName);//Install the desired libraries
        bool updateLibrariesIndex();//Update the libraries index
        short searchConnectedBoard();//Search the connected boards
        bool createConfigFile();//Create the Arduino-CLI config file
        bool addBoardJsonURL(std::string jsonURL);//Add a 3rd part board with json url
        bool installCore(std::string desiredCore);//Install the desired cores
        bool uploadSketch();//Upload sketch to board
        bool updateCoreIndex();//Update the core index

        void setSketchPath(std::string aPath);//Setter to the sketch path
        void setUsedFQBN(std::string aFQBN);//Setter to the used FQBN
        void setUsedPort(std::string aPort);//Setter to the used serial port
    private:
        std::string sketchPath;
        std::string usedFQBN;
        std::string usedPort;

        void parseBoardFile(short* nbrBoards);

        //Add a parameter to the Arduino-CLI config file
        bool addParamToConfigFile(std::string parameter,std::string value);
};

#endif // TARDUINOCLI_H
