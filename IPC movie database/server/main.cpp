#include "database.h"
#include <iostream>
#include <tunnel.h>

int main(int, char **) {
    std::cout << "Hello, from IPC-json_test-server!\n";

    Tunnel tunnel;
    MovieDB db;
    std::vector<std::string> cmds;

    while (true) {
        cmds = tunnel.receive();
        std::string reply;

        if (!cmds.empty()) {
            if (cmds.at(0) == "close")
                break;
            else if (cmds.at(0) == "list")
                reply = db.listAll();
            else if (cmds.at(0) == "load")
                reply = db.getMovie(cmds.at(1));
            else if (cmds.at(0) == "add")
                reply = db.addMovie(cmds.at(1));
            else
                reply = ESC RED_TXT "invalid input\n" RESET;

            tunnel.send(reply);
        } else
            tunnel.send("");
    }
}
