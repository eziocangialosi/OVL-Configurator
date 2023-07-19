#ifndef OVL_CONF_H
#define OVL_CONF_H

#include "tarduinocli.h"

#define TRACKER_BOARD   "FireBeetle ESP32-E"
#define TRACKER_FQBN    "DFRobot:esp32:esp32-e"
#define SUIT_BRDS       "/tmp/suitables_boards.list"

class ovl_conf
{
    public:
        short searchSuitableBoards(); //Search the suitable boards connected
    private:
        std::string usedBoard = TRACKER_BOARD;
        tArduinoCLI arduino;

        short catchSuitableBoards(short nbrTotal); //Select the suitable boards
};

#endif // OVL_CONF_H
