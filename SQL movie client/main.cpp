// SQL movie client.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <pqxx/pqxx>
#include <string>

int main() {
    std::cout << "Hello SQL Movie Client" << std::endl;
    pqxx::connection cx{"dbname=MovieDB user=postgres password=password "
                        "hostaddr=127.0.0.1 port=5432"};

    if (cx.is_open()) {
        std::cout << "Connected to PostGres\n";
        pqxx::work tx{cx};

        // int length = tx.query_value<int>(
        //	"SELECT \"length\""
        //	"FROM \"MovieDB\""
        //	"WHERE name = \'Lactose\'");
        // std::cout << length;

        pqxx::result r = tx.exec(" SELECT \"length\", \"name\", \"rating\" "
                                 "FROM \"MovieDB\" WHERE name = 'Lactose'");
        for (auto row : r)
            std::cout << row["name"].as<std::string>() << " | "
                      << row["length"].as<int>() << " | "
                      << row["rating"].as<int>();

        tx.commit();
    }

    // pqxx::work tx{ cx };

    // auto name = tx.query_value<std::string>("SELECT name FROM MovieDB");
    // std::cout << name;

    return 0;
}
