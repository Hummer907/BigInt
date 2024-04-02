//
// Created by nursu on 11/3/2023.
//

#ifndef BIGINTEGER_INVALIDINPUTEXCEPTION_H
#define BIGINTEGER_INVALIDINPUTEXCEPTION_H

#include <stdexcept>
class InvalidInputException : public std::exception {
public:
    const char *what() const noexcept override;
};



#endif //BIGINTEGER_INVALIDINPUTEXCEPTION_H
