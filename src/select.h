//
// Created by Kacper Domżalski on 05/12/2023.
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
#include <map>
#include "memory.h"


class Select {

public:
    static auto selectDataFromTable(const std::vector<std::string> &tokens) -> void;

    static auto findColumnsFromUser(const std::vector<std::string> &tokens) -> std::vector<std::string>;

    static auto findPathToTable(const std::vector<std::string> &tokens) -> std::string;

    static auto findIndexesOfColumns(const std::vector<std::string> &tokens) -> std::vector<int>;

    static auto findDataForColumnsIndexes(const std::vector<std::string> &tokens,
                                          const std::vector<int> &indexes) -> std::vector<std::vector<std::string>>;

    static auto printData(const std::vector<std::string> &tokens) -> void;

    static auto getDataFromLine(const std::string &line, const std::vector<int> &indexes) -> std::vector<std::string>;


};



