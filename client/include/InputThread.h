#pragma once
#include "../include/ConnectionHandler.h"
#include "../include/GameSummary.h"

class InputThread{

private:
                  ConnectionHandler &ch;
                  GameSummary &gs;


public:
InputThread(ConnectionHandler& connectionHandler,  GameSummary & game);
void run();

};