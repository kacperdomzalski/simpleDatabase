//
// Created by Kacper Domżalski on 16/11/2023.
//

#pragma once

#include <map>

enum class Option {
    CREATE_DATABASE,
    DROP_DATABASE,
    CREATE_TABLE,
    DROP_TABLE,
    INSERT_INTO,
    SELECT,
    UPDATE,
    DELETE,
    EXIT,
    EXECUTE,
    INVALID
};


class Instructions {
public:
    static auto getOptionFromTokens(const std::vector<std::string> &tokens) -> std::optional<Option>;


    Instructions() = default;

};


