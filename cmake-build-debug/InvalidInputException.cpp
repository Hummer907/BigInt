//
// Created by nursu on 11/3/2023.
//

#include "InvalidInputException.h"

const char *InvalidInputException::what() const noexcept {
    return "invalid input";
}