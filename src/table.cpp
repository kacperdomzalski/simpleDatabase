//
// Created by Kacper Domżalski on 18/11/2023.
//

#include "table.h"
#include "config.h"

auto isValidType(const std::string &typeStr) -> bool {
    return Table::stringToDataType(typeStr) != DataType::INVALID;
}

auto isInteger(const std::string &s) -> bool {
    return !s.empty() and std::ranges::all_of(s, [](char c) { return std::isdigit(c); });
}

auto isFloat(const std::string &s) -> bool {
    auto dotCount = std::ranges::count(s, '.');
    auto dotIndex = std::ranges::find(s, '.');

    auto beforeDot = std::string(s.begin(), dotIndex);
    auto afterDot = std::string(dotIndex + 1, s.end());

    if (dotCount != 1) {
        return false;
    }

    return !s.empty() and std::ranges::all_of(beforeDot, [](char c) { return std::isdigit(c); }) &&
           std::ranges::all_of(afterDot, [](char c) { return std::isdigit(c); });
}


auto Table::processCreateTable(const std::vector<std::string> &tokens) -> void {
//CREATE TABLE nazwaTabeli nazwaBD STRING imie INT wiek FLOAT pensja

    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];
    auto fullPath = std::filesystem::path(BASE_PATH + databaseName + "/" + tableName);
    auto fileContent = std::string();

    // Check if the file (table) already exists
    if (std::filesystem::exists(fullPath)) {
        fmt::println("Error: Table {} already exists in the directory.", tokens[2]);
        return;
    }

    for (auto it = tokens.begin() + 4; it != tokens.end(); it += 2) {

        if (it + 1 == tokens.end()) {
            fmt::println("Error: Column type without a name.");
            return; // Exit if there's a type with no corresponding name
        }

        auto columnType = *it;
        auto columnName = *(it + 1);


        if (!isValidType(columnType)) {
            fmt::println("Error: Invalid column type '{}'. Only STRING, INT, and FLOAT are allowed.", columnType);
            return;
        }

        fileContent.append(columnType).append(" ").append(columnName).append(" ");

    }

    auto file = std::fstream(fullPath, std::fstream::out | std::fstream::trunc);
    fmt::print(file, "{}\n", fileContent);
    fmt::println("Table '{}' created correctly.", tableName);

}

auto Table::processInsert(const std::vector<std::string> &tokens) -> void {

    //INSERT INTO path_to_file.txt tableType, column2, column3, ...)
    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];
    auto filePath = std::filesystem::path(BASE_PATH + databaseName + "/" + tableName);


    auto file = std::fstream(filePath, std::fstream::app);

    auto typesFromUserVec = typesFromUserInput(tokens); //vector string
    auto typesFromTableVec = typesFromTable(tokens); //vector string

    auto isVecSizeEqual = typesFromUserVec.size() == typesFromTableVec.size();
    auto isVecEqual = std::ranges::equal(typesFromUserVec, typesFromTableVec);


    // Check if the vectors are of the same size and have the same elements in the same order
    if (isVecSizeEqual and isVecEqual and isValidTypeAndValue(tokens)) {

        for (auto it = tokens.begin() + 4; it != tokens.end(); it++) {

            fmt::print(file, "{} ", *it);
        }
        fmt::println(file, "");
    } else {
        fmt::print("The data types provided do not match the table's column types.\n");
        return;
    }

    fmt::println("Data inserted correctly for database {} in table {}.", databaseName, tableName);
    
}

auto Table::typesFromUserInput(const std::vector<std::string> &tokens) -> std::vector<std::string> {
    auto typesFromUser = std::vector<std::string>();

    for (int i = 4; i < tokens.size(); ++i) {
        if (i % 2 == 0) {
            typesFromUser.push_back(tokens[i]);
        }
    }

    fmt::println("types from user: {} ", typesFromUser);

    return typesFromUser;
}

auto Table::typesFromTable(const std::vector<std::string> &tokens) -> std::vector<std::string> {
    auto typesFromTable = std::vector<std::string>();

    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];

    auto file = std::ifstream(BASE_PATH + databaseName + "/" + tableName);
    auto line = std::string();

    if (std::getline(file, line)) {
        auto iss = std::istringstream(line);
        auto type = std::string();
        while (iss >> type) {
            // Skip every second word
            auto skip = std::string();
            iss >> skip;
            typesFromTable.push_back(type);
        }
    }

    fmt::println("types from table: {} ", typesFromTable);

    return typesFromTable;
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

        if (*it == "INT" and !isInteger(value)) {
            fmt::println("Error: Column name '{}' is not a valid INT.", value);
            return false;
        }

        if (*it == "FLOAT" and !isFloat(value)) {
            fmt::println("Error: Column name '{}' is not a valid FLOAT.", value);
            return false;
        }

    }

    return true;
}


