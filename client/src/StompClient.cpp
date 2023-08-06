
#include <stdlib.h>
#include "../include/SocketThread.h"
#include "../include/Frame.h"
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include "../include/ConnectionHandler.h"

/**
 * This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
 */
int main(int argc, char *argv[])
{
    StompProtocol protocol;
    ConnectionHandler *connectionHandler;
    while (true)
    {   
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        int len = line.length();

        std::string word = protocol.extractWord(line);
        
        while (word != "login" && !protocol.getClientLoggedIn())
        {
            memset(buf, 0, len);
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            len = line.length();
            word = protocol.extractWord(line);
        }
        std::stringstream commandStream(line);
        std::string command, host, login, passcode;
        commandStream >> command >> host >> login >> passcode;
        short port = (short)std::stoi(host.substr(host.find(":") + 1));
        std::string host2 = host.substr(0, host.find(":"));


        GameSummary game = GameSummary(login);
        protocol.setUserName(login);

        Frame connectFrame = Frame("CONNECT");
        connectFrame.addHeader("accept-version", "1.2");
        connectFrame.addHeader("host", "stomp.cs.bgu.ac.il");
        connectFrame.addHeader("login", login);
        connectFrame.addHeader("passcode", passcode);
        connectFrame.setBody("");

        if (!protocol.getActiveCall())
        {
            connectionHandler = new ConnectionHandler(host2, port, protocol);
            std::cout << "Inside StompClient : Created a new connection handlar " <<std::endl;
            if (!connectionHandler->connect())
            {
                std::cout << "Cannot connect to " << host << ":" << port << std::endl;
                return 0;
            }
            protocol.setActiveCall(true);
        }
        
        std::string frameasString = connectFrame.toString();

        if (!connectionHandler->sendLine(frameasString))
        {
            std::cout << "Disconnected. Exiting...\n"<< std::endl;
            return 0;
        }
        std::string answer;
        if (!connectionHandler->getLine(answer))
        {
            std::cout << "Disconnected. Exiting...\n"<< std::endl;

            return 0;
        }
        Frame connectedFrame = Frame::parse(answer);
        if (connectedFrame.getfcommand() != "CONNECTED")
        {
            std::cout << "Disconnected. Exiting...\n"<< std::endl;
            return 0;
        }

        if (connectedFrame.getfcommand() == "CONNECTED")
        {
           
            protocol.setClientLoggedIn(true);
        }
        std::cout << connectedFrame.toString() << std::endl;
        SocketThread socketObj(connectionHandler, game);

        std::thread socketThr(&SocketThread::run, &socketObj);
        // --------------------------------------------------
        while (protocol.getClientLoggedIn())
        {

            const short bufsize2 = 1024;
            char buf2[bufsize2];
            std::cin.getline(buf2, bufsize2);
            if(!protocol.getClientLoggedIn())
                break;
            std::string line2(buf2);
            int len2 = line2.length();
            std::string first = protocol.extractWord(line2);
            std::cout << first << std::endl;
            if (first == "summary")
            {
                std::stringstream commandStream2(line2);
                std::string command2;
                std::string username2;
                std::string channel2;
                std::string filename2;
                commandStream2 >> command2 >> channel2 >> username2 >> filename2;
                game.summary(username2, channel2, filename2);
            }
            else
            {
                std::vector<Frame> frames = protocol.createFrame(line2, first);
                if (frames[0].getfcommand() == "NULL")
                {
                    continue;
                }
                int fSize = frames.size();
                for (int i = 0; i < fSize; i++)
                {
                    std::string packet = frames[i].toString();
                     std::cout <<"THE FRAME SEND TO SERVER IN THE CLIENT IS : " +  packet << std::endl;
                    if (!connectionHandler->sendLine(packet))
                    {
                        std::cout << packet << std::endl;
                        std::cout << "Inside StompClient : ERROR IN TRANSFER";
                    }
                }
            }
            memset(buf2, 0, len2); // clear the buf
        }
        // --------------------------------------------
      
        socketThr.join();
        if(!protocol.getActiveCall()){
            delete connectionHandler;
        }
        
    }

    return 0;
}