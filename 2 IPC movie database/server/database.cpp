#include "database.h"
#include <string>


// using namespace std;
#if ANALYTIC_SEARCH_TIME || ANALYTIC_FILELOAD_TIME || ANALYTIC_SAVEFILE_TIME
#include <chrono>
using namespace std::chrono;
#endif
// delete
// list <action> <modifier> // example: rg 4 (rating greater 4)

MovieDB::MovieDB() {
    std::cout << "Welcome to the Movie Database\n";
    addJSONFile("mdb.json");
}

std::string MovieDB::addMovie(std::string info) {
    std::string reply;

    int rating = info.at(0) - '0';
    int i = 2;
    while (info.at(i) != ' ')
        i++;
    int length = stoi(info.substr(2, i));
    std::string name = info.substr(i + 1);

#if ANALYTIC_SEARCH_TIME
    auto start = std::chrono::steady_clock::now();
#endif
    for (auto &it : j["titles"])
        if (it["name"] == name) {
            reply = ESC RED_TXT "movie \"" + name + "\"already exist\n" RESET;
#if ANALYTIC_SEARCH_TIME
            auto end = std::chrono::steady_clock::now();
            auto diff_ms = duration_cast<milliseconds>(end - start).count();
            std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms
                      << RESET << '\n';
#endif
            return reply;
        }
#if ANALYTIC_SEARCH_TIME
    auto end = std::chrono::steady_clock::now();
    auto diff_ms = duration_cast<milliseconds>(end - start).count();
    std::cout << ESC << GREEN_TXT << "addMovie Search Time: " << diff_ms
              << RESET << '\n';
#endif
    j["titles"].push_back(
        {{"name", name}, {"length", length}, {"rating", rating}});
    this->saveFile();

    return "Added " + name + ", " + std::to_string(length) +
           "min with rating " + std::to_string(rating);
}

std::string MovieDB::getMovie(std::string name) {
    std::string reply;

#if ANALYTIC_SEARCH_TIME
    auto start = std::chrono::steady_clock::now();
#endif
    for (auto &movie : j["titles"]) {
        if (movie["name"] == name) {
            reply = name + "\n" + "Length: " + movie["length"].dump() + "\n" +
                    "Rating: " + movie["rating"].dump() + "\n";
#if ANALYTIC_SEARCH_TIME
            auto end = std::chrono::steady_clock::now();
            auto diff_ms = duration_cast<milliseconds>(end - start).count();
            std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms
                      << RESET << '\n';
#endif
            return reply;
        }
    }
#if ANALYTIC_SEARCH_TIME
    auto end = std::chrono::steady_clock::now();
    auto diff_ms = duration_cast<milliseconds>(end - start).count();
    std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms
              << RESET << '\n';
#endif
    return ESC RED_TXT "Movie does not exist in database\n" RESET;
}

std::string MovieDB::listAll() {
    if (j["titles"].empty()) return ESC RED_TXT "No Movies in database\n" RESET;

    std::string reply = "";

#if ANALYTIC_SEARCH_TIME
    auto start = std::chrono::steady_clock::now();
#endif
    for (auto it = j["titles"].begin(); it != j["titles"].end(); it++)
        reply += std::string((*it)["name"]) + "\n";
#if ANALYTIC_SEARCH_TIME
    auto end = std::chrono::steady_clock::now();
    auto diff_ms = duration_cast<milliseconds>(end - start).count();
    std::cout << ESC << GREEN_TXT << "listALl Search Time: " << diff_ms << RESET
              << '\n';
#endif
    return reply;
}

bool MovieDB::addJSONFile(std::string filename) {
    this->filename = filename;
    std::ifstream f(filename);

    if (f.is_open()) {
#if ANALYTIC_FILELOAD_TIME
        auto start = std::chrono::steady_clock::now();
#endif
        f >> j;
        f.close();
#if ANALYTIC_FILELOAD_TIME
        std::cout << "render ";
        auto end = std::chrono::steady_clock::now();
        auto diff_ms = duration_cast<milliseconds>(end - start).count();
        std::cout << ESC << GREEN_TXT << "loadFile Time: " << diff_ms << RESET
                  << '\n';
#endif
    } else {
        std::cout << ESC << RED_TXT
                  << "no database file. starting new database\n"
                  << RESET;
        // j["titles"] = nlohmann::json::object();
    }

    return false;
}

bool MovieDB::saveFile() {
    std::ofstream f(filename);
    if (f.is_open()) {
#if ANALYTIC_SAVEFILE_TIME
        auto start = std::chrono::steady_clock::now();
#endif
        f << j;
        f.close();
#if ANALYTIC_SAVEFILE_TIME
        std::cout << "render ";
        auto end = std::chrono::steady_clock::now();
        auto diff_ms = duration_cast<milliseconds>(end - start).count();
        std::cout << ESC << GREEN_TXT << "saveFile Time: " << diff_ms << RESET
                  << '\n';
#endif
    } else {
        std::cout << ESC << RED_TXT << "Failed to save to db file\n" << RESET;
        return false;
    }
    return true;
}
