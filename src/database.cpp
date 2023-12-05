//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "database.h"
#include "table.h"
#include "fmt/core.h"
#include "config.h"


auto Database::createDatabase(const std::string &name) -> void {


    if (std::filesystem::exists(BASE_PATH + name)) {
        fmt::print("Database {} already exists\n", name);
    } else {
        fmt::print("Creating database {}\n", name);
        std::filesystem::create_directory(BASE_PATH + name);
    }

}


auto Database::deleteDatabase(const std::string &name) -> void {

    if (std::filesystem::exists(BASE_PATH + name)) {
        fmt::print("Deleting database {}\n", name);
        std::filesystem::remove_all(BASE_PATH + name);
    } else {
        fmt::print("Database {} does not exist\n", name);
    }
}

auto Database::createTable(const std::vector<std::string> &tokens) -> void {

    // Check if the directory exists
    if (!std::filesystem::exists(BASE_PATH + tokens[2])) {
        fmt::println("Database {} doesn't exist", tokens[2]);
        return;
    } else {
        Table::processCreateTable(tokens);
    }

}

auto Database::insertInto(const std::vector<std::string> &tokens) -> void {

    Table::processInsert(tokens);

}
