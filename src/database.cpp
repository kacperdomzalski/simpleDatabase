//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "database.h"
#include "table.h"
#include "fmt/core.h"


auto Database::createDatabase(const std::string &name, const std::filesystem::path &path) -> void {

    auto dbPath = path / name;
    if (std::filesystem::exists(dbPath)) {
        fmt::print("Database {} already exists\n", name);
    } else {
        fmt::print("Creating database {}\n", name);
        std::filesystem::create_directory(dbPath);
    }

}


auto Database::deleteDatabase(const std::string &name) -> void {

    if (std::filesystem::exists(name)) {
        fmt::print("Deleting database {}\n", name);
        std::filesystem::remove_all(name);
    } else {
        fmt::print("Database {} does not exist\n", name);
    }
}

auto Database::createTable(const std::vector<std::string> &tokens) -> void {

    // Check if the directory exists
    if (!std::filesystem::exists(tokens[3])) {
        fmt::println("Database doesn't exist {}: ", tokens[3]);
        return;
    } else {
        Table::processCreateTable(tokens);
    }

}

auto Database::insertInto(const std::vector<std::string> &tokens) -> void {

    Table::processInsert(tokens);

}
