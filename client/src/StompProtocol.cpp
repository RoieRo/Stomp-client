#include "../include/StompProtocol.h"

StompProtocol::StompProtocol() : clientLoggedIn(false),activeCall(), disconnectReceipt("null"), unSubscribeReceipt(std::map<std::string, std::string>()), subscribeReceipt(std::map<std::string, std::string>()), userName(""), topicID(std::map<std::string, int>()) {}

std::string StompProtocol::extractWord(std::string cmd)
{
   std::string word;
   std::stringstream ss(cmd);
   ss >> word;
   return word;
}

std::vector<Frame> StompProtocol::createFrame(std::string cmd, std::string word)
{

   if (clientLoggedIn)
   {
      if (word == "join")
      {
         std::stringstream commandStream(cmd);
         std::string command;
         std::string game_name;

         commandStream >> command >> game_name;
         Frame subframe = Frame("SUBSCRIBE");
         subframe.addHeader("destination", game_name);

         int intId = subframe.getSubCounter();
         std::string id = std::to_string(intId);
         subframe.incrementSubCounter();
         subframe.addHeader("id", id);

         std::string receiptID = std::to_string(subframe.getReceiptCounter());
         subframe.incrementReceiptCounter();
         subframe.addHeader("receipt", receiptID);
         subframe.setBody("");
         subscribeReceipt.insert(std::make_pair(receiptID, game_name));
         // ADD NEW TOPIC TO EVENTS
         topicID.insert(std::pair<std::string, int>(game_name, intId));
         std::vector<Frame> vector = {subframe};
         return vector;
      }

      if (word == "exit")
      {
         std::stringstream commandStream(cmd);
         std::string command;
         std::string game_name;

         commandStream >> command >> game_name;

         Frame unsubframe = Frame("UNSUBSCRIBE");

         int id = topicID.at(game_name);
         unsubframe.addHeader("id", std::to_string(id));

         std::string receiptID = std::to_string(unsubframe.getReceiptCounter());
         unsubframe.incrementReceiptCounter();
         unsubframe.addHeader("receipt", receiptID);
         unSubscribeReceipt.insert(std::make_pair(receiptID, game_name));
         unsubframe.setBody("");
         // removes the subscribtion id from topicId
         topicID.erase(game_name);

         std::vector<Frame> vector = {unsubframe};
         return vector;
      }

      if (word == "report")
      {
         std::stringstream commandStream(cmd);
         std::string command;
         std::string json_path;


         std::vector<std::string> split_string;
         std::vector<std::string> words;
         std::istringstream iss(cmd);
         std::string word;
         //commandStream >> command >> json_path;
         while (iss >> word) {
            words.push_back(word);
         }
         command = words[0];
         int sizeWord= words.size();
          json_path = words[1];
         for(int i=2; i<sizeWord;i++){
            json_path = json_path +" " +words[i];
         }
         

         
         names_and_events data = parseEventsFile(json_path);
 
         // converts a vector of events to a vector of SEND frames:
         std::string game_name = data.team_a_name + "_" + data.team_b_name;

         std::vector<Frame> frameVec;
         // TODO:ADD events TO EVENT DATA STRUCTURE
         int size = data.events.size();
         for (int i = 0; i < size; i++)
         {
            Frame sendframe = Frame("SEND");
            sendframe.addHeader("destination", game_name);

            //Body:
            std::string body;
            body = body + "user: "+ userName +"\n";
            if (i == 0)
            { // CHECK
               body = body + "team a: "+ data.events[i].get_team_a_name() +"\n";
               body = body + "team b: " + data.events[i].get_team_b_name() +"\n";
            }
            body = body + "event name: " + data.events[i].get_name() +"\n";

            body = body + "time: " + std::to_string(data.events[i].get_time()) +"\n";
            // run at the map of general updates and insert to string

            std::string strGeneral = "";

            for (auto it = data.events[i].get_game_updates().begin(); it != data.events[i].get_game_updates().end(); ++it)
            {
               strGeneral += it->first + ":" + it->second + "\n";
            }
            body = body + "general game updates: " + strGeneral +"\n";

            // run at the map of general team a updates and insert to string

            std::string strGeneralTeamA = "";

            for (auto it = data.events[i].get_team_a_updates().begin(); it != data.events[i].get_team_a_updates().end(); ++it)
            {
               strGeneralTeamA += it->first + ":" + it->second + "\n";
            }

            body = body + "team a updates: " + strGeneralTeamA +"\n";

            // run at the map of general team b updates and insert to string

            std::string strGeneralTeamB = "";

            for (auto it = data.events[i].get_team_b_updates().begin(); it != data.events[i].get_team_b_updates().end(); ++it)
            {
               strGeneralTeamB += it->first + ": " + it->second + "\n";
            }

            body = body + "team b updates: " + strGeneralTeamB +"\n";
            // get the description
            body = body + "descripion: " +data.events[i].get_discription()+"\n";

            sendframe.setBody(body);
            
            frameVec.push_back(sendframe);

         }
         int sizet=frameVec.size();
         for(int i=0; i<sizet;i++){
         }
         return frameVec;
      }
      if (word == "logout")
      {
         Frame discoframe = Frame("DISCONNECT");
         std::string receiptID = std::to_string(discoframe.getReceiptCounter());
         std::cout<< "Inside StompProtocol, Creat Frame: disconnect case, the disconnectrecipt is:"+receiptID<< std::endl;
         disconnectReceipt = receiptID;
         discoframe.incrementReceiptCounter();
         discoframe.addHeader("receipt", receiptID);
         discoframe.setBody("");
         std::vector<Frame> vector = {discoframe};
         return vector;
      }
   }

   Frame nullFrame = Frame("NULL");
   std::vector<Frame> vector = {nullFrame};
   return vector;
}

