//
// Created by Kacper Domżalski on 13/11/2023.
//
#pragma once

#include <string>
#include <filesystem>

struct Database {
    static auto createDatabase(const std::vector<std::string> &tokens) -> void;

    static auto deleteDatabase(const std::vector<std::string> &tokens) -> void;

};

