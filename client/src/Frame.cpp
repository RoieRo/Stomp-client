#include "../include/Frame.h"
Frame::Frame() : fcommand(""), headers(std::map<std::string, std::string>()),body(""){
}

Frame::Frame(const std::string &cmd) : fcommand(cmd), headers(std::map<std::string, std::string>()),body(""){
   
}

Counter Frame::receiptCounter;
Counter Frame::subCounter;
void Frame::addHeader(const std::string &key, const std::string &value)
   {
      headers[key] = value;
   }
/////////
std::map<std::string, std::string> Frame :: getHeaders(){
   return headers;
}
std::string Frame:: getValue(std::string key) {
            std::string value = headers[key];
            return value;
        }

////////

std::string Frame::getBody(){
   return body;

}
void Frame::setBody(std::string newBody){
   body = newBody;
}

   std::string Frame::toString() const
   {
      std::stringstream ss;
      ss << fcommand << "\n";
      for (const auto &p : headers)
      {
         const auto &key = p.first;
         const auto &value = p.second;
         ss << key << ":" << value << "\n";
      }
      ss << "\n"
         << body ;
      return ss.str();
   }



Frame Frame::parse(const std::string &serializedFrame)
{
{
   std::stringstream ss(serializedFrame);
   std::string line;

   std::getline(ss, line);
   Frame frame(line);
   while (std::getline(ss, line) && line != "")
   {
      auto index = line.find(':');
      if (index != std::string::npos)
      {
         std::string key = line.substr(0, index);
         std::string value = line.substr(index + 1);
         frame.addHeader(key, value);
      }
      else{
         break;
      }
   }
   std::string body;
   std::getline(ss, body, '\0');
   frame.body = body;
   return frame;
}

}
std::string Frame::getfcommand(){
   return fcommand;
}
void Frame::incrementSubCounter(){
   subCounter.increment();
}

int Frame::getSubCounter(){
   return subCounter.get();
}
void Frame::incrementReceiptCounter()
{
  receiptCounter.increment();
}

int Frame::getReceiptCounter(){
   return receiptCounter.get();
}
