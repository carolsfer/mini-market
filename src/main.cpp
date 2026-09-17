#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "Database.h"
#include "Product.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string readText(const string& message) {
    string value;

    do {
        cout << message;
        getline(cin, value);

        if (value.empty()) {
            cout << "O campo nao pode ficar vazio.\n";
        }
    } while (value.empty());

    return value;
}

int readNonNegativeInt(const string& message) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value && value >= 0) {
            clearInput();
            return value;
        }

        cout << "Digite um numero inteiro maior ou igual a zero.\n";
        clearInput();
    }
}

double readNonNegativeDouble(const string& message) {
    double value;

    while (true) {
        cout << message;

        if (cin >> value && value >= 0) {
            clearInput();
            return value;
        }

        cout << "Digite um numero maior ou igual a zero (use ponto nos centavos).\n";
        clearInput();
    }
}

int readOption() {
    int option;
    cout << "Escolha uma opcao: ";

    if (!(cin >> option)) {
        clearInput();
        return -1;
    }

    clearInput();
    return option;
}

Product readProductData(int id = 0) {
    Product product;
    product.id = id;
    product.name = readText("Nome: ");
    product.category = readText("Categoria: ");
    product.price = readNonNegativeDouble("Preco: R$ ");
    product.quantity = readNonNegativeInt("Quantidade em estoque: ");
    return product;
}

void showProduct(const Product& product) {
    cout << "\nID: " << product.id << '\n';
    cout << "Nome: " << product.name << '\n';
    cout << "Categoria: " << product.category << '\n';
    cout << fixed << setprecision(2);
    cout << "Preco: R$ " << product.price << '\n';
    cout << "Quantidade: " << product.quantity << '\n';
    cout << "-----------------------------\n";
}

void showMenu() {
    cout << "\n=============================\n";
    cout << "        MINI MERCADO\n";
    cout << "=============================\n";
    cout << "1 - Cadastrar produto\n";
    cout << "2 - Listar produtos\n";
    cout << "3 - Consultar produto\n";
    cout << "4 - Atualizar produto\n";
    cout << "5 - Excluir produto\n";
    cout << "0 - Sair\n";
    cout << "=============================\n";
}

int main() {
    filesystem::create_directories("database");
    Database database("database/minimarket.db");

    if (!database.connect()) {
        cerr << "Erro ao abrir o banco: " << database.getLastError() << '\n';
        return 1;
    }

    if (!database.createTable()) {
        cerr << "Erro ao criar a tabela: " << database.getLastError() << '\n';
        return 1;
    }

    int option;

    do {
        showMenu();
        option = readOption();

        switch (option) {
            case 1: {
                cout << "\n--- CADASTRAR PRODUTO ---\n";
                Product product = readProductData();
                cout << (database.addProduct(product)
                             ? "Produto cadastrado com sucesso!\n"
                             : "Erro ao cadastrar produto: " + database.getLastError() + "\n");
                break;
            }
            case 2: {
                cout << "\n--- PRODUTOS CADASTRADOS ---\n";
                const auto products = database.listProducts();

                if (products.empty()) {
                    cout << "Nenhum produto cadastrado.\n";
                } else {
                    for (const Product& product : products) {
                        showProduct(product);
                    }
                }
                break;
            }
            case 3: {
                cout << "\n--- CONSULTAR PRODUTO ---\n";
                const int id = readNonNegativeInt("Digite o ID do produto: ");
                const auto product = database.findProduct(id);

                if (product.has_value()) {
                    showProduct(product.value());
                } else {
                    cout << "Produto nao encontrado.\n";
                }
                break;
            }
            case 4: {
                cout << "\n--- ATUALIZAR PRODUTO ---\n";
                const int id = readNonNegativeInt("Digite o ID do produto: ");

                if (!database.findProduct(id).has_value()) {
                    cout << "Produto nao encontrado.\n";
                    break;
                }

                cout << "Digite os novos dados:\n";
                Product product = readProductData(id);
                cout << (database.updateProduct(product)
                             ? "Produto atualizado com sucesso!\n"
                             : "Erro ao atualizar produto.\n");
                break;
            }
            case 5: {
                cout << "\n--- EXCLUIR PRODUTO ---\n";
                const int id = readNonNegativeInt("Digite o ID do produto: ");
                cout << (database.deleteProduct(id)
                             ? "Produto excluido com sucesso!\n"
                             : "Produto nao encontrado.\n");
                break;
            }
            case 0:
                cout << "\nEncerrando programa...\n";
                break;
            default:
                cout << "\nOpcao invalida. Digite um numero de 0 a 5.\n";
        }
    } while (option != 0);

    return 0;
}
