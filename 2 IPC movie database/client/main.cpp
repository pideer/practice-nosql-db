// main.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <string>
#include "tunnel.h"

#define ESC "\033["
#define GREEN_TXT "0;32m"
#define RED_TXT "0;31m"
#define RESET "\033[m"

int main() {
	std::cout << "Hello, from IPC-json_test-client!\n";

	Tunnel tunnel;

	std::string cmd;
	while (true) {
		std::cout << "\nPlease select a command:\n" <<
			"(load <movie name>) start movie\n" <<
			"(add <movie name>) add movie\n" <<
			"(list) list all movies\n" <<
			" > ";
		std::getline(std::cin, cmd);

		if (cmd.substr(0, 3) == "add") {
			std::string name = cmd.substr(4);
			int length{ 0 }, rating{ 0 };
			std::string t;
			std::cout << "add \"" << name << "\"\n";

			do {
				std::cout << "enter movie length\n > ";
				std::getline(std::cin, t);

				try { length = std::stoi(t); } catch (...) { std::cout << ESC << RED_TXT << "invalid input\n" << RESET; continue; }
				if (length < 0) return false;
				if (length == 0 || 600 < length)
					std::cout << ESC << RED_TXT << "invalid input\n" << RESET;
			} while (length == 0);

			do {
				std::cout << "enter movie rating\n > ";
				std::getline(std::cin, t);

				try { rating = std::stoi(t); } catch (...) { std::cout << ESC << RED_TXT << "invalid input\n" << RESET; continue; }
				if (rating < 0) return false;
				if (rating == 0 || 5 < rating)
					std::cout << ESC << RED_TXT << "invalid input\n" << RESET;
			} while (rating == 0);

			tunnel.send("add " + std::to_string(rating) + ' ' + std::to_string(length) + ' ' + name);
		} else {
			tunnel.send(cmd);
			std::cout << "sent\n";
		}
		if (cmd == "exit" || cmd == "close") break;
		std::cout << tunnel.receive();
	}

	return 0;
}
