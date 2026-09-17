#include "Database.h"

Database::Database(const std::string& path)
    : connection(nullptr), databasePath(path) {}

Database::~Database() {
    if (connection != nullptr) {
        sqlite3_close(connection);
    }
}

bool Database::connect() {
    return sqlite3_open(databasePath.c_str(), &connection) == SQLITE_OK;
}

bool Database::createTable() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "price REAL NOT NULL CHECK(price >= 0),"
        "quantity INTEGER NOT NULL CHECK(quantity >= 0)"
        ");";

    return sqlite3_exec(connection, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}

std::string Database::getLastError() const {
    if (connection == nullptr) {
        return "Banco de dados nao conectado.";
    }

    return sqlite3_errmsg(connection);
}

bool Database::addProduct(const Product& product) {
    const char* sql =
        "INSERT INTO products (name, category, price, quantity) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(connection, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(statement, 1, product.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, product.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement, 3, product.price);
    sqlite3_bind_int(statement, 4, product.quantity);

    const bool success = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);
    return success;
}

std::vector<Product> Database::listProducts() {
    std::vector<Product> products;
    const char* sql =
        "SELECT id, name, category, price, quantity "
        "FROM products ORDER BY id;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(connection, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return products;
    }

    while (sqlite3_step(statement) == SQLITE_ROW) {
        Product product;
        product.id = sqlite3_column_int(statement, 0);
        product.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
        product.category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
        product.price = sqlite3_column_double(statement, 3);
        product.quantity = sqlite3_column_int(statement, 4);
        products.push_back(product);
    }

    sqlite3_finalize(statement);
    return products;
}

std::optional<Product> Database::findProduct(int id) {
    const char* sql =
        "SELECT id, name, category, price, quantity "
        "FROM products WHERE id = ?;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(connection, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(statement, 1, id);
    std::optional<Product> result;

    if (sqlite3_step(statement) == SQLITE_ROW) {
        Product product;
        product.id = sqlite3_column_int(statement, 0);
        product.name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
        product.category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
        product.price = sqlite3_column_double(statement, 3);
        product.quantity = sqlite3_column_int(statement, 4);
        result = product;
    }

    sqlite3_finalize(statement);
    return result;
}

bool Database::updateProduct(const Product& product) {
    const char* sql =
        "UPDATE products SET name = ?, category = ?, price = ?, quantity = ? "
        "WHERE id = ?;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(connection, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(statement, 1, product.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, product.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement, 3, product.price);
    sqlite3_bind_int(statement, 4, product.quantity);
    sqlite3_bind_int(statement, 5, product.id);

    const bool executed = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);
    return executed && sqlite3_changes(connection) > 0;
}

bool Database::deleteProduct(int id) {
    const char* sql = "DELETE FROM products WHERE id = ?;";
    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(connection, sql, -1, &statement, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(statement, 1, id);
    const bool executed = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);
    return executed && sqlite3_changes(connection) > 0;
}
