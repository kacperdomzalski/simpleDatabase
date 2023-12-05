//
// Created by Kacper Domżalski on 16/11/2023.
//

#include <string>
#include "instructions.h"
#include "fmt/core.h"


std::optional<Option> Instructions::getOptionFromTokens(const std::vector<std::string> &tokens) {

    std::string command;
    auto it = tokens.begin();
    // Compare the command string to known commands
    if (*it == "CREATE" and *(it + 1) == "DATABASE") {
        return Option::CREATE_DATABASE;
    } else if (*it == "DELETE" and *(it + 1) == "DATABASE") {
        return Option::DELETE_DATABASE;
    } else if (*it == "CREATE" and *(it + 1) == "TABLE") {
        return Option::CREATE_TABLE;
    } else if (command == "delete table") {
        return Option::DELETE_TABLE;
    } else if (*it == "INSERT" and *(it + 1) == "INTO") {
        return Option::INSERT;
    } else if (command == "select") {
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
        return std::nullopt;
    }


}