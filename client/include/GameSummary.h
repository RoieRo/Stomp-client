#pragma once
#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include "../include/event.h"


using string = std::string;
using FourthLayerMap = std::map<string, std::map<string, std::map<string, std::map<string, string>>>>;
using ThirdLayerMap = std::map<string, std::map<string, std::map<string,string>>>;
using TwoLayerMap = std::map<string, std::map<string, string>>;



class GameSummary{
    private:
    string username;
    FourthLayerMap data;

    public:

    GameSummary(string username);

    void addUser(string username);

    void addChannel(string username, string teamA, string teamB);

    void summary(string username, string channel, string file);

    string writeSummary(string username, string channel);
    //Shira
    void deleteChannel(string username, string teamA, string teamB);

    void insertEvent(Event event, std::string username);
};
  
