// SQL movie client.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <pqxx/pqxx>
#include <string>

#define ESC "\033["
#define GREEN_TXT "\033[0;32m"
#define RED_TXT "\033[0;31m"
#define RESET "\033[m"

void listAll(pqxx::work& tx);
void getMovie(pqxx::work &tx, std::string name);
void addMovie(pqxx::work &tx, std::string name);
std::string validInput(std::string& s);

int main() {
    std::cout << "Hello SQL Movie Client" << std::endl;
    
    try {
        pqxx::connection cx("dbname=MovieDB user=postgres password=passwor "
            "hostaddr=127.0.0.1 port=5432");
   

    if (cx.is_open()) {
        std::cout << "Connected to PostGres\n";
        pqxx::work tx{ cx };

        std::string cmd;
        while (cmd != "exit") {
            std::cout << "\nPlease select a command:\n" <<
                "(load <movie name>) start movie\n" <<
                "(add <movie name>) add movie\n" <<
                "(list) list all movies\n" <<
                " > ";
            std::getline(std::cin, cmd);

            if (cmd == "exit") break;
            else if (cmd == "list") listAll(tx);
            else if (cmd.substr(0, 4) == "load") getMovie(tx, cmd.substr(5));
            else if (cmd.substr(0, 3) == "add") addMovie(tx, cmd.substr(4));
            else std::cout << RED_TXT << "invalid input\n" << RESET;
        }
        tx.commit();
    }
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "failed to connect to database";
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}

void listAll(pqxx::work& tx) {
    pqxx::result r = tx.exec("SELECT \"name\" "
        "FROM \"MovieDB\"");
    for (auto row : r)
        std::cout << row["name"].as<std::string>() << "\n";
}

void getMovie(pqxx::work& tx, std::string name) {
    std::string qname = tx.quote(validInput(name));
    try {
        pqxx::row r = tx.exec1(" SELECT \"length\", \"name\", \"rating\" "
            "FROM \"MovieDB\" WHERE name = " + qname);

        std::cout << r["name"].as<std::string>() << "\n" <<
            "Length: " << r["length"].as<int>() << "\n" <<
            "Rating: " << r["rating"].as<int>() << "\n";
            
    } catch (const pqxx::unexpected_rows& e) {
        std::cerr << "Movie does not exist in the database\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void addMovie(pqxx::work& tx, std::string name) {
    std::string vname = validInput(name);
    std::string qname = tx.quote(vname);

    auto exists = tx.query_value<bool>(
        "SELECT EXISTS(SELECT 1 FROM \"MovieDB\" WHERE name = " +
        qname + ")");

    if (exists) {
        std::cout << "Movie already exists\n";
        return;
    }

    int length{ 0 }, rating{ 0 };
    std::string t;
    std::cout << "add \"" << vname << "\"\n";

    do {
        std::cout << "enter movie length\n > ";
        std::getline(std::cin, t);

        try { length = std::stoi(t); } catch (...) { std::cout <<  RED_TXT << "invalid input\n" << RESET; continue; }
        if (length <= 1 || 600 < length)
            std::cout <<  RED_TXT << "invalid input\n" << RESET;
    } while (length == 0);

    do {
        std::cout << "enter movie rating\n > ";
        std::getline(std::cin, t);

        try { rating = std::stoi(t); } catch (...) { std::cout <<  RED_TXT << "invalid input\n" << RESET; continue; }
        if (rating <= 0 || 5 < rating)
            std::cout << RED_TXT << "invalid input\n" << RESET;
    } while (rating == 0);

    try {
        std::string val = "INSERT INTO \"MovieDB\" (name, length, rating) " 
            "VALUES (" + qname + "," + tx.quote(length) + "," + tx.quote(rating) + ")";
        pqxx::result r = tx.exec(val);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

std::string validInput(std::string& s) {
    std::string r;

    for (auto ch : s)
        if (ch == '\'' || ch == '\"')
            r.append(2, ch);
        else
            r += ch;
    return r;
}
