//
// Created by Kacper Domżalski on 18/11/2023.
//

#include "headers/table.h"
#include "headers/config.h"
#include "headers/memory.h"

//CREATE TABLE test t2 INT wiek; DELETE TABLE test t2; EXECUTE
//INSERT INTO test t1231 STRING imie;EXECUTE

auto Table::createMemoryTable(const std::vector<std::string> &tokens) -> void {

    auto tablePath = findPathToTable(tokens);
    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];

    if (tokens.size() < 5) {
        fmt::println("Error: Not enough arguments.");
        return;
    }

    if (Memory::tableExists(tablePath) or std::filesystem::exists(tablePath)) {
        fmt::println("Error: Table '{}' already exists.", tablePath);
        return;
    } else if (!std::filesystem::exists(BASE_PATH + databaseName)) {
        fmt::println("Error: Database '{}' does not exist.", databaseName);
        return;
    }

    auto tableData = std::vector<std::vector<std::string>>();
    auto columnInfo = std::vector<std::string>();


    for (auto it = tokens.begin() + 4; it != tokens.end(); it += 2) {
        if (it + 1 == tokens.end()) {
            fmt::println("Error: Column type without a name.");
            return;
        }

        auto &columnType = *it;
        auto columnName = *(it + 1);

        if (!isValidType(columnType)) {
            fmt::println("Error: Invalid column type '{}'. Only STRING, INT, and FLOAT are allowed.", columnType);
            return;
        }

        columnInfo.push_back(columnType);
        columnInfo.push_back(columnName);
    }

    tableData.push_back(std::move(columnInfo));
    Memory::createMemoryTable(tablePath, tableData);
    fmt::println("Table created correctly {}", tableName);

}


auto Table::deleteTable(const std::vector<std::string> &tokens) -> void {

    auto fullPath = findPathToTable(tokens);

    Memory::deleteTable(fullPath);

    if (std::filesystem::exists(fullPath)) {
        std::filesystem::remove(fullPath);
        fmt::println("Table '{}' deleted correctly.", fullPath);
    } else {
        fmt::println("Error: Table '{}' does not exist in filesystem.", fullPath);
    }

}

auto Table::insertIntoMemoryTable(const std::vector<std::string> &tokens) -> void {
    auto tablePath = findPathToTable(tokens);

    if (tokens.size() < 5) {
        fmt::println("Error: Not enough arguments.");
        return;
    }


    if (!Memory::tableExists(tablePath) && !std::filesystem::exists(tablePath)) {
        fmt::print("Error: Table '{}' does not exist in memory or as a file.\n", tablePath);
        return;
    } else {
        Memory::getTableData(tablePath);
    }

    auto typesFromUser = typesFromUserInput(tokens);
    auto typesFromTable = Table::typesFromTable(tablePath);
    auto isVecSizeEqual = typesFromUser.size() == typesFromTable.size();
    auto isVecEqual = std::ranges::equal(typesFromUser, typesFromTable);

    if (isVecSizeEqual and isVecEqual and isValidTypeAndValue(tokens)) {

        auto data = std::vector<std::string>(tokens.begin() + 4, tokens.end());
        Memory::insertIntoTable(tablePath, data);

    } else {
        fmt::println("Error: The data types provided do not match the table's column types.");
        return;
    }
}


auto Table::findPathToTable(const std::vector<std::string> &tokens) -> std::string {

    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];
    auto fullPath = std::string(BASE_PATH + databaseName + "/" + tableName);

    return fullPath;
}


auto Table::isValidType(const std::string &typeStr) -> bool {
    return Table::stringToDataType(typeStr) != DataType::INVALID;
}


auto Table::isNumberInt(const std::string &str) -> bool {
    std::regex numberPattern("^[-+]?[0-9]+$");
    return std::regex_match(str, numberPattern);
}


auto Table::isNumberFloat(const std::string &str) -> bool {
    std::regex numberPattern("^[-+]?[0-9]*\\.?[0-9]+$");
    return std::regex_match(str, numberPattern);
}

auto Table::typesFromUserInput(const std::vector<std::string> &tokens) -> std::vector<std::string> {
    auto typesFromUser = std::vector<std::string>();

    for (int i = 4; i < tokens.size(); ++i) {
        if (i % 2 == 0) {
            typesFromUser.push_back(tokens[i]);
        }
    }

    return typesFromUser;
}


auto Table::stringToDataType(const std::string &typeStr) -> DataType {
    if (typeStr == "INT") {
        return DataType::INT;
    } else if (typeStr == "STRING") {
        return DataType::STRING;
    } else if (typeStr == "FLOAT") {
        return DataType::FLOAT;
    } else {
        return DataType::INVALID;
    }
}

auto Table::isValidTypeAndValue(const std::vector<std::string> &tokens) -> bool {
    for (auto it = tokens.begin() + 3; it != tokens.end(); ++it) {
        auto value = *(it + 1);

        if (*it == "INT" and !isNumberInt(value)) {
            fmt::println("Error: Column name '{}' is not a valid INT.", value);
            return false;
        }

        if (*it == "FLOAT" and !isNumberFloat(value)) {
            fmt::println("Error: Column name '{}' is not a valid FLOAT.", value);
            return false;
        }

    }

    return true;
}

auto Table::typesFromTable(const std::string &tablePath) -> std::vector<std::string> {

    auto map = Memory::getTableData(tablePath);
    auto typesFromTable = std::vector<std::string>();

    for (auto i = 0; i < map[0].size(); i += 2) {
        typesFromTable.push_back(map[0][i]);
    }

    return typesFromTable;
}



