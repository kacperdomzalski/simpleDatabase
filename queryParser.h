//
// Created by Kacper Domżalski on 13/11/2023.
//

#pragma once

#include <string>

class QueryParser {
public:
    QueryParser();

//    static auto splitCommands(const std::string &commands);

    static auto parseAndExecute(const std::string &query) -> void;
};

