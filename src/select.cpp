//
// Created by Kacper Domżalski on 05/12/2023.
//

#include "select.h"
#include "config.h"

auto Select::selectDataFromTable(const std::vector<std::string> &tokens) -> void {

    auto test = findColumnsFromUser(tokens);

    printData(tokens);

}

auto Select::getDataFromLine(const std::string &line, const std::vector<int> &indexes) -> std::vector<std::string> {
    auto iss = std::istringstream(line);
    auto word = std::string();
    auto spaceIndex = 0;

    auto vecValues = std::vector<std::string>();

    while (std::getline(iss, word, ' ')) {
        vecValues.push_back(word);
        spaceIndex++;
    }

    auto vec = std::vector<std::string>();

    for (auto &element: indexes) {
        vec.push_back(vecValues[element]);
    }

    return vec;
}


auto Select::findPathToTable(const std::vector<std::string> &tokens) -> std::string {

    auto fromIndex = std::ranges::find(tokens, "FROM");
    auto whereIndex = std::ranges::find(tokens, "WHERE");

    if (std::ranges::distance(fromIndex, whereIndex) == 3) {
        auto dbName = std::string(*(fromIndex + 1));
        auto tableName = std::string(*(fromIndex + 2));

        auto fullPath = std::string(BASE_PATH + dbName + "/" + tableName);

        return fullPath;
    }


    return "";
}


auto Select::findColumnsFromUser(const std::vector<std::string> &tokens) -> std::vector<std::string> {

    auto fromIndex = std::ranges::find(tokens, "FROM");
    auto columns = std::vector<std::string>(tokens.begin() + 1, fromIndex);

    fmt::print("Columns: {}\n", columns);
    return columns;
}

bool doColumnsExist(const std::vector<std::string> &columnsFromUser, const std::map<std::string, int> &map) {
    for (const auto &column: columnsFromUser) {
        if (!map.contains(column)) {
            fmt::print("Error: Column '{}' does not exist in the table.\n", column);
            return false;
        }
    }
    return true;
}

auto Select::findIndexesOfColumns(const std::vector<std::string> &tokens) -> std::vector<int> {
    auto columnsFromUser = findColumnsFromUser(tokens);
    auto columnIndexes = std::vector<int>();

    auto map = std::map<std::string, int>();

    auto tablePath = findPathToTable(tokens);
    auto file = std::fstream(tablePath);

    auto line = std::string();
    if (std::getline(file, line)) { // Pobierz pierwszą linię z pliku
        auto iss = std::istringstream(line);
        auto word = std::string();
        int currentIndex = 0;
        while (iss >> word) { // podziel linie na slowa
            if (currentIndex % 2 != 0) {
                map[word] = currentIndex;
            }
            currentIndex++;
        }

    }

    // Check if all columns exist
    if (!doColumnsExist(columnsFromUser, map)) {
        return {}; // Return an empty vector or handle as needed
    }

    // If all columns exist, find their indexes
    for (const auto &column: columnsFromUser) {
        columnIndexes.push_back(map[column]);
    }





//    for (auto &e: columnsFromUser) {
//        if (map.contains(e)) {
//            columnIndexes.push_back(map[e]);
//        }
//        else {
//            // Column not found in the table, print error and return
//            fmt::print("Error: Column '{}' does not exist in the table.\n", e);
//            return {}; // Return an empty vector or handle the error as needed
//        }
//    }

    fmt::println("Map: {}\n", map);
    fmt::println("Column indexes: {}\n", columnIndexes);
    return columnIndexes;
}

auto Select::findDataForColumnsIndexes(const std::vector<std::string> &tokens,
                                       const std::vector<int> &indexes) -> std::vector<std::vector<std::string>> {
    auto tablePath = findPathToTable(tokens);
    auto file = std::fstream(tablePath);

    auto records = std::vector<std::vector<std::string>>();

    auto line = std::string();

    auto isFirstLine = true;

    while (getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
        } else {
            records.push_back(std::move(getDataFromLine(line, indexes)));
        }
    }
    return records;
}


auto format_as_string(std::vector<std::string> const &t) {
    return fmt::format("{}", fmt::join(t, " "));
}


// SELECT imie wiek FROM test table2; EXECUTE
// SELECT imie FROM test table3; EXECUTE
// SELECT imie wiek FROM test table2; SELECT imie FROM test table3;SELECT imie wiek FROM test table2;EXECUTE
auto Select::printData(const std::vector<std::string> &tokens) -> void {
    auto indexesOfColumns = findIndexesOfColumns(tokens);
    auto columnsFromUser = findColumnsFromUser(tokens);


    auto tableName = findPathToTable(tokens);


    fmt::println("test {}", Memory::getTableData(tableName));

    auto results = findDataForColumnsIndexes(tokens, indexesOfColumns);

    if (indexesOfColumns.empty()) {
        // If empty, this means one or more columns do not exist. Do not print.
        return;
    }


    fmt::println("{}", format_as_string(columnsFromUser));

    for (auto &e: results) {
        fmt::println("{} ", format_as_string(e));
    }
}


