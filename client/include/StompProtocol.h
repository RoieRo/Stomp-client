#pragma once

#include "../include/event.h"
#include "../include/Frame.h"
#include "../include/GameSummary.h"
// #include <boost/asio.hpp>
// using boost::asio::ip::tcp;

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
bool clientLoggedIn;
bool activeCall;
std::string disconnectReceipt;
std::map <std::string, std::string> unSubscribeReceipt;
std::map <std::string, std::string>  subscribeReceipt;
std::string userName;
std::map <std::string, int> topicID;


//add events

public:
/// StompProtocol::StompProtocol(bool clientLoggedIn ,std::string disconnectRecipt){}
StompProtocol();
std::string extractWord(std::string cmd);

std::vector<Frame> createFrame(std::string cmd, std::string word);

std::vector<std::string> receiptFrame(Frame frame);


void setClientLoggedIn( bool newResclientLoggedInet);

void setDisconnectReceipt(std::string num);







bool getClientLoggedIn();

std::string getDisconnectReceipt();


void setUserName(std::string newUsername);
bool getActiveCall();

void setActiveCall(bool active);


};
