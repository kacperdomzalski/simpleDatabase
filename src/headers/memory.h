//
// Created by Kacper Domżalski on 12/12/2023.
// https://refactoring.guru/pl/design-patterns/singleton/cpp/example

#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include "fmt/ranges.h"
#include "fmt/core.h"
#include <ranges>
#include <algorithm>
#include <fstream>
#include "fmt/std.h"
#include <set>
#include <unordered_set>


class Memory {
    Memory() = default;

    static std::unordered_set<std::string> modifiedTables;
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

    static auto
    createMemoryTable(const std::string &tableName, const std::vector<std::vector<std::string>> &data) -> void;

    static auto
    insertIntoTable(const std::string &tableName, const std::vector<std::string> &rowData) -> void;

    static auto saveTablesToFiles() -> void;

    static auto updateTable(const std::string &tableName, std::vector<std::vector<std::string>> &map) -> void;

    static auto tableExists(const std::string &tableName) -> bool;

    static auto deleteTable(const std::string &tableName) -> void;

    static auto deleteDatabase(const std::string &database) -> void;

    static auto removeRowFromTable(const std::string &tableName, const std::vector<int> &rowIndices) -> void;

};

// UPDATE test table2 WHERE imie = wojtas SET imie dupa;
// SELECT test table2 WHERE imie = dupa; EXECUTE

