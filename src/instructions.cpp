//
// Created by Kacper Domżalski on 16/11/2023.
//

#include <string>
#include "instructions.h"
#include "fmt/core.h"


std::optional<Option> Instructions::getOptionFromTokens(const std::vector<std::string> &tokens) {

    std::string command;
    auto it = tokens.begin();
    auto &firstCommand = *it;
    auto &secondCommand = *(it + 1);


    // Compare the command string to known commands
    if (firstCommand == "CREATE" and secondCommand == "DATABASE") {
        return Option::CREATE_DATABASE;
    } else if (firstCommand == "DELETE" and secondCommand == "DATABASE") {
        return Option::DELETE_DATABASE;
    } else if (firstCommand == "CREATE" and secondCommand == "TABLE") {
        return Option::CREATE_TABLE;
    } else if (firstCommand == "DELETE" and secondCommand == "TABLE") {
        return Option::DELETE_TABLE;
    } else if (firstCommand == "INSERT" and secondCommand == "INTO") {
        return Option::INSERT;
    } else if (firstCommand == "SELECT") {
        return Option::SELECT;
    } else if (command == "update") {
        return Option::UPDATE;
    } else if (command == "delete") {
        return Option::DELETE;
    } else if (command == "exit") {
        return Option::EXIT;
    } else if (*it == "EXECUTE") {
        return Option::EXECUTE;
        // If no valid command is matched, return an invalid option

    }

    return std::nullopt;
}