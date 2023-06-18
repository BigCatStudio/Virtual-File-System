//
// Created by piotr on 6/11/23.
//

#ifndef LAB5_VIRTUALFILESYSTEM_H
#define LAB5_VIRTUALFILESYSTEM_H

#include <iostream>
#include <cstring>      // Used for operations on char arrays
#include <fstream>
#include <filesystem>
#include <vector>
#include <iomanip>      // Used for writing and reading FSI and FI
#include <cmath>        // Used for dividing memory amount and std::ceil()
#include <cstdio>       // For operations on real files (deleting, renaming)
#include "Exceptions.h"

/*
 * VFS - Virtual File System
 * FSI - File System Info
 * FI - File Info
 */

struct FileInfo {
    char name[48];      // Name of file used for identifying it
    int start_address;  // Position where data related to file starts
    int end_address;    // Position where data related to file ends
    int size;           // Size of file in bytes -> size = end_address - start_address
};

struct FileSystemInfo {
    char name[48];
    int blocks_amount;      // Total amount of available blocks
    int blocks_used;        // Amount of free blocks can be evaluated from these variables
    int files_amount;       // Amount of files that are saved in VFS
};

class VirtualFileSystem {
private:
    static const int BLOCK_SIZE {1024};
    static const int INFO_BLOCK_AMOUNT {3};

    FileSystemInfo FSI;             // Structure containing info about VFS
    std::vector<FileInfo> files;    // List of currently stored files in VFS - it should be ordered according to start_address

    /* Utilities functions */
    [[ nodiscard ]] std::string correct_path(const std::string &name) const;
    [[ nodiscard ]] int get_VFS_free_size() const;
    [[ nodiscard ]] int get_FSI_FI_size() const;
    [[ nodiscard ]] int find_file(const std::string &name) const;
    void defragmentation();

public:
    void create_VFS(const std::string &name, int size);
    void open_VFS(const std::string &name);    // Change it to Private function that should be called at the beginning of other functions
    void copy_from_Linux_to_VFS(const std::string &linux_name, const std::string &VFS_name);
    void copy_from_VFS_to_Linux(const std::string &VFS_name, const std::string &linux_name);
    void remove_file(const std::string &name);
    void remove_VFS(const std::string &name);
    void display_VFS_content() const;
    void display_VFS_structure() const;
};

#endif //LAB5_VIRTUALFILESYSTEM_H
