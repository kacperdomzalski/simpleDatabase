//
// Created by Kacper Domżalski on 18/11/2023.
//

#include "table.h"

auto format(Table &c) { return std::pair<std::string, std::string>(c.name, c.typeName); };

auto Table::processCreateTable(const std::vector<std::string> &tokens) -> void {

    Table table;

    auto directoryPath = std::filesystem::path(tokens[3]);
    auto fileName = tokens[2] + ".txt";
    auto fullPath = directoryPath / fileName;


    // Check if the file (table) already exists
    if (std::filesystem::exists(fullPath)) {
        fmt::println("Error: Table {} already exists in the directory.", tokens[2]);
        return;
    }

    auto file = std::fstream(fullPath, std::fstream::out | std::fstream::trunc);


    for (auto it = tokens.begin() + 4; it != tokens.end(); it += 2) {

        if (it + 1 == tokens.end()) {
            fmt::println("Error: Column type without a name.");
            return; // Exit if there's a type with no corresponding name
        }

        auto columnType = table.typeName = (*it);
        auto columnName = table.name = (*(it + 1));


        fmt::print(file, "{} {} ", columnType, columnName); // Write the column to the file
    }

    fmt::println(file, " ");

}

//add validation
auto Table::processInsert(const std::vector<std::string> &tokens) -> void {

    //INSERT INTO path_to_file.txt tableType, column2, column3, ...)

    auto filePath = std::filesystem::path(tokens[2] + ".txt");
    auto file = std::fstream(filePath, std::fstream::app);

    for (auto it = tokens.begin() + 3; it != tokens.end(); it++) {
        fmt::print(file, "{} ", *it);

    }

    fmt::println(file, " ");
}