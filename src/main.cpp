#include <iostream>

#include "headers/queryParser.h"
#include "headers/memory.h"

// ZOBACZYC CO SIE STANIE PRZY USUNIECIU BAZY DANYCH A NASTEPNIE STWORZENIU JEJ PONOWNIE
// ZONACZYC CO SIE STANIE PRZY USUNIECIE BAZY DANYCH A NASTEPNIE WYSZUKANIU DANYCH Z TABELI Z TEJ BAZY DANYCH
// NAPRAWIC BLAD, GDZIE JESLI NIE MA JAKIEJS TABELI TO PROGRAM SIE WYLACZA PRZY SELECT
// ZMIENIC DELETE TABLE I DELETE DATABASE NA DROP TABLE I DROP DATABASE
// libc++abi: terminating due to uncaught exception of type std::bad_optional_access: bad_optional_access <- wyjebac albo naprawic std::optional
//ZAIMPLEMENTOWAC HELP
// DODAC FORMATOWANIE DANYCH W TABELI <15
// DODAC MOZLIWOSC INSERTOWANIA FLOATA TYLKO Z 2 miejscami po przecinku maksymalnie PRZY INSERTOWANIU
// OGARNAC IMPORTY
// DODAC PRZY STD RANGES FIND SPRAWDZANIE CZY FAKTYCZNIE ZWRACA ISTENIEACY ELEMENT (WYKLAD  3.1)



// NAPISAĆ WSZYSTKEI POLECENIA
auto signalHandler(int signum) -> void;

auto main() -> int {
    signal(SIGTERM, signalHandler);
    auto query = std::string();

    fmt::println(
            "=====================================================================\n"
            "Welcome to dB! \n"
            "Type 'HELP' to see all available commands. \n"
            "Use 'EXECUTE' to execute your queries or 'EXIT' to exit the program. \n"
            "=====================================================================\n"
    );


    while (true) {
        fmt::print("dB> ");
        if (!std::getline(std::cin, query) || query == "EXIT") {
            fmt::println("Exiting...");
            break;
        } else {
            QueryParser::parseAndExecute(query);
        }
    }

    fmt::println("Saving tables to files...");
    Memory::saveTablesToFiles();
    fmt::println("Tables saved successfully!");


}

auto signalHandler(int signum) -> void {
    fmt::println("Interrupt signal ({}) received.", signum);

    Memory::saveTablesToFiles();

    exit(signum);
}