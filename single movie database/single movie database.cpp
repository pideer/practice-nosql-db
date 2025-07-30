// single movie database.cpp : Defines the entry point for the application.
//

#include "single movie database.h"
#include <string>
#include <chrono>

//using namespace std;
using namespace std::chrono;
// delete
// list <action> <modifier> // example: rg 4 (rating greater 4)

#define ANALYTIC_SEARCH_TIME false
#define ANALYTIC_FILELOAD_TIME false
#define ANALYTIC_SAVEFILE_TIME false

#define ESC "\033["
#define GREEN_TXT "0;32m"
#define RED_TXT "0;31m"
#define RESET "\033[m"

int main() {
	MovieDB db;
	
	std::string cmd;
	while (cmd != "exit") {
		std::cout << "\nPlease select a command:\n" <<
			"(load <movie name>) start movie\n" <<
			"(add <movie name>) add movie\n" <<
			"(list) list all movies\n" <<
			" > ";
		std::getline(std::cin, cmd);

		if (cmd == "exit") break;
		else if (cmd == "list") db.listAll();
		else if (cmd.substr(0, 4) == "load") db.getMovie(cmd.substr(5));
		else if (cmd.substr(0, 3) == "add") db.addMovie(cmd.substr(4));
		else std::cout << ESC << RED_TXT << "invalid input\n" << RESET;
	}
	return 0;
}

MovieDB::MovieDB() {
	std::cout << "Welcome to the Movie Database\n";
	addJSONFile("mdb.json");
}

bool MovieDB::addMovie(std::string name) {

#if ANALYTIC_SEARCH_TIME	
	auto start = std::chrono::steady_clock::now(); 
#endif
	for (auto& it : j["titles"])
	if (it["name"] == name){
		std::cout << ESC << RED_TXT << "movie \"" << name << "\"already exist\n" << RESET;
#if ANALYTIC_SEARCH_TIME
		auto end = std::chrono::steady_clock::now();
		auto diff_ms = duration_cast<milliseconds>(end - start).count();
		std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms << RESET << '\n';
#endif
		return false;
	}
#if ANALYTIC_SEARCH_TIME
	auto end = std::chrono::steady_clock::now();
	auto diff_ms = duration_cast<milliseconds>(end - start).count();
	std::cout << ESC << GREEN_TXT << "addMovie Search Time: " << diff_ms << RESET << '\n';
#endif

	int length{ 0 }, rating{ 0 };
	std::string t;
	std::cout << "add \"" << name << "\"\n";

	do {
		std::cout << "enter movie length\n > ";
		std::getline(std::cin, t);

		try { length = std::stoi(t); }
		catch (...) { std::cout << ESC << RED_TXT << "invalid input\n"<< RESET; continue; }
		if (length < 0) return false;
		if (length == 0 || 600 < length)
			std::cout << ESC << RED_TXT << "invalid input\n"<< RESET;
	} while (length == 0);

	do {
		std::cout << "enter movie rating\n > ";
		std::getline(std::cin, t);

		try{ rating = std::stoi(t); }
		catch(...){std::cout << ESC << RED_TXT << "invalid input\n" << RESET; continue; }
		if (rating < 0) return false;
		if (rating == 0 || 5 < rating)
			std::cout << ESC << RED_TXT << "invalid input\n" << RESET;
	} while (rating == 0);

	j["titles"].push_back({ {"name", name}, { "length", length }, {"rating", rating} });
	this->saveFile();


	return true;
}

void MovieDB::getMovie(std::string name) {
#if ANALYTIC_SEARCH_TIME	
	auto start = std::chrono::steady_clock::now();
#endif
	for (auto& movie : j["titles"]) {
		if (movie["name"] == name) {
			std::cout << name << '\n' <<
				"Length: " << movie["length"] << '\n' <<
				"Rating: " << movie["rating"] << '\n';
#if ANALYTIC_SEARCH_TIME
			auto end = std::chrono::steady_clock::now();
			auto diff_ms = duration_cast<milliseconds>(end - start).count();
			std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms << RESET << '\n';
#endif
			return;
		}
	}
#if ANALYTIC_SEARCH_TIME
	auto end = std::chrono::steady_clock::now();
	auto diff_ms = duration_cast<milliseconds>(end - start).count();
	std::cout << ESC << GREEN_TXT << "getMovie Search Time: " << diff_ms << RESET << '\n';
#endif
	std::cout << ESC << RED_TXT << "Movie does not exist in database\n" << RESET; 
}

void MovieDB::listAll() {
#if ANALYTIC_SEARCH_TIME	
	auto start = std::chrono::steady_clock::now();
#endif
	if (j["titles"].empty()) {
		std::cout << ESC << RED_TXT << "No Movies in database\n" << RESET; 
		return;
	}
	for (auto it = j["titles"].begin(); it != j["titles"].end(); it++) {
		std::cout << (*it)["name"] << '\n';
	}
#if ANALYTIC_SEARCH_TIME
	auto end = std::chrono::steady_clock::now();
	auto diff_ms = duration_cast<milliseconds>(end - start).count();
	std::cout << ESC << GREEN_TXT << "listALl Search Time: " << diff_ms << RESET << '\n';
#endif
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
		std::cout << ESC << GREEN_TXT << "loadFile Time: " << diff_ms << RESET << '\n';
#endif
	} else {
		std::cout << ESC << RED_TXT << "no database file. starting new database\n" << RESET;
		//j["titles"] = nlohmann::json::object();
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
		std::cout << ESC << GREEN_TXT << "saveFile Time: " << diff_ms << RESET << '\n';
#endif
	} else {
		std::cout << ESC << RED_TXT << "Failed to save to db file\n" << RESET;
		return false;
	}
	return true;
}
