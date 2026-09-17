# MiniMarket

MiniMarket is a C++ terminal application for managing a small grocery store's product records. The project was originally developed as a college programming exercise and was later refactored to improve its structure, readability, and data management.

The current version uses SQLite for persistent storage and implements the basic CRUD operations: create, read, update, and delete products. It also includes input validation, parameterized SQL queries, separation of responsibilities, and a CMake configuration.

## Features

- Register products with a name, category, price, and stock quantity
- List all registered products
- Search for a product by ID
- Update and delete products by ID
- Validate empty fields, invalid numeric input, and negative values
- Automatically create the database and table on the first run
- Use parameterized queries instead of building SQL commands from user input

## Project Structure

```text
MiniMarket/
|-- include/
|   |-- Database.h
|   `-- Product.h
|-- src/
|   |-- Database.cpp
|   `-- main.cpp
|-- database/
|   `-- .gitkeep
|-- CMakeLists.txt
|-- .gitignore
`-- README.md
```

The `main.cpp` file handles the menu, user input, and messages displayed in the terminal. The `Product` struct represents a product's data. The `Database` class is responsible only for the database connection and SQL operations.

## Requirements

- A compiler with C++17 support
- CMake 3.16 or newer
- SQLite 3 development files

## Windows with MSYS2 UCRT64

Open the **MSYS2 UCRT64** terminal and install the required packages:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-sqlite3
```

Navigate to the project directory and build the application:

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

Run the application from the project root directory:

```bash
./build/minimarket.exe
```

## Linux (Ubuntu/Debian)

Install the required packages:

```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev
```

Build and run the application:

```bash
cmake -S . -B build
cmake --build build
./build/minimarket
```

## Database

The application automatically creates the `database/minimarket.db` file. This database file is not committed to GitHub because it is included in `.gitignore`.

The application creates the following table:

```sql
CREATE TABLE products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    category TEXT NOT NULL,
    price REAL NOT NULL CHECK(price >= 0),
    quantity INTEGER NOT NULL CHECK(quantity >= 0)
);
```

## Sample Data for Testing

Use the application menu to register the following products:

| Name | Category | Price | Quantity |
| --- | --- | ---: | ---: |
| Rice 5 kg | Food | 24.90 | 15 |
| Milk 1 L | Dairy | 5.49 | 30 |
| Soap | Personal Care | 3.25 | 20 |

After registering the products, test the product list, search for an existing and a nonexistent ID, update a product, and delete a product. You can also enter text in numeric fields or use negative values to check the input validation.
