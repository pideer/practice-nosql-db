#include "catch2/catch_test_macros.hpp"
#include <tunnel.h>

std::vector<std::string> parser(std::string cmd) {
    std::string s;
    std::istringstream ss(cmd);
    std::vector<std::string> cmds;

    if (ss >> s) {
        cmds.push_back(s);
        s.clear();

        std::getline(ss, s);
        if (!s.empty()) {
            if (s[0] == ' ') s.erase(0, 1);
            cmds.push_back(s);
        }
    }
}

TEST_CASE("getline usability", "[mini function]") {
    

    
}