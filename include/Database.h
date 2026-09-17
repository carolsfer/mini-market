#ifndef DATABASE_H
#define DATABASE_H

#include <optional>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "Product.h"

class Database {
private:
    sqlite3* connection;
    std::string databasePath;

public:
    explicit Database(const std::string& path);
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool connect();
    bool createTable();
    std::string getLastError() const;

    bool addProduct(const Product& product);
    std::vector<Product> listProducts();
    std::optional<Product> findProduct(int id);
    bool updateProduct(const Product& product);
    bool deleteProduct(int id);
};

#endif
