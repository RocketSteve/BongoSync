#include "../../include/Database/DatabaseManager.h"

DatabaseManager::DatabaseManager() {
    try {
        // Setup the connection string
        std::string connectionStr = "postgresql://myappuser:myapppassword@localhost:5432/myappdb";
        C = pqxx::connection(connectionStr);

        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cerr << "Can't open database" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::createRecord(const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& data) {
    try {
        pqxx::work W(C);

        // Construct the SQL query with columns and values
        std::ostringstream query;
        query << "INSERT INTO " << table;

        if (!columns.empty()) {
            query << " (";
            for (size_t i = 0; i < columns.size(); ++i) {
                query << columns[i];
                if (i < columns.size() - 1) {
                    query << ", ";
                }
            }
            query << ")";
        }

        query << " VALUES (";
        for (size_t i = 0; i < data.size(); ++i) {
            query << W.quote(data[i]);
            if (i < data.size() - 1) {
                query << ", ";
            }
        }
        query << ");";

        // Execute the query directly
        W.exec(query.str());
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error creating record: " << e.what() << std::endl;
        return false;
    }
}



std::string DatabaseManager::readRecord(const std::string& table, int id) {
    try {
        pqxx::work W(C);

        // Mapping of table names to their respective ID column names
        std::unordered_map<std::string, std::string> idColumns = {
                {"app_users", "user_id"},
                {"workspace", "workspace_id"},
                {"workspaceshared", "workspace_id"},
                {"files", "file_id"}
        };

        // Determine the correct ID column name for the given table
        std::string idColumn = idColumns.count(table) > 0 ? idColumns[table] : "id";

        // Construct the SQL query
        std::ostringstream query;
        query << "SELECT * FROM " << table << " WHERE " << idColumn << " = " << W.quote(id) << ";";

        // Execute the query and get the result
        pqxx::result R = W.exec(query.str());

        // Process the result
        if (!R.empty()) {
            std::ostringstream resultString;
            for (const auto& row : R) {
                for (const auto& field : row) {
                    resultString << field.c_str() << " ";
                }
            }
            std::cout << "Result string: " << resultString.str() << std::endl;
            return resultString.str();
        } else {
            return "No record found.";
        }
    } catch (const std::exception &e) {
        std::cerr << "Error reading record: " << e.what() << std::endl;
        return "Error occurred.";
    }
}

bool DatabaseManager::updateRecord(const std::string& table, const std::string& column, int id, const std::string& newData) {
    try {
        pqxx::work W(C);

        // Construct the SQL query
        std::ostringstream query;
        query << "UPDATE " << table
              << " SET " << column << " = " << W.quote(newData)
              << " WHERE id = " << W.quote(id) << ";";

        // Execute the query
        W.exec(query.str());
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error updating record: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::deleteRecord(const std::string& table, int id) {
    try {
        pqxx::work W(C);

        // Construct the SQL query
        std::ostringstream query;
        query << "DELETE FROM " << table
              << " WHERE id = " << W.quote(id) << ";";

        // Execute the query
        W.exec(query.str());
        W.commit();
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error deleting record: " << e.what() << std::endl;
        return false;
    }
}


std::string DatabaseManager::readCustomRecord(const std::string& baseQuery, const std::vector<std::string>& values) {
    try {
        pqxx::work W(C);

        // Construct the SQL query by replacing placeholders
        std::string query = baseQuery;
        for (size_t i = 0; i < values.size(); ++i) {
            // Find the placeholder and replace it
            std::string placeholder = "$" + std::to_string(i + 1); // placeholders are 1-indexed
            size_t pos = query.find(placeholder);
            if (pos != std::string::npos) {
                query.replace(pos, placeholder.length(), W.quote(values[i]));
            }
        }

        // Execute the query
        pqxx::result R = W.exec(query);


        // Process the result
        if (!R.empty()) {
            std::ostringstream resultString;
            for (const auto& row : R) {
                for (const auto& field : row) {
                    resultString << field.c_str() << " ";
                }
            }
            // Manually trim the result string
            std::string result = resultString.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            return result;
        } else {
            return ""; // No record found
        }
    } catch (const std::exception &e) {
        std::cerr << "Error reading record: " << e.what() << std::endl;
        return "";
    }
}


