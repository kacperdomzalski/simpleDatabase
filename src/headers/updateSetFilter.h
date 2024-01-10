//
// Created by Kacper Domżalski on 19/12/2023.
//

#pragma once

#include <utility>
#include <string>

class UpdateSetFilter {

    int columnIndex = -1;
    std::string columnName;
    std::string value;
public:
    int getColumnIndex() const;

    const std::string &getColumnName() const;

    const std::string &getValue() const;

public:
    UpdateSetFilter(const std::string &columnName, const std::string &value, int columnIndex);


};



