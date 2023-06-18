//
// Created by piotr on 6/16/23.
//

#ifndef LAB5_EXCEPTIONS_H
#define LAB5_EXCEPTIONS_H

#include <exception>

// ("Defragmentation Of Virtual File System")
class FileNotOpenedException : public std::exception {
    FileNotOpenedException() noexcept = default;
    ~FileNotOpenedException() override = default;

public:
    const char* what() const noexcept override;
};

// ("Creating Virtual File System", name)
class InvalidNameException : public std::exception {

};

// ("Creating Virtual File System", size)
class InvalidSizeException : public std::exception {

};

// ("Copying File To Linux", VFS_name)
class FileNotFoundException : public std::exception {

};

// ("Copying File To Virtual File System", linux_name)
class NotEnoughSpaceException : public std::exception {

};

#endif //LAB5_EXCEPTIONS_H
