//
// Created by Kacper Domżalski on 17/12/2023.
//

#include "headers/whereFilter.h"


Filter::Filter(std::string fieldValue, std::string columnName, Condition condition,
               int columnIndex) : fieldValue(std::move(fieldValue)), columnName(std::move(columnName)),
                                  condition(condition),
                                  columnIndex(columnIndex) {}

const std::string &Filter::getFieldValue() const {
    return fieldValue;
}

const std::string &Filter::getColumnName() const {
    return columnName;
}

Condition Filter::getCondition() const {
    return condition;
}

int Filter::getColumnIndex() const {
    return columnIndex;
}