void StompProtocol::setClientLoggedIn(bool newResclientLoggedInet)
{
   clientLoggedIn = newResclientLoggedInet;
}


bool StompProtocol::getClientLoggedIn()
{
   return clientLoggedIn;
}

std::string StompProtocol::getDisconnectReceipt()
{
   return disconnectReceipt;
}

void StompProtocol::setDisconnectReceipt(std::string num)
{
   disconnectReceipt = num;
}

void StompProtocol::setUserName(std::string newUsername)
{
   userName = newUsername;
}

std::vector<std::string> StompProtocol::receiptFrame(Frame frame)
{

   std::vector<std::string> vecnull{"none"};
   
      if (frame.getValue("receipt-id") == disconnectReceipt)
      {
         clientLoggedIn = false;
         disconnectReceipt = "null";
         std::vector<std::string> vector{"logout approved"};
         return vector;
      }
      
      if (subscribeReceipt.count(frame.getValue("receipt-id")) == 1)
      {

         std::string channel = subscribeReceipt[frame.getValue("receipt-id")];
         int indexOfColon = channel.find('_');
         std::string teamA = channel.substr(0, indexOfColon);
         std::string teamB = channel.substr(indexOfColon + 1);
         std::string subscribe = "SUBSCRIBE";
         std::vector<std::string> vector{subscribe, userName, teamA, teamB};
         subscribeReceipt.erase(frame.getValue("receipt-id"));
         return vector;
      }

      if (unSubscribeReceipt.count(frame.getValue("receipt-id")) == 1)
      {
         std::string channel = unSubscribeReceipt[frame.getValue("receipt-id")];
         int indexOfColon = channel.find('_');
         std::string teamA = channel.substr(0, indexOfColon);
         std::string teamB = channel.substr(indexOfColon + 1);
         std:: string unsub ="UNSUBSCRIBE"; 
         std::vector<std::string> vector{unsub, userName, teamA, teamB};
       
         unSubscribeReceipt.erase(frame.getValue("receipt-id"));
         return vector;
      }
   
   return vecnull;
}

bool StompProtocol::getActiveCall(){
   return activeCall;
}

void StompProtocol::setActiveCall(bool active){
   activeCall = active;
}

