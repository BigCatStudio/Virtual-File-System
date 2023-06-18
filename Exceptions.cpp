//
// Created by piotr on 6/16/23.
//

#include "Exceptions.h"

#include <utility>

FileNotOpenedException::FileNotOpenedException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {
    this->message = "File " + this->name + " did not open correctly during " + this->message;
}

const char* FileNotOpenedException::what() const noexcept {
    return message.c_str();
}


InvalidNameException::InvalidNameException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {
    this->message = "Provided file name " + this->name + " is invalid during " + this->message;
}

const char* InvalidNameException::what() const noexcept {
    return message.c_str();
}


InvalidSizeException::InvalidSizeException(std::string message, int size) noexcept
    : message {std::move(message)}, size {size} {
    this->message = "Virtual File System of size " + std::to_string(this->size) + " is invalid during " + this->message;
}

[[nodiscard]] const char* InvalidSizeException::what() const noexcept {
    return message.c_str();
}


FileNotFoundException::FileNotFoundException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {
    this->message = "File " + this->name + " was not found  during " + this->message;
}

[[nodiscard]] const char* FileNotFoundException::what() const noexcept {
    return message.c_str();
}


NotEnoughSpaceException::NotEnoughSpaceException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {
    this->message = "There is not enough space for file " + this->name + " during " + this->message;
}

[[nodiscard]] const char* NotEnoughSpaceException::what() const noexcept {
    return message.c_str();
}
