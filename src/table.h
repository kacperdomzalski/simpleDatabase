//
// Created by Kacper Domżalski on 18/11/2023.
//

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "fmt/ranges.h"
#include "fmt/core.h"
#include <ranges>
#include <algorithm>
#include <fstream>
#include "fmt/std.h"

enum class DataType {
    INT,
    STRING,
    FLOAT,
    INVALID
};

struct Table {
//    std::string name;
//    std::string typeName;

    static auto processCreateTable(const std::vector<std::string> &tokens) -> void;

    static auto processInsert(const std::vector<std::string> &tokens) -> void;

    static auto typesFromUserInput(const std::vector<std::string> &tokens) -> std::vector<std::string>;

    static auto typesFromTable(const std::vector<std::string> &tokens) -> std::vector<std::string>;

    static auto stringToDataType(const std::string &typeStr) -> DataType;

    static auto isValidTypeAndValue(const std::vector<std::string> &tokens) -> bool;
};
