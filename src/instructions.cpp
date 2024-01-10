//
// Created by Kacper Domżalski on 16/11/2023.
//

#include <string>
#include "headers/instructions.h"
#include "fmt/core.h"


auto Instructions::getOptionFromTokens(const std::vector<std::string> &tokens) -> std::optional<Option> {

    auto command = std::string();
    auto it = tokens.begin();
    auto &firstCommand = *it;
    auto &secondCommand = *(it + 1);


    // Compare the command string to known commands
    if (firstCommand == "CREATE" and secondCommand == "DATABASE") {
        return Option::CREATE_DATABASE;
    } else if (firstCommand == "DROP" and secondCommand == "DATABASE") {
        return Option::DROP_DATABASE;
    } else if (firstCommand == "CREATE" and secondCommand == "TABLE") {
        return Option::CREATE_TABLE;
    } else if (firstCommand == "DROP" and secondCommand == "TABLE") {
        return Option::DROP_TABLE;
    } else if (firstCommand == "INSERT" and secondCommand == "INTO") {
        return Option::INSERT_INTO;
    } else if (firstCommand == "SELECT") {
        return Option::SELECT;
    } else if (firstCommand == "UPDATE") {
        return Option::UPDATE;
    } else if (firstCommand == "DELETE" and secondCommand == "FROM") {
        return Option::DELETE;
    } else if (command == "exit") {
        return Option::EXIT;
    } else if (*it == "EXECUTE") {
        return Option::EXECUTE;
    }

    return Option::INVALID;
}