#include "GameSummary.h"
#include <string>


GameSummary::GameSummary(std:: string username) : username(""), data(std::map<string, std::map<string, std::map<string, std::map<string, string>>>>())
{
    addUser(username);
    
}

void GameSummary::addUser(string username)
{
    ThirdLayerMap channels;
   data.insert(std::make_pair(username, channels));
}

void GameSummary::addChannel(string username, string teamA, string teamB)
{   
    string nullStr = "";

    std::map<string,string> generalStats;
    string active = "active";
    string beforeHT = "before halftime";
    generalStats.insert(std::make_pair(active,nullStr));
    generalStats.insert(std::make_pair(beforeHT,nullStr));

    std::map<string,string> teamAStats;
    string goals = "goals";
    string possesions = "possession";
    teamAStats.insert(std::make_pair(goals, nullStr));
    teamAStats.insert(std::make_pair(possesions,nullStr));

    std::map<string,string> teamBStats;
    teamBStats.insert(std::make_pair(goals,nullStr));
    teamBStats.insert(std::make_pair(possesions,nullStr));

    std::map<string,string> reportsBH;
    std::map<string,string> reportsAH;

    TwoLayerMap categories;

    //Game stats:
    string general = "General stats";
    string stats = " stats";
    string tAstats = teamA + stats;
    string tBstats = teamB + stats;
    categories.insert(std::make_pair(general, generalStats));
    categories.insert(std::make_pair(tAstats, teamAStats));
    categories.insert(std::make_pair(tBstats, teamBStats));

    //Game event reports:
    string reportbefore = "Reports before half time";
    string reportafter = "Reports after half time";
    categories.insert(std::make_pair(reportbefore,reportsBH));
    categories.insert(std::make_pair(reportafter,reportsAH));
        string vs = " vs ";
        //importent 
        string channelname = teamA + vs + teamB;
        data[username].insert(std::make_pair(channelname, categories));
   // }
}

void GameSummary::summary(string username, string channel, string filename)
{
    
std::ofstream file(filename, std::ios::app); // open file in append mode
  if (file.is_open()) {
    string body = writeSummary(username, channel);
    file << body;
    file.close();
  } else {
    std::cout << "Could not open file.";
  }

}

string GameSummary::writeSummary(string username, string channel)
{
    string summary;
    size_t pos = channel.find('_');
    string teamA;
    string teamB;
    if (pos != string::npos) 
    {
        teamA = channel.substr(0, pos);
        teamB = channel.substr(pos + 1);
        channel.replace(pos, 1, " vs ");
    }
    summary = summary + channel + "\n Game stats: \n General stats: \n active: ";
    string activeD = data[username][channel]["General stats"]["active"];
    summary = summary + activeD +"\n before halftime: ";
    string bhD = data[username][channel]["General stats"]["before halftime"];
    //------team A stats:
    summary = summary + bhD + "\n" + teamA + " stats:";

    string headerA = teamA + " stats";
    string goalsA = data[username][channel][headerA]["goals"];
    summary = summary + goalsA + "\n possession: ";

    string posA = data[username][channel][headerA]["possession"];
    summary = summary + posA + " \n " + teamB +" stats: ";

    string headerB = teamB + " stats";
    string goalsB = data[username][channel][headerB]["goals"];
    summary = summary + goalsB + "\n possession: ";

    string posB = data[username][channel][headerB]["possession"];
    summary = summary + posB + " \n Game event reports: \n";

     for (const auto& pair : data[username][channel]["Reports before half time"]) {
      summary = summary + pair.second + "\n\n";
     }

     for (const auto& pair : data[username][channel]["Reports after half time"]) {
      summary = summary + pair.second + "\n\n";
     }

    return summary;


}

void GameSummary::deleteChannel(string username, string teamA, string teamB)
{
    std::string channelname = teamA + " vs " + teamB;
    data[username].erase(channelname);
}
