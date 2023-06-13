#include <iostream>
#include "VirtualFileSystem.h"

int main() {
    VirtualFileSystem vfs;
    //vfs.create_VFS("vfs_file.txt", 10);
    vfs.open_VFS("vfs_file.txt");
    //vfs.display_VFS_content();
    vfs.display_VFS_structure();
    //vfs.remove_file("copy_2048B.txt");
    //vfs.copy_from_VFS_to_Linux("copy_3kB.txt", "new_linux_3kB");
    //vfs.copy_from_Linux_to_VFS("linux_test_file_1kB.txt", "new_copy_1kB.txt");

    return 0;
}
