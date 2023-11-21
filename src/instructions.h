//
// Created by Kacper Domżalski on 16/11/2023.
//

#pragma once

#include <map>

enum class Option {
    CREATE_DATABASE,
    DELETE_DATABASE,
    CREATE_TABLE,
    DELETE_TABLE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
    EXIT
};


class Instructions {
public:
    static std::optional<Option> getOptionFromTokens(const std::vector<std::string> &tokens);


    Instructions() = default;

};


