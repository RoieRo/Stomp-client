#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/GameSummary.h"

class SocketThread
{
    private:
    ConnectionHandler *ch;
    GameSummary &gs;


    public:
    SocketThread(ConnectionHandler* connectionHandler, GameSummary& game);
    
    void run();
    
};
