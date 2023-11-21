//
// Created by Kacper Domżalski on 13/11/2023.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <string>
#include "fmt/ranges.h"
#include "fmt/core.h"
#include <algorithm>

struct QueryParser {
    static auto splitCommands(const std::string &commands);

    static auto parseAndExecute(const std::string &query) -> void;
};

