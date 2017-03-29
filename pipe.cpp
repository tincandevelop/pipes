#include "pipe.h"

wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}
std::string GetString(wchar_t* txt){
  std::wstring ws(txt);
  // your new String
  std::string str(ws.begin(), ws.end());
  return str;
}

bool pipe::connect(conn_type type,std::string path){
  if(type == server){
    Pipe = CreateNamedPipe(
      TEXT(path.c_str()), // name of the pipe
      PIPE_ACCESS_DUPLEX, // 1-way pipe -- send only
      PIPE_TYPE_BYTE, // send data as a byte stream
      1, // only allow 1 instance of this pipe
      0, // no outbound buffer
      0, // no inbound buffer
      0, // use default wait time
      NULL // use default security attributes
    );

    bool result = ConnectNamedPipe(Pipe, NULL);

    if(!result){
      std::cout << "FAILED TO CREATE PIPE!\n";
      system("pause");
      exit(1);
    }

  } else {
    Pipe = CreateFile(
        TEXT(path.c_str()),
        GENERIC_ALL, // only need read access
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (Pipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to connect to pipe." << std::endl;
        return false;
    }
  }
  return true;
}

// bool canConnect(std::string path){
//   HANDLE checkpipe = CreateFile(
//       TEXT(path.c_str()),
//       GENERIC_WRITE, // only need read access
//       FILE_SHARE_READ | FILE_SHARE_WRITE,
//       NULL,
//       OPEN_EXISTING,
//       FILE_ATTRIBUTE_NORMAL,
//       NULL
//   );
//   if (checkpipe == INVALID_HANDLE_VALUE) {
//     // CommitTransaction(checkpipe);
//     CloseHandle(checkpipe);
//     return false;
//   } else {
//     CloseHandle(checkpipe);
//     return true;
//   }
// }

bool pipe::sendMessage(std::string in){
  wchar_t* data = GetWC(in.c_str());
  DWORD length = 0;
  WriteFile(
      Pipe, // handle to our outbound pipe
      data, // data to send
      wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
      &length, // will store actual amount of data sent
      NULL // not using overlapped IO
  );

  if(length == 0){
    std::cout << "FAILED TO SEND DATA!\n";
    system("pause");
    exit(1);
  }


  return true;
}


std::string pipe::recieveMessage(){
  wchar_t buffer[128];
  DWORD numBytesRead = 0;
  bool result = ReadFile(
    Pipe,
    buffer, // the data from the pipe will be put here
    127 * sizeof(wchar_t), // number of bytes allocated
    &numBytesRead, // this will store number of bytes actually read
    NULL // not using overlapped IO
  );

  if(result){
    buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
    return GetString(buffer);
  }
  else{
    std::cout << "FAILED TO  RECIEVE MESSAGE!\n";
    return "";
  }
}

void pipe::close(){
  CloseHandle(Pipe);
}
