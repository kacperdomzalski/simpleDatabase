//
// Created by Kacper Domżalski on 05/12/2023.
//

#include "select.h"
#include "config.h"

// SELECT imie FROM test table10;
// SELECT imie wiek FROM test table2 WHERE pensja > 3233.99;EXECUTE


auto Select::selectDataFromTable(const std::vector<std::string> &tokens) -> void {
    auto tableName = findPathToTable(tokens);

    if (!Memory::tableExists(tableName) and !std::filesystem::exists(tableName)) {
        fmt::println("Table {} does not exist", tableName);
        return;
    }

    auto map = Memory::getTableData(tableName);

    auto columnsToFind = findColumnsFromUser(tokens, map);

    if (columnsToFind.empty()) {
        return;
    }

    auto whereQuery = filterWhereQuery(tokens, map);

    printDataFromMapWithConditions(whereQuery, columnsToFind, map);

}

auto Select::findPathToTable(const std::vector<std::string> &tokens) -> std::string {


    auto fullPath = std::string();
    auto fromIndex = std::ranges::find(tokens, "FROM");

    auto whereIndex = std::ranges::find(tokens, "WHERE");


    if (std::ranges::distance(fromIndex, whereIndex) == 3) {
        auto dbName = std::string(*(fromIndex + 1));
        auto tableName = std::string(*(fromIndex + 2));

        fullPath = std::string(BASE_PATH + dbName + "/" + tableName);

    }


    return fullPath;
}


auto Select::findColumnsFromUser(const std::vector<std::string> &tokens,
                                 const std::vector<std::vector<std::string>> &map) -> std::vector<std::string> {

    auto fromIndex = std::ranges::find(tokens, "FROM");
    auto columns = std::vector<std::string>(tokens.begin() + 1, fromIndex);

    if (columns.size() == 1 and columns[0] == "ALL") {
        return columns;
    }

    for (const auto &column: columns) {
        auto columnIt = std::ranges::find(map[0], column);
        if (columnIt == map[0].end() and column != "STRING" and column != "INT" and column != "FLOAT") {
            fmt::println("Column {} does not exist in the table", column);
            return {};
        }
    }


    fmt::print("Columns: {}\n", columns);
    return columns;
}


auto Select::filterWhereQuery(const std::vector<std::string> &tokens,
                              const std::vector<std::vector<std::string>> &map) -> std::vector<Filter> {

    auto whereIndex = std::ranges::find_if(tokens, [](const std::string &token) {
        return token == "WHERE";
    });

    if (whereIndex->empty()) {
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


auto Select::findColumnIndex(const std::string &columnName, const std::vector<std::vector<std::string>> &map) -> int {

    auto columnIt = std::ranges::find(map[0], columnName);

    if (columnIt == map[0].end() or columnName == "STRING" or columnName == "INT" or columnName == "FLOAT") {
        fmt::println("Column {} not found", columnName);
        return -1;
    }

    auto columnIndex = std::ranges::distance(map[0].begin(), columnIt);


    return static_cast<int>(columnIndex);
}


auto Select::findConditionType(const std::string &condition) -> Condition {
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


auto Select::printDataFromMapWithConditions(const std::vector<Filter> &filters,
                                            const std::vector<std::string> &columnsToTake,
                                            const std::vector<std::vector<std::string>> &map) -> void {

    auto rowIndexes = findRowIndexes(map, filters);
    auto columnIndexes = std::vector<int>();

    if (columnsToTake[0] == "ALL" and columnsToTake.size() == 1) {
        for (int i = 1; i < map[0].size(); i += 2) {
            columnIndexes.push_back(i);
        }
    } else {
        for (const auto &column: columnsToTake) {
            int index = findColumnIndex(column, map);
            if (index != -1) {
                columnIndexes.push_back(index);
            }
        }
    }

    if (rowIndexes.empty()) {
        fmt::println("No rows found");
        return;
    }

    printTableHeaders(columnIndexes, map);

    // Print the data rows
    for (const auto &rowIndex: rowIndexes) {
        for (const auto &colIndex: columnIndexes) {
            if (colIndex < map[rowIndex].size()) {
                fmt::print("{:<15}", map[rowIndex][colIndex]);
            } else {
                fmt::print("{:<15}", " ");
            }
        }
        fmt::println(" ");
    }
}


auto Select::printTableHeaders(const std::vector<int> &columnIndexes,
                               const std::vector<std::vector<std::string>> &map) -> void {
    for (const auto &index: columnIndexes) {
        fmt::print("{:<15}", map[0][index]);
    }
    fmt::print("\n");
}

auto Select::findRowIndexes(const std::vector<std::vector<std::string>> &map,
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

auto Select::isFloatNumber(const std::string &str) -> bool {
    auto numberPattern = std::regex("^[-+]?[0-9]*\\.?[0-9]+$");
    return std::regex_match(str, numberPattern);
}

auto Select::isColumnTypeIntOrFloat(const std::vector<std::vector<std::string>> &map, int columnIndex) -> bool {
    if (columnIndex <= 0 || columnIndex % 2 == 0) {
        return false;
    }
    auto columnType = map[0][columnIndex - 1];
    return columnType == "INT" || columnType == "FLOAT";
}