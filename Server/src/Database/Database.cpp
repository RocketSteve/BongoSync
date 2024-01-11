#include "../include/Database/Database.h"

Database::Database() {
    std::string connectionStr = "postgresql://myappuser:myapppassword@localhost:5432/myappdb";

    try {
        C = pqxx::connection(connectionStr);
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cerr << "Can't open database with connection string: " << connectionStr << std::endl;
        }
    } catch (const pqxx::sql_error &e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query: " << e.query() << std::endl;
        std::cerr << "Connection string used: " << connectionStr << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        std::cerr << "Connection string used: " << connectionStr << std::endl;
    }
}



void Database::initialize() {
    if (!databaseExists()) {
        createTables();
    }
}

bool Database::databaseExists() {
    try {
        pqxx::connection conn("postgresql://myappuser:myapppassword@localhost:5432/myappdb");
        pqxx::work txn(conn);

        // Query to check if a specific table exists
        std::string query = "SELECT EXISTS ("
                            "SELECT FROM pg_tables "
                            "WHERE schemaname = 'public' AND tablename  = 'app_users');";

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
        W.exec("CREATE TABLE IF NOT EXISTS app_users ("
               "user_id SERIAL PRIMARY KEY,"
               "email VARCHAR(255) UNIQUE NOT NULL,"
               "hostname VARCHAR(255),"
               "password_hash VARCHAR(255),"
               "workspace_id INT);");


        // Workspace table
        W.exec("CREATE TABLE IF NOT EXISTS WORKSPACE ("
               "workspace_id SERIAL PRIMARY KEY,"
               "owner_email VARCHAR(255) REFERENCES app_users(email),"
               "workspace_hash VARCHAR(255));");

        // WorkspaceShared table
        W.exec("CREATE TABLE IF NOT EXISTS WORKSPACESHARED ("
               "workspace_id INT REFERENCES WORKSPACE(workspace_id),"
               "shared_with_email VARCHAR(255) REFERENCES app_users(email));");

        // Files table
        W.exec("CREATE TABLE IF NOT EXISTS FILES ("
               "file_id SERIAL PRIMARY KEY,"
               "workspace_id INT REFERENCES WORKSPACE(workspace_id),"
               "file_hash VARCHAR(255),"
               "file_path VARCHAR(255));");

        W.commit();
        std::cout << "Tables created successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
