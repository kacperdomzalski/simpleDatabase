//
// Created by Kacper Domżalski on 17/12/2023.
//
#pragma once

#include <string>
#include <utility>

enum class Condition {
    GREATER,
    LESS,
    EQUAL,
    NOT_EQUAL,
    INVALID
};


class Filter {
    std::string fieldValue;
    std::string columnName;
    Condition condition;
    int columnIndex = -1;


public:
    Filter(std::string fieldValue, std::string columnName, Condition condition, int columnIndex);

    [[nodiscard]] const std::string &getFieldValue() const;

    [[nodiscard]] const std::string &getColumnName() const;

    [[nodiscard]] Condition getCondition() const;

    [[nodiscard]] int getColumnIndex() const;

};


