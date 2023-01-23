#pragma once
#include <iostream>

using namespace std;

bool GetServer(char* call, short port, struct sockaddr_in* from, int* flen, string* pipeName);

// Error handling
std::string GetErrorMsgText(int code);
std::string SetErrorMsgText(std::string msgText, int code);