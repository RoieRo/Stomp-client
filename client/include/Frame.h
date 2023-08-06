#pragma once
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <atomic>
#include "Counter.h"

class Frame
{
private:
std::string fcommand;
std::map<std::string, std::string> headers;
std::string body;
static Counter receiptCounter;
static Counter subCounter;


public:
Frame();
Frame(const std::string &cmd);
void addHeader(const std::string &key, const std::string &value);
void setBody(std::string newBody);
std::string getBody();
std::string toString() const;
static Frame parse(const std::string &serializedFrame);
std::string getfcommand();
static void incrementSubCounter();
static int getSubCounter();
static void incrementReceiptCounter();
static int getReceiptCounter();
std::map<std::string, std::string> getHeaders();
std::string getValue(std::string key);
};
