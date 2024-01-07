#include "../include/Database/Database.h"

Database::Database(const std::string& connectionStr) : C(connectionStr) {
    if (C.is_open()) {
        std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    } else {
        std::cerr << "Can't open database" << std::endl;
    }
}

void Database::initialize() {
    if (!databaseExists()) {
        createTables();
    }
}

bool Database::databaseExists() {
    try {
        pqxx::connection conn("dbname=your_dbname user=your_username password=your_password host=your_host");
        pqxx::work txn(conn);

        // Query to check if a specific table exists
        std::string query = "SELECT EXISTS ("
                            "SELECT FROM pg_tables "
                            "WHERE schemaname = 'public' AND tablename  = 'your_table_name');";

        pqxx::result r = txn.exec(query);

        // Check if the query result is true (table exists)
        if (!r.empty() && r[0][0].as<bool>()) {
            return true;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return false;
}

void Database::createTables() {
    try {
        pqxx::work W(C);

        // User table
        W.exec("CREATE TABLE IF NOT EXISTS USER ("
               "email VARCHAR(255) PRIMARY KEY NOT NULL,"
               "hostname VARCHAR(255),"
               "password_hash VARCHAR(255),"
               "workspace_id INT);");

        // Workspace table
        W.exec("CREATE TABLE IF NOT EXISTS WORKSPACE ("
               "workspace_id SERIAL PRIMARY KEY,"
               "owner_email VARCHAR(255) REFERENCES USER(email),"
               "workspace_hash VARCHAR(255));");

        // WorkspaceShared table
        W.exec("CREATE TABLE IF NOT EXISTS WORKSPACESHARED ("
               "workspace_id INT REFERENCES WORKSPACE(workspace_id),"
               "shared_with_email VARCHAR(255) REFERENCES USER(email));");

        // Files table
        W.exec("CREATE TABLE IF NOT EXISTS FILES ("
               "file_id SERIAL PRIMARY KEY,"
               "workspace_id INT REFERENCES WORKSPACE(workspace_id),"
               "file_hash VARCHAR(255));");

        W.commit();
        std::cout << "Tables created successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
