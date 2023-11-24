//
// Created by Kacper Domżalski on 18/11/2023.
//

#include "table.h"

//auto format(Table &c) { return std::pair<std::string, std::string>(c.name, c.typeName); };

auto isValidType(const std::string &typeStr) -> bool {
    return Table::stringToDataType(typeStr) != DataType::INVALID;
}

auto isInteger(const std::string &s) -> bool {
    return !s.empty() and std::ranges::all_of(s, [](char c) { return std::isdigit(c); });
}
// HOW TO CHECK IF STRING IS FLOAT

auto Table::processCreateTable(const std::vector<std::string> &tokens) -> void {

    auto directoryPath = std::filesystem::path(tokens[3]);
    auto fileName = tokens[2] + ".txt";
    auto fullPath = directoryPath / fileName;
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

//        if (columnType == "INT" and !isInteger(columnName)) {
//            fmt::println("Error: Column name '{}' is not a valid INT.", columnName);
//            return;
//        }

        if (!isValidType(columnType)) {
            fmt::println("Error: Invalid column type '{}'. Only STRING, INT, and FLOAT are allowed.", columnType);
            return;
        }

        fileContent.append(columnType).append(" ").append(columnName).append(" ");
    }

    auto file = std::fstream(fullPath, std::fstream::out | std::fstream::trunc);
    fmt::print(file, "{}\n", fileContent);

}

auto Table::processInsert(const std::vector<std::string> &tokens) -> void {

    //INSERT INTO path_to_file.txt tableType, column2, column3, ...)

    auto filePath = std::filesystem::path(tokens[2] + ".txt");
    auto file = std::fstream(filePath, std::fstream::app);

    auto typesFromUserVec = typesFromUserInput(tokens); //vector string
    auto typesFromTableVec = typesFromTable(tokens); //vector string

    auto isVecSizeEqual = typesFromUserVec.size() == typesFromTableVec.size();
    auto isVecEqual = std::ranges::equal(typesFromUserVec, typesFromTableVec);


    // Check if the vectors are of the same size and have the same elements in the same order
    if (isVecSizeEqual and isVecEqual and isValidTypeAndValue(tokens)) {

        for (auto it = tokens.begin() + 3; it != tokens.end(); it++) {

            fmt::print(file, "{} ", *it);
        }
        fmt::println(file, "");
    } else {
        fmt::print("The data types provided do not match the table's column types.\n");
        return;
    }
}

auto Table::typesFromUserInput(const std::vector<std::string> &tokens) -> std::vector<std::string> {
    auto typesFromUser = std::vector<std::string>();

    for (int i = 3; i < tokens.size(); ++i) {
        if (i % 2 != 0) {
            typesFromUser.push_back(tokens[i]);
        }
    }

    fmt::println("types from user: {} ", typesFromUser);

    return typesFromUser;
}

auto Table::typesFromTable(const std::vector<std::string> &tokens) -> std::vector<std::string> {
    auto typesFromTable = std::vector<std::string>();
    auto file = std::ifstream(tokens[2] + ".txt");
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

    }

    return true;
}


