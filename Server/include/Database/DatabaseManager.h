#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>  // Include the pqxx library
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const std::string& connStr);
    ~DatabaseManager();

    bool createRecord(const std::string& table, const std::string& data);
    std::string readRecord(const std::string& table, int id);
    bool updateRecord(const std::string& table, int id, const std::string& newData);
    bool deleteRecord(const std::string& table, int id);

private:
    pqxx::connection conn;
};

#endif // DATABASEMANAGER_H
