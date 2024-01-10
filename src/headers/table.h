//
// Created by Kacper Domżalski on 18/11/2023.
//

#pragma once

#include <vector>
#include <string>
#include "fmt/ranges.h"
#include "fmt/core.h"
#include <ranges>
#include <algorithm>
#include <fstream>
#include "fmt/std.h"
#include <regex>


enum class DataType {
    INT,
    STRING,
    FLOAT,
    INVALID
};

class Table {

    static auto typesFromUserInput(const std::vector<std::string> &tokens) -> std::vector<std::string>;

    static auto typesFromTable(const std::string &tablePath) -> std::vector<std::string>;

    static auto stringToDataType(const std::string &typeStr) -> DataType;

    static auto isValidTypeAndValue(const std::vector<std::string> &tokens) -> bool;

    static auto isNumberFloat(const std::string &str) -> bool;

    static auto isNumberInt(const std::string &str) -> bool;

    static auto isValidType(const std::string &typeStr) -> bool;

    static auto findPathToTable(const std::vector<std::string> &tokens) -> std::string;

public:
    static auto insertIntoMemoryTable(const std::vector<std::string> &tokens) -> void;

    static auto createMemoryTable(const std::vector<std::string> &tokens) -> void;

    static auto deleteTable(const std::vector<std::string> &tokens) -> void;
};
