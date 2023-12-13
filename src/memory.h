//
// Created by Kacper Domżalski on 12/12/2023.
// https://refactoring.guru/pl/design-patterns/singleton/cpp/example

#pragma once

#include <string>
#include <vector>
#include <map>
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


class Memory {
    Memory() = default;

    std::map<std::string, std::vector<std::vector<std::string>>> tableMap;

    static Memory *instance;

public:
    Memory(Memory const &) = delete;

    void operator=(Memory const &) = delete;

    static Memory &getInstance() {
        if (instance == nullptr) {
            instance = new Memory();
        }
        return *instance;
    }
    static auto getTableData(const std::string &tableName) -> std::vector<std::vector<std::string>>;
};




