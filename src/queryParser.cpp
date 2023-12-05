//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "queryParser.h"
#include "database.h"
#include "instructions.h"
#include "table.h"


auto QueryParser::splitCommands(const std::string &commands) {
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

            vectorOfCommands.push_back(tokens); // move tokens to vectorOfCommands

        }

    }
    fmt::println("Command VEC: {} ", vectorOfCommands);
    return vectorOfCommands;
}


auto QueryParser::parseAndExecute(const std::string &query) -> void {
    auto commandToTokens = splitCommands(query);


    for (const auto &tokens: commandToTokens) {
        commandQueue.push(tokens);
        fmt::println("QUEUE: {} ", commandQueue);

    }

    while (commandQueue.back()[0] == "EXECUTE" and commandQueue.back().size() == 1) {
        auto tokens = commandQueue.front();
        auto option = Instructions::getOptionFromTokens(tokens);
        commandQueue.pop();

        switch (option.value()) {
            case Option::CREATE_DATABASE:
                if (tokens.size() == 3) {
                    auto databaseName = tokens[2];
                    Database::createDatabase(databaseName);
                }
                break;
            case Option::DELETE_DATABASE:
                if (tokens.size() == 3) {
                    auto databasePath = tokens[2];
                    Database::deleteDatabase(databasePath);
                }
                break;
            case Option::CREATE_TABLE:
                Database::createTable(tokens);
                break;
            case Option::INSERT:
                Database::insertInto(tokens);
                break;
            case Option::EXECUTE:
                fmt::println("Executing commands");
                break;

            default:
                fmt::print("Invalid or unimplemented command {}\n", fmt::join(tokens, " "));
                break;
        }
    }
}


