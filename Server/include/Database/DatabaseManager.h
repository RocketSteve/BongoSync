#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

class DatabaseManager {
public:
    static DatabaseManager& getInstance();
    bool createRecord(const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& data);
    std::string readRecord(const std::string& table, int id);
    bool updateRecord(const std::string& table, const std::string& column, int id, const std::string& newData);
    bool deleteRecord(const std::string& table, int id);
    std::string readCustomRecord(const std::string& query, const std::vector<std::string>& values);

private:
    pqxx::connection C;
    DatabaseManager();  // Constructor is private
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

};

#endif // DATABASEMANAGER_H
