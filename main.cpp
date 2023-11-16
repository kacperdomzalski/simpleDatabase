#include <iostream>

#include "queryParser.h"
#include "database.h"

auto main() -> int {

    std::string query;

    while (true) {
        std::cout << "db> "; // Prompt dla użytkownika
        if (!std::getline(std::cin, query) || query == "EXIT") {
            break; // Wyjście z pętli jeśli użytkownik wpisze EXIT
        } else {
            QueryParser::parseAndExecute(query);
        }


    }
}
