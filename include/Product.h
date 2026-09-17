#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

struct Product {
    int id = 0;
    std::string name;
    std::string category;
    double price = 0.0;
    int quantity = 0;
};

#endif
