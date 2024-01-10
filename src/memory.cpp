//
// Created by Kacper Domżalski on 12/12/2023.
//

// SELECT imie FROM test table2;
// SELECT imie FTOM test table3;
// UPDATE test table2 WHERE imie = Kacper2 SET imie kacper; EXECUTE


#include "headers/memory.h"

std::unordered_set<std::string> Memory::modifiedTables;

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
//        fmt::println("Mapa o tym kluczu juz istnieje {} ", memory.tableMap.contains(tableName));
        return memory.tableMap[tableName];
    } else {
        auto tableVector = std::vector<std::vector<std::string>>();
        auto lines = readLinesFromFile(tableName);

        for (const auto &line: lines) {
            tableVector.push_back(line);
        }


        memory.tableMap[tableName] = tableVector;
    }

//    fmt::println("PrintTableMap: {}", memory.tableMap);
    return memory.tableMap[tableName];
}

auto
Memory::createMemoryTable(const std::string &tableName, const std::vector<std::vector<std::string>> &data) -> void {
    Memory &memory = Memory::getInstance();
    memory.tableMap[tableName] = data;
    modifiedTables.insert(tableName);
//    fmt::println("PrintTableMap: {}", memory.tableMap);

}

auto Memory::updateTable(const std::string &tableName, std::vector<std::vector<std::string>> &map) -> void {
    Memory &memory = Memory::getInstance();
    memory.tableMap[tableName].swap(map);
    modifiedTables.insert(tableName);
}

auto
Memory::insertIntoTable(const std::string &tableName, const std::vector<std::string> &rowData) -> void {
    Memory &memory = Memory::getInstance();

    if (memory.tableMap.find(tableName) != memory.tableMap.end()) {
        memory.tableMap[tableName].push_back(rowData);
        modifiedTables.insert(tableName);
        fmt::println("Data added to map");
    } else {
        fmt::println("Such map does not exist");
    }

//    fmt::println("PrintTableMap: {}", memory.tableMap);

}

auto Memory::deleteTable(const std::string &tableName) -> void {
    Memory &memory = Memory::getInstance();
    memory.tableMap.erase(tableName);
    modifiedTables.erase(tableName);
//    fmt::println("PrintTableMap: {}", memory.tableMap);
//    fmt::println("tableSet: {}", modifiedTables);


}


auto Memory::saveTablesToFiles() -> void {
    Memory &memory = Memory::getInstance();

//    if (modifiedTables.empty()) {
//        modifiedTables.clear();
//        return;
//    }


    for (const auto &tableName: modifiedTables) {
        auto it = memory.tableMap.find(tableName);
        if (it != memory.tableMap.end()) {
            std::ofstream outFile(tableName);
            if (!outFile.is_open()) {
                fmt::print("Error: Unable to open file '{}' for writing.\n", tableName);
                continue;
            }

            for (const auto &row: it->second) {
                for (const auto &item: row) {
                    outFile << item << " ";
                }
                outFile << "\n";
            }
        }
    }
    modifiedTables.clear(); // Wyczyść zbiór zmodyfikowanych tabel
}

auto Memory::tableExists(const std::string &tableName) -> bool {
    Memory &memory = Memory::getInstance();
    return memory.tableMap.find(tableName) != memory.tableMap.end();
}

auto Memory::deleteDatabase(const std::string &database) -> void {
    Memory &memory = Memory::getInstance();

    memory.tableMap.erase(database);


    for (auto it = memory.tableMap.begin(); it != memory.tableMap.end();) {
        if (it->first.find(database) != std::string::npos) {
            it = memory.tableMap.erase(it);
        } else {
            ++it;
        }
    }

}


auto Memory::removeRowFromTable(const std::string &tableName, const std::vector<int> &rowIndices) -> void {
    Memory &memory = Memory::getInstance();

    for (const auto &row: rowIndices) {
        if (memory.tableMap.find(tableName) != memory.tableMap.end()) {
            memory.tableMap[tableName].erase(memory.tableMap[tableName].begin() + row);
            modifiedTables.insert(tableName);
            fmt::println("Data removed from map");
        } else {
            fmt::println("Such map does not exist");
        }

        fmt::println("PrintTableMap: {}", memory.tableMap);


    }


}

