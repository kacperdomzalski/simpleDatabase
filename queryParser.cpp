//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "queryParser.h"
#include "database.h"
#include "instructions.h"

#include <sstream>
#include <vector>
#include <string>
#include <fmt/ranges.h>
#include <fmt/core.h>
#include <ranges>
#include <algorithm>


auto splitCommands(const std::string &commands) {
    auto commandLine = std::istringstream(commands);
    auto command = std::string();
    auto vectorOfCommands = std::vector<std::vector<std::string>>();

    while (std::getline(commandLine, command, ';')) {

        if (!command.empty()) {
            auto commandStream = std::istringstream(command);
            auto tokens = std::vector<std::string>{
                    std::istream_iterator<std::string>(commandStream), //iterates over the words in the stream
                    std::istream_iterator<std::string>()// end of stream iterator
            };

            vectorOfCommands.push_back(std::move(tokens)); // move tokens to vectorOfCommands
        }
    }
    fmt::println("{}", vectorOfCommands);
    return vectorOfCommands;
}


auto QueryParser::parseAndExecute(const std::string &query) -> void {
    auto commandToTokens = splitCommands(query);

    for (const auto &tokens: commandToTokens) {
        auto option = Instructions::getOptionFromTokens(tokens);

        if (option.has_value()) {
            switch (option.value()) {
                case Option::CREATE_DATABASE:
                    if (tokens.size() == 4) {
                        auto databaseName = tokens[2];
                        auto databasePath = tokens[3];
                        Database::createDatabase(databaseName, databasePath);
                    }
                    break;
                case Option::DELETE_DATABASE:
                    if (tokens.size() == 3) {
                        auto databaseName = tokens[2];
                        Database::deleteDatabase(databaseName);
                    }
                    break;
                default:
                    fmt::print("Invalid or unimplemented command {}\n", fmt::join(tokens, " "));
                    break;
            }
        } else {
            fmt::print("Invalid command {}\n", fmt::join(tokens, " "));
        }
    }

//    for (const auto &tokens: commandToTokens) {
//
//        if (tokens.size() == 4 and tokens[0] == "CREATE" and tokens[1] == "DATABASE") {
//            auto databaseName = tokens[2];
//            auto databasePath = tokens[3];
//
//            Database::createDatabase(databaseName, databasePath);
//
//
//        } else if (tokens.size() == 3 and tokens[0] == "DELETE" and tokens[1] == "DATABASE") {
//            auto databaseName = tokens[2];
//
//            Database::deleteDatabase(databaseName);
//
//        } else {
//            fmt::print("Invalid command {}\n", fmt::join(tokens, " "));
//        }
//    }
}




