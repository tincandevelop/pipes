#ifndef PIPE_H
#define PIPE_H

// #pragma once

#include <windows.h>
#include <iostream>

// using namespace std;

enum conn_type{
  server,
  client
};

bool canConnect(std::string in);

class pipe{
  HANDLE Pipe;

public:
  bool connect(conn_type, std::string);
  bool sendMessage(std::string);
  std::string recieveMessage();
  void close();
};

// class connectPipe (){
//
// };

#endif
