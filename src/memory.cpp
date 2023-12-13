//
// Created by Kacper Domżalski on 12/12/2023.
//

#include "memory.h"

Memory *Memory::instance = nullptr;

auto readLinesFromFile(const std::string &tableName) {

    auto lines = std::vector<std::vector<std::string>>();
    auto file = std::fstream(tableName);
    auto line = std::string();


    while (std::getline(file, line)) {
        auto iss = std::istringstream(line);
        auto words = std::vector<std::string>();
        auto word = std::string();

        while (iss >> word) {
            words.push_back(word);
        }

        lines.push_back(words);
    }

    return lines;
}


auto Memory::getTableData(const std::string &tableName) -> std::vector<std::vector<std::string>> {
    Memory &memory = Memory::getInstance();

    if (memory.tableMap.contains(tableName)) {
        fmt::println("Mapa o tym kluczu juz istnieje {} ", memory.tableMap.contains(tableName));
        return memory.tableMap[tableName];
    } else {
        auto tableVector = std::vector<std::vector<std::string>>();
        auto lines = readLinesFromFile(tableName);

        for (const auto &line: lines) {
            tableVector.push_back(line);
        }


        memory.tableMap[tableName] = tableVector;
    }

    fmt::println("PrintTableMap: {}", memory.tableMap);
    return memory.tableMap[tableName];
}