//
// Created by Kacper Domżalski on 13/11/2023.
//
#pragma once

#include <string>
#include <filesystem>

class Database {
public:
    static auto createDatabase(const std::string &name, const std::filesystem::path &path) -> void;

    static auto deleteDatabase(const std::string &name) -> void;


};

