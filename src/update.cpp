//
// Created by Kacper Domżalski on 19/12/2023.
//

#include "headers/update.h"

//UPDATE test table2 WHERE imie = Kacper AND wiek = 12 SET imie Kacper2 AND wiek 13;EXECUTE
auto Update::processUpdate(const std::vector<std::string> &tokens) -> void {
    auto tablePath = findPathToTable(tokens);

    if (!std::filesystem::exists(tablePath)) {
        fmt::println("Table {} does not exist", tablePath);
        return;
    }

    auto map = Memory::getTableData(tablePath);

    auto tokensAfterWhere = parseTokensAfterWhereToSet(tokens, map);
    auto tokensAfterSet = parseTokensAfterSet(tokens, map);
    auto rowIndexes = findRowIndexes(map, tokensAfterWhere);

    if (rowIndexes.empty()) {
        fmt::println("No rows found");
        return;
    }

    for (auto &rowIndex: rowIndexes) {
        for (auto &setFilter: tokensAfterSet) {
            map[rowIndex][setFilter.getColumnIndex()] = setFilter.getValue();
        }
    }

    fmt::println("{}", map);
    Memory::updateTable(tablePath, map);
}

auto Update::findPathToTable(const std::vector<std::string> &tokens) -> std::string {
    auto databaseName = tokens[1];
    auto tableName = tokens[2];
    auto pathToTable = std::string(BASE_PATH + databaseName + "/" + tableName);
    return pathToTable;
}


auto Update::findColumnIndex(const std::string &columnName, const std::vector<std::vector<std::string>> &map) -> int {


    auto columnIt = std::ranges::find(map[0], columnName);

    if (columnIt == map[0].end()) {
        fmt::println("Column {} not found", columnName);
        return -1;
    }

    auto columnIndex = std::ranges::distance(map[0].begin(), columnIt);


    return columnIndex;
}


auto Update::parseTokensAfterSet(const std::vector<std::string> &tokens,
                                 const std::vector<std::vector<std::string>> map) -> std::vector<UpdateSetFilter> {

    auto setIndex = std::ranges::find_if(tokens, [](const std::string &token) {
        return token == "SET";
    });

    if (setIndex == tokens.end()) {
        fmt::println("No SET keyword found");
        return std::vector<UpdateSetFilter>();
    }

    auto tempVec = std::vector<std::string>();
    auto setFilters = std::vector<std::vector<std::string>>();
    auto vectorOfSetFilters = std::vector<UpdateSetFilter>();


    for (auto it = setIndex + 1; it != tokens.end(); it++) {

        if (*it == "AND") {
            continue;
        }

        tempVec.push_back(*it);

        if (tempVec.size() == 2) {
            setFilters.push_back(std::move(tempVec));
            tempVec.clear();
        }
    }

    for (auto &element: setFilters) {

        auto columnName = element[0];
        auto value = element[1];
        auto columnIndex = findColumnIndex(columnName, map);

        auto columnType = determineColumnType(map, columnIndex);

        if (isColumnTypeIntOrFloat(map, columnIndex)) {
            if (columnType == "INT" && !isIntNumber(value)) {
                fmt::println("Incorrect value type");
                return std::vector<UpdateSetFilter>();
            } else if (columnType == "FLOAT" && !isFloatNumber(value)) {
                fmt::println("Incorrect value type");
                return std::vector<UpdateSetFilter>();
            }
        }

        auto setFilter = UpdateSetFilter(columnName, value, columnIndex);
        vectorOfSetFilters.push_back(setFilter);
    }

    return vectorOfSetFilters;
}

auto Update::parseTokensAfterWhereToSet(const std::vector<std::string> &tokens,
                                        const std::vector<std::vector<std::string>> &map) -> std::vector<Filter> {


    auto whereIndex = std::ranges::find_if(tokens, [](const std::string &token) {
        return token == "WHERE";
    });

    if (whereIndex->empty()) {
        fmt::println("No WHERE keyword found");
        return {};
    }

    auto setIndex = std::ranges::find_if(tokens, [](const std::string &token) {
        return token == "SET";
    });

    auto tempVec = std::vector<std::string>();
    auto whereFilters = std::vector<std::vector<std::string>>();
    auto vectorOfWhereFilters = std::vector<Filter>();


    for (auto it = whereIndex + 1; it != setIndex; it++) {

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

auto Update::findConditionType(const std::string &condition) -> Condition {
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

// UPDATE test table2 WHERE imie = test AND wiek > 90 AND pensja > 2000 SET imie Grzes;EXECUTE
auto Update::findRowIndexes(const std::vector<std::vector<std::string>> &map,
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


    }
    return rowIndexes;
}

auto Update::isColumnTypeIntOrFloat(const std::vector<std::vector<std::string>> &map, int columnIndex) -> bool {
    if (columnIndex <= 0 || columnIndex % 2 == 0) {
        return false;
    }
    auto columnType = map[0][columnIndex - 1];
    return columnType == "INT" || columnType == "FLOAT";
}

auto Update::determineColumnType(const std::vector<std::vector<std::string>> &map, int columnIndex) -> std::string {
    if (columnIndex <= 0 || columnIndex % 2 == 0) {
        return "INVALID";
    }
    auto columnType = map[0][columnIndex - 1];
    fmt::println("{}", columnType);
    return columnType;
}

auto Update::isFloatNumber(const std::string &str) -> bool {
    auto numberPattern = std::regex("^[-+]?[0-9]*\\.?[0-9]+$");
    return std::regex_match(str, numberPattern);
}

auto Update::isIntNumber(const std::string &str) -> bool {
    auto numberPattern = std::regex("^[-+]?[0-9]+$");
    return std::regex_match(str, numberPattern);
}
