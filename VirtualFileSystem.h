//
// Created by piotr on 6/11/23.
//

#ifndef LAB5_VIRTUALFILESYSTEM_H
#define LAB5_VIRTUALFILESYSTEM_H

#include <iostream>
#include <string>
#include <cstring>      // Used for operations on char arrays
#include <fstream>
#include <filesystem>
#include <vector>
#include <iomanip>      // Used for writing and reading FSI and FI
#include <cmath>        // Used for dividing memory amount and std::ceil()
#include <cstdio>       // For operations on real files (deleting, renaming)

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
    int get_VFS_free_size();
    int get_FSI_FI_size();
    void display_structures_size();

public:
    void create_VFS(std::string name, int size);
    void open_VFS(std::string name);
    void copy_from_Linux_to_VFS(std::string linux_name, std::string VFS_name);
    void copy_from_VFS_to_Linux(std::string VFS_name, std::string linux_name);
};



#endif //LAB5_VIRTUALFILESYSTEM_H
