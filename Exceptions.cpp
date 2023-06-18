//
// Created by piotr on 6/16/23.
//

#include "Exceptions.h"

#include <utility>

FileNotOpenedException::FileNotOpenedException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {}

const char* FileNotOpenedException::what() const noexcept {
    return ("File " + name + "did not opened correctly during " + message).c_str();
}


InvalidNameException::InvalidNameException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {}

const char* InvalidNameException::what() const noexcept {
    return ("Provided file name " + name + "is invalid during " + message).c_str();
}


InvalidSizeException::InvalidSizeException(std::string message, int size) noexcept
    : message {std::move(message)}, size {size} {}

[[nodiscard]] const char* InvalidSizeException::what() const noexcept {
    return ("Virtual File System of size " + std::to_string(size) + " is invalid during " + message).c_str();
}


FileNotFoundException::FileNotFoundException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {}

[[nodiscard]] const char* FileNotFoundException::what() const noexcept {
    return ("File " + name + "was not found  during " + message).c_str();
}


NotEnoughSpaceException::NotEnoughSpaceException(std::string message, std::string name) noexcept
    : message {std::move(message)}, name {std::move(name)} {}

[[nodiscard]] const char* NotEnoughSpaceException::what() const noexcept {
    return ("There is not enough space for file " + name + " during " + message).c_str();
}
