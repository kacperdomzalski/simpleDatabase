//
// Created by Kacper Domżalski on 19/12/2023.
//

#pragma once

#include <sstream>
#include <vector>
#include <string>
#include "fmt/ranges.h"
#include "fmt/core.h"
#include <ranges>
#include <algorithm>
#include <fstream>
#include "fmt/std.h"
#include "memory.h"
#include "config.h"
#include "updateSetFilter.h"
#include "whereFilter.h"
#include <regex>

struct Update {

    static auto processUpdate(const std::vector<std::string> &tokens) -> void;

    static auto findPathToTable(const std::vector<std::string> &tokens) -> std::string;

    static auto parseTokensAfterSet(const std::vector<std::string> &tokens,
                                    const std::vector<std::vector<std::string>> map) -> std::vector<UpdateSetFilter>;


    static auto findColumnIndex(const std::string &columnName, const std::vector<std::vector<std::string>> &map) -> int;

    static auto parseTokensAfterWhereToSet(const std::vector<std::string> &tokens,
                                           const std::vector<std::vector<std::string>> &map) -> std::vector<Filter>;

    static auto findConditionType(const std::string &condition) -> Condition;

    static auto checkCondition(const std::string &value1, const std::string &value2, Condition condition) -> void;

    static auto findRowIndexes(const std::vector<std::vector<std::string>> &map,
                               const std::vector<Filter> &filters) -> std::vector<int>;

    static auto isColumnTypeIntOrFloat(const std::vector<std::vector<std::string>> &map, int columnIndex) -> bool;

    static auto determineColumnType(const std::vector<std::vector<std::string>> &map, int columnIndex) -> std::string;

    static auto isFloatNumber(const std::string &str) -> bool;

    static auto isIntNumber(const std::string &str) -> bool;
};


