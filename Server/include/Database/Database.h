#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include "pqxx/pqxx"
#include <filesystem>

class Database {
public:
    Database(const std::string& connectionStr);
    void initialize();

private:
    pqxx::connection C;
    void createTables();
    bool databaseExists();
};

#endif // DATABASE_H
