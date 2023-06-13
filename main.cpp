#include <iostream>
#include "VirtualFileSystem.h"

int main() {
    VirtualFileSystem vfs;
    //vfs.create_VFS("vfs_file.txt", 10);
    vfs.open_VFS("vfs_file.txt");
    //vfs.copy_from_Linux_to_VFS("linux_test_file_1kB.txt", "new_copy_1kB.txt");

    return 0;
}
