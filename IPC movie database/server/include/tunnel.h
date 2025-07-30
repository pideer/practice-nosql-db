#include <codecvt>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>
#endif

#ifndef TUNNEL_H
#define TUNNEL_H

class Tunnel {
  public:
    Tunnel();
    ~Tunnel();
    void send(std::string msg);
    std::vector<std::string> receive();

  private:
    void createPipe();
    void connect();
    bool disconnect();
#ifdef _WIN32
    HANDLE pipe;
#elif __linux__
    const char *pipe_path = "/tmp/my_pipe";
    int pipe;
#endif
};
#endif // TUNNEL_H