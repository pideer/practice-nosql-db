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
    return cmds;
}

TEST_CASE("getline usability", "[mini function]") {}

TEST_CASE("add movie parser", "[mini function]") {
    std::string info = "4 186 Lactose";

    int rating = info.at(0) - '0';
    CHECK(rating == 4);

    int i = 2;
    while (info.at(i) != ' ')
        i++;
    int length = stoi(info.substr(2, i));
    CHECK(length == 186);

    std::string name = info.substr(i + 1);
    CHECK(name == "Lactose");
}