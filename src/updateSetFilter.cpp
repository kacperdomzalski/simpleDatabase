//
// Created by Kacper Domżalski on 19/12/2023.
//

#include "headers/updateSetFilter.h"

UpdateSetFilter::UpdateSetFilter(const std::string &columnName, const std::string &value, int columnIndex) : columnName(
        columnName), value(value), columnIndex(columnIndex) {}

int UpdateSetFilter::getColumnIndex() const {
    return columnIndex;
}

const std::string &UpdateSetFilter::getColumnName() const {
    return columnName;
}

const std::string &UpdateSetFilter::getValue() const {
    return value;
}
