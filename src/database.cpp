//
// Created by Kacper Domżalski on 13/11/2023.
//

#include "headers/database.h"
#include "headers/table.h"
#include "fmt/core.h"
#include "headers/config.h"
#include "headers/memory.h"


auto Database::createDatabase(const std::vector<std::string> &tokens) -> void {

    if (tokens.size() != 3) {
        fmt::println("Error: Not enough arguments. {}", fmt::join(tokens, " "));
        return;
    }

    auto &databaseName = tokens[2];

    if (std::filesystem::exists(BASE_PATH + databaseName)) {
        fmt::println("Database {} already exists", databaseName);
    } else {
        fmt::println("Creating database {}", databaseName);
        std::filesystem::create_directory(BASE_PATH + databaseName);
    }
}


auto Database::deleteDatabase(const std::vector<std::string> &tokens) -> void {

    if (tokens.size() != 3) {
        fmt::println("Error: Not enough arguments. {}", fmt::join(tokens, " "));
        return;

    }

    auto &databaseName = tokens[2];

    if (std::filesystem::exists(BASE_PATH + databaseName)) {
        fmt::println("Deleting database {}", databaseName);
        Memory::deleteDatabase(BASE_PATH + databaseName);
        std::filesystem::remove_all(BASE_PATH + databaseName);
    } else {
        fmt::println("Database {} does not exist", databaseName);
    }
}

