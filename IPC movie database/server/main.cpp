#include <iostream>
#include <tunnel.h>

int main(int, char **) {
    std::cout << "Hello, from IPC-json_test-server!\n";

    Tunnel tunnel;
    std::vector<std::string> cmds;
    while (true) {
        cmds = tunnel.receive();
        if (!cmds.empty()) tunnel.send(cmds.at(0));
        /*for (auto& cmd : cmds)
            std::cout << cmd << " ";*/
    }
}
