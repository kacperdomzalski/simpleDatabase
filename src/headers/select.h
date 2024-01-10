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
#include <algorithm>
#include <iterator>
#include "whereFilter.h"
#include <regex>
#include "config.h"


class Select {

    static auto findColumnsFromUser(const std::vector<std::string> &tokens,
                                    const std::vector<std::vector<std::string>> &map) -> std::vector<std::string>;

    static auto findPathToTable(const std::vector<std::string> &tokens) -> std::string;

    static auto printDataFromMapWithConditions(const std::vector<Filter> &filters,
                                               const std::vector<std::string> &columnsToTake,
                                               const std::vector<std::vector<std::string>> &map) -> void;

    static auto filterWhereQuery(const std::vector<std::string> &tokens,
                                 const std::vector<std::vector<std::string>> &map) -> std::vector<Filter>;

    static auto findConditionType(const std::string &condition) -> Condition;

    static auto findColumnIndex(const std::string &columnName, const std::vector<std::vector<std::string>> &map) -> int;

    static auto findRowIndexes(const std::vector<std::vector<std::string>> &map,
                               const std::vector<Filter> &filters) -> std::vector<int>;

    static auto isColumnTypeIntOrFloat(const std::vector<std::vector<std::string>> &map, int columnIndex) -> bool;

    static auto isFloatNumber(const std::string &str) -> bool;

    static auto printTableHeaders(const std::vector<int> &columnIndexes,
                                  const std::vector<std::vector<std::string>> &map) -> void;


public:
    static auto selectDataFromTable(const std::vector<std::string> &tokens) -> void;
};


