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


struct Table {
    std::string name;
    std::string typeName;

    static auto processCreateTable(const std::vector<std::string> &tokens) -> void;

    static auto processInsert(const std::vector<std::string> &tokens) -> void;

};
