//
// Created by piotr on 6/16/23.
//

#ifndef LAB5_EXCEPTIONS_H
#define LAB5_EXCEPTIONS_H

#include <exception>
#include <string>

class FileNotOpenedException : public std::exception {
    std::string message;
    std::string name;

public:
    FileNotOpenedException(std::string message, std::string name) noexcept;
    ~FileNotOpenedException() override = default;

    [[nodiscard]] const char* what() const noexcept override;
};


class InvalidNameException : public std::exception {
    std::string message;
    std::string name;

public:
    InvalidNameException(std::string message, std::string name) noexcept;
    ~InvalidNameException() override = default;

    [[nodiscard]] const char* what() const noexcept override;
};


class InvalidSizeException : public std::exception {
    std::string message;
    int size;

public:
    InvalidSizeException(std::string message, int size) noexcept;
    ~InvalidSizeException() override = default;

    [[nodiscard]] const char* what() const noexcept override;
};


class FileNotFoundException : public std::exception {
    std::string message;
    std::string name;

public:
    FileNotFoundException(std::string message, std::string name) noexcept;
    ~FileNotFoundException() override = default;

    [[nodiscard]] const char* what() const noexcept override;
};

// ("Copying File To Virtual File System", name)
class NotEnoughSpaceException : public std::exception {
    std::string message;
    std::string name;

public:
    NotEnoughSpaceException(std::string message, std::string name) noexcept;
    ~NotEnoughSpaceException() override = default;

    [[nodiscard]] const char* what() const noexcept override;
};

#endif //LAB5_EXCEPTIONS_H
