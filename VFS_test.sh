#!/bin/bash

# creating virtual File System
./Lab5 create vfs 10

# displaying its structure
./Lab5 vfs display

# copying files from linux to Virtual File System
./Lab5 vfs cp_linux_vfs linux_test_file_1kB.txt file_1kB
./Lab5 vfs cp_linux_vfs linux_test_file_2048B.txt file_2048B
./Lab5 vfs cp_linux_vfs linux_test_file_3kB.txt file_3kB
./Lab5 vfs cp_linux_vfs linux_test_file_1kB.txt new_file_1kB

# displaying structure and files
./Lab5 vfs ls
./Lab5 vfs display

# trying to copy new file to Virtual File System with not enough space
./Lab5 vfs cp_linux_vfs linux_test_file_3kB.txt new_file_3kB

# copying from Virtual File System to linux
./Lab5 vfs cp_vfs_linux file_3kB new_linux_file_3kB.txt

# removing files from Virtual File System
./Lab5 vfs remove file_1kB
./Lab5 vfs remove file_3kB

# displaying final structure and files
./Lab5 vfs ls
./Lab5 vfs display

# deleteing virtual file system
./Lab5 delete vfs
