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
#include <queue>
#include "fmt/format.h"

struct QueryParser {
    static auto parseAndExecute(const std::string &query) -> void;

    static auto splitCommands(const std::string &commands);

private:
//    static std::queue<std::vector<std::string>> commandQueue;
    inline static auto commandQueue = std::queue<std::vector<std::string>>{};

};

