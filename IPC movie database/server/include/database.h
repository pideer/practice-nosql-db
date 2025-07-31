#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

#ifndef DATABASE_H
#define DATABASE_H

#define ANALYTIC_SEARCH_TIME false
#define ANALYTIC_FILELOAD_TIME false
#define ANALYTIC_SAVEFILE_TIME false

#define ESC "\033["
#define GREEN_TXT "0;32m"
#define RED_TXT "0;31m"
#define RESET "\033[m"

// TODO: Reference additional headers your program requires here.
class MovieDB {
  public:
    MovieDB();

    std::string addMovie(std::string info);
    std::string getMovie(std::string name);
    std::string listAll();

  private:
    std::string filename;
    nlohmann::json j;

    bool addJSONFile(std::string filename);
    bool saveFile();
    // nlohmann::json* movieExist(std::string name);
};

#endif // DATABASE_H