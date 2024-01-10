//
// Created by Kacper Domżalski on 20/12/2023.
//

#include "headers/deleteRow.h"

// DELETE FROM database table_name WHERE column (condition) value AND column (condition) value;
// find path to table
// find column index
// find condition type
// check condition
// find row indexes
// delete rows from Map


// DELETE FROM test table3 WHERE imie = kuba;
auto Delete::processDelete(const std::vector<std::string> &tokens) -> void {
    auto pathToTable = findPathToTable(tokens);

    if (!std::filesystem::exists(pathToTable)) {
        fmt::println("Table {} does not exist", pathToTable);
        return;
    }

    auto map = Memory::getTableData(pathToTable);

    auto tokensAfterWhere = parseTokensAfterWhereToSet(tokens, map);
    auto rowIndexes = findRowIndexes(map, tokensAfterWhere);

    if (rowIndexes.empty()) {
        fmt::println("No rows found");
        return;
    }

    Memory::removeRowFromTable(pathToTable, rowIndexes);
}

auto Delete::findPathToTable(const std::vector<std::string> &tokens) -> std::string {

    auto &databaseName = tokens[2];
    auto &tableName = tokens[3];
    auto pathToTable = std::string(BASE_PATH + databaseName + "/" + tableName);
    return pathToTable;


}

auto Delete::parseTokensAfterWhereToSet(const std::vector<std::string> &tokens,
                                        const std::vector<std::vector<std::string>> &map) -> std::vector<Filter> {


    auto whereIndex = std::ranges::find_if(tokens, [](const std::string &token) {
        return token == "WHERE";
    });

    if (whereIndex->empty()) {
        fmt::println("No WHERE keyword found");
        return {};
    }

    auto tempVec = std::vector<std::string>();
    auto whereFilters = std::vector<std::vector<std::string>>();
    auto vectorOfWhereFilters = std::vector<Filter>();


    for (auto it = whereIndex + 1; it != tokens.end(); it++) {

        if (*it == "AND") {
            continue;
        }

        tempVec.push_back(*it);

        if (tempVec.size() == 3) {
            whereFilters.push_back(std::move(tempVec));
            tempVec.clear();
        }
    }


    for (auto &element: whereFilters) {

        auto columnName = element[0];
        auto value = element[2];
        auto columnIndex = findColumnIndex(columnName, map);
        auto condition = findConditionType(element[1]);

        auto whereFilter = Filter(value, columnName, condition, columnIndex);
        vectorOfWhereFilters.push_back(whereFilter);
    }


    return vectorOfWhereFilters;
}


auto
Delete::findColumnIndex(const std::string &columnName, const std::vector<std::vector<std::string>> &map) -> int {

    auto columnIt = std::ranges::find(map[0], columnName);

    if (columnIt == map[0].end()) {
        fmt::println("Column {} not found", columnName);
        return -1;
    }

    auto columnIndex = std::ranges::distance(map[0].begin(), columnIt);


    return static_cast<int>(columnIndex);
}

auto Delete::findConditionType(const std::string &condition) -> Condition {
    Condition conditionType;

    if (condition == "=") {
        conditionType = Condition::EQUAL;
    } else if (condition == "!=") {
        conditionType = Condition::NOT_EQUAL;
    } else if (condition == "<") {
        conditionType = Condition::LESS;
    } else if (condition == ">") {
        conditionType = Condition::GREATER;
    } else {
        fmt::println("Incorrect condition operator");
        conditionType = Condition::INVALID;
    }

    return conditionType;
}


auto Delete::findRowIndexes(const std::vector<std::vector<std::string>> &map,
                            const std::vector<Filter> &filters) -> std::vector<int> {

    auto rowIndexes = std::vector<int>();

    for (int rowIndex = 1; rowIndex < map.size(); ++rowIndex) {
        bool rowMatches = true;

        for (const auto &filter: filters) {
            auto cellValue = map[rowIndex][filter.getColumnIndex()];

            if (filter.getColumnIndex() >= map[rowIndex].size()) {
                rowMatches = false;
                break;
            }

            auto checkColumnType = isColumnTypeIntOrFloat(map, filter.getColumnIndex());
            switch (filter.getCondition()) {
                case Condition::EQUAL:
                    rowMatches &= (cellValue == filter.getFieldValue());
                    break;
                case Condition::NOT_EQUAL:
                    rowMatches &= (cellValue != filter.getFieldValue());
                    break;
                case Condition::GREATER:
                    if (checkColumnType and isFloatNumber(filter.getFieldValue())) {
                        rowMatches &= (std::stof(cellValue) > std::stof((filter.getFieldValue())));
                        break;
                    }
                case Condition::LESS:
                    if (checkColumnType and isFloatNumber(filter.getFieldValue())) {
                        rowMatches &= (std::stof(cellValue) < std::stof((filter.getFieldValue())));
                        break;
                    }
                case Condition::INVALID:
                    rowMatches = false;
                    break;

            }


            if (!rowMatches) {
                break;
            }
        }

        if (rowMatches) {
            rowIndexes.push_back(rowIndex);
        }


        std::ranges::sort(rowIndexes, std::ranges::greater());
    }
    return rowIndexes;
}

auto Delete::isColumnTypeIntOrFloat(const std::vector<std::vector<std::string>> &map, int columnIndex) -> bool {
    if (columnIndex <= 0 || columnIndex % 2 == 0) {
        return false;
    }
    auto columnType = map[0][columnIndex - 1];
    return columnType == "INT" || columnType == "FLOAT";
}

auto Delete::isFloatNumber(const std::string &str) -> bool {
    auto numberPattern = std::regex("^[-+]?[0-9]*\\.?[0-9]+$");
    return std::regex_match(str, numberPattern);
}

auto Delete::isIntNumber(const std::string &str) -> bool {
    auto numberPattern = std::regex("^[-+]?[0-9]+$");
    return std::regex_match(str, numberPattern);
}