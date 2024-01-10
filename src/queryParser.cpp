//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "headers/queryParser.h"
#include "headers/database.h"
#include "headers/instructions.h"
#include "headers/table.h"
#include "headers/select.h"
#include "headers/deleteRow.h"


auto QueryParser::splitCommands(const std::string &commands) -> std::vector<std::vector<std::string>> {
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
    return vectorOfCommands;
}


auto QueryParser::parseAndExecute(const std::string &query) -> void {
    auto commandToTokens = splitCommands(query);


    for (const auto &tokens: commandToTokens) {
        commandQueue.push(tokens);
//        fmt::println("QUEUE: {} ", commandQueue);

    }

    while (commandQueue.back()[0] == "EXECUTE" and commandQueue.back().size() == 1) {
        auto tokens = commandQueue.front();
        auto option = Instructions::getOptionFromTokens(tokens);
        commandQueue.pop();

        switch (option.value()) {
            case Option::CREATE_DATABASE:
                Database::createDatabase(tokens);
                break;
            case Option::DROP_DATABASE:
                Database::deleteDatabase(tokens);
                break;
            case Option::CREATE_TABLE:
                Table::createMemoryTable(tokens);
                break;
            case Option::DROP_TABLE:
                Table::deleteTable(tokens);
                break;
            case Option::INSERT_INTO:
                Table::insertIntoMemoryTable(tokens);
                break;
            case Option::UPDATE:
                Update::processUpdate(tokens);
                break;
            case Option::EXECUTE:
                fmt::println("Commands was executed");
                break;
            case Option::SELECT:
                Select::selectDataFromTable(tokens);
                break;
            case Option::DELETE:
                Delete::processDelete(tokens);
                break;

            default:
                fmt::println("Invalid or unimplemented command: '{}'", fmt::join(tokens, " "));
                break;
        }
    }
}


