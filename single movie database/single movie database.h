// single movie database.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "json.hpp"
#include <fstream>

// TODO: Reference additional headers your program requires here.
class MovieDB {
public:
	MovieDB();

	bool addMovie(std::string name);
	void getMovie(std::string name);
	void listAll();

private:
	std::string filename;
	nlohmann::json j;

	bool addJSONFile(std::string filename);
	bool saveFile();
	//nlohmann::json* movieExist(std::string name);

};