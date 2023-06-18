#include <iostream>
#include "VirtualFileSystem.h"

int main(int args, char **argv) {

    VirtualFileSystem vfs;
    try {
        if(args == 4) {
            if(!std::strcmp(argv[1], "create")) {
                vfs.create_VFS(argv[2], std::atoi(argv[3]));
            } else if(!std::strcmp(argv[2], "remove")) {
                vfs.open_VFS(argv[1]);
                vfs.remove_file(argv[3]);
            }
        } else if(args == 5) {
            if(!std::strcmp(argv[2], "cp_linux_vfs")) {
                vfs.open_VFS(argv[1]);
                vfs.copy_from_Linux_to_VFS(argv[3], argv[4]);
            } else if(!std::strcmp(argv[2], "cp_vfs_linux")) {
                vfs.open_VFS(argv[1]);
                vfs.copy_from_VFS_to_Linux(argv[3], argv[4]);
            }
        } else if(args == 3) {
            if(!std::strcmp(argv[1], "delete")) {
                vfs.remove_VFS(argv[2]);
            } else if(!std::strcmp(argv[2], "ls")) {
                vfs.open_VFS(argv[1]);
                vfs.display_VFS_content();
            } else if(!std::strcmp(argv[2], "display")) {
                vfs.open_VFS(argv[1]);
                vfs.display_VFS_structure();
            }
        }
    } catch(const std:: exception &exception) {
        std::cout << exception.what() << "\n";
    }

    //vfs.create_VFS("vfs_file.txt", 10);
    //vfs.open_VFS("vfs_file.txt");
    //vfs.display_VFS_content();
    //vfs.display_VFS_structure();
    //vfs.remove_file("copy_3kB.txt");
    //vfs.copy_from_VFS_to_Linux("new_copy_1kB.txt", "new_linux_1kB");
    //vfs.copy_from_Linux_to_VFS("linux_test_file_1kB.txt", "new_copy_1kB.txt");

    return 0;
}
