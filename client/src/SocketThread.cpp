#include "../include/SocketThread.h"

#include <thread>

SocketThread::SocketThread(ConnectionHandler *connectionHandler, GameSummary &game) : ch(connectionHandler), gs(game) {}

void SocketThread::run()
{
    while (ch!=nullptr && ch->getProtocol().getClientLoggedIn() && ch->getProtocol().getActiveCall())
    {

        std::string stringFrame;
        if (!ch->getFrameAscii(stringFrame, '\0'))
        {
            std::cout << "Disconnected. Exiting...\n"
                      << std::endl;
                      continue;
        }

        Frame frame;
        frame = frame.parse(stringFrame);

        if (frame.getfcommand() == "RECEIPT")
        {
            std::vector<std::string> vector = ch->getProtocol().receiptFrame(frame);

            // In case we got aproval to disconnect
            if (vector[0] == "logout approved")
            {
                return;
            }
            if (vector[0] == "SUBSCRIBE")
            {
                gs.addChannel(vector[1], vector[2], vector[3]);
                continue;
            }
            if (vector[0] == "UNSUBSCRIBE")
            {
                gs.deleteChannel(vector[1], vector[2], vector[3]);
                continue;
            }
            if(vector[0] =="none"){
                continue;
            }
        }

        if (frame.getfcommand() == "ERROR")
        {
            std::cout << frame.toString() << std::endl;
            ch->getProtocol().setClientLoggedIn(false);
            ch->getProtocol().setActiveCall(false);
            return;
        }
        if (frame.getfcommand() == "MESSAGE")
        {
            string destination = frame.getValue("destination");
            string subscription = frame.getValue("subscription");
            string messageId = frame.getValue("message-id");
            string messageBody = frame.getBody();
            Event event(messageBody);
            
            //ADD INSRTEVENT FROM GAME SUMMERY
            return;

        }

    }
}
