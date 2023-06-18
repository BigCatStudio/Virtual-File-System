//
// Created by piotr on 6/12/23.
//

#include "../header/VirtualFileSystem.h"

/* Used in order to generate VFS and other files in the same directory as source code */
std::string VirtualFileSystem::correct_path(const std::string &name) const {
    return "../" + name;
}

/* Returns how much free size is available for file data */
int VirtualFileSystem::get_VFS_free_size() const {
    return (FSI.blocks_amount - FSI.blocks_used) * BLOCK_SIZE;
}

/* Returns size used for first info blocks */
int VirtualFileSystem::get_FSI_FI_size() const {
    int FSI_size = sizeof(FileSystemInfo);
    int FI_size = sizeof(FileInfo) * files.size();
    return FSI_size + FI_size;
}

void VirtualFileSystem::defragmentation() {
    std::ifstream current_file_VFS;
    std::ofstream modified_file_VFS;

    current_file_VFS.open(correct_path(FSI.name));
    if(!current_file_VFS.good()) {
         throw FileNotOpenedException("Defragmentation Of Virtual File System", FSI.name);
    }

    modified_file_VFS.open(correct_path("modified.txt"));
    if(!modified_file_VFS.good()) {
        current_file_VFS.close();
        throw FileNotOpenedException("Defragmentation Of Virtual File System", "modified.txt");
    }

    char buffer[BLOCK_SIZE];
    for(int i {0};i < BLOCK_SIZE;i++) {
        buffer[i] = '\0';
    }

    modified_file_VFS.seekp(0);
    for(int i {0};i < FSI.blocks_amount;i++) {
        modified_file_VFS.write(buffer, BLOCK_SIZE);
    }

    /* Setting new file data position in VFS */
    int start_address {BLOCK_SIZE*INFO_BLOCK_AMOUNT};
    int start_new {0};
    int end_new {0};

    for(auto &file : files) {
        if(file.start_address > start_address) {        // Checking if file starts at the first possible free block
            start_new = start_address;
            end_new = start_new + file.size;

            /* Saving file data */
            int current_block_size = file.size;
            current_file_VFS.seekg(file.start_address);
            modified_file_VFS.seekp(start_new);

            for(int i {0};i < std::ceil(static_cast<float>(file.size) / BLOCK_SIZE);i++) {
                if(current_block_size >= BLOCK_SIZE) {
                    current_file_VFS.read(buffer, BLOCK_SIZE);
                    modified_file_VFS.write(buffer, BLOCK_SIZE);
                } else {
                    current_file_VFS.read(buffer, current_block_size);
                    modified_file_VFS.write(buffer, current_block_size);
                }
                current_block_size -= BLOCK_SIZE;
            }

            file.start_address = start_new;
            file.end_address = end_new;
        } else {
            int current_block_size = file.size;
            current_file_VFS.seekg(file.start_address);
            modified_file_VFS.seekp(file.start_address);

            for(int i {0};i < std::ceil(static_cast<float>(file.size) / BLOCK_SIZE);i++) {
                if(current_block_size >= BLOCK_SIZE) {
                    current_file_VFS.read(buffer, BLOCK_SIZE);
                    modified_file_VFS.write(buffer, BLOCK_SIZE);
                } else {
                    current_file_VFS.read(buffer, current_block_size);
                    modified_file_VFS.write(buffer, current_block_size);
                }
                current_block_size -= BLOCK_SIZE;
            }
        }

        start_address += BLOCK_SIZE * std::ceil(static_cast<float>(file.size) / BLOCK_SIZE);
    }

    FSI.blocks_used = std::ceil(static_cast<float>(start_address) / BLOCK_SIZE);

    /* Saving FSI and FI information */
    modified_file_VFS.seekp(0);
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.name), sizeof(FSI.name));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_amount), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_used), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.files_amount), sizeof(int));

    for(const auto &file : files) {
        modified_file_VFS.write(reinterpret_cast<const char *>(&file.name), sizeof(file.name));
        modified_file_VFS.write(reinterpret_cast<const char *>(&file.start_address), sizeof(int));
        modified_file_VFS.write(reinterpret_cast<const char *>(&file.end_address), sizeof(int));
        modified_file_VFS.write(reinterpret_cast<const char *>(&file.size), sizeof(int));
    }

    current_file_VFS.close();
    modified_file_VFS.close();

    std::remove(correct_path(FSI.name).c_str());
    std::rename(correct_path("modified.txt").c_str(), correct_path(FSI.name).c_str());
}

void VirtualFileSystem::create_VFS(const std::string &name, int size) {
    /* Assigning VFS parameters */
    if(name.length() > 48 || name.length() == 0) {
        throw InvalidNameException("Creating Virtual File System", name);
    }
    if(size <= 3) {     // 3 is reserved for info about VFS structure
        throw InvalidSizeException("Creating Virtual File System", size);
    }
    strcpy(FSI.name, name.c_str());
    FSI.blocks_amount = size;   // Size should be provided in kB
    FSI.blocks_used = 3;        // At the beginning only first block is used for VFS info
    FSI.files_amount = 0;

    /* creating VFS locally */
    char buffer[BLOCK_SIZE];
    for(int i {0};i < BLOCK_SIZE;i++) {
        buffer[i] = '\0';
    }

    std::ofstream file_VFS;
    file_VFS.open(correct_path(name));
    if(!file_VFS.good()) {
        throw FileNotOpenedException("Creating Virtual File System", name);
    }

    // Writing FSI data at the beginning of VFS
    file_VFS.write(reinterpret_cast<const char *>(&FSI.name), sizeof(FSI.name));
    file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_amount), sizeof(int));
    file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_used), sizeof(int));
    file_VFS.write(reinterpret_cast<const char *>(&FSI.files_amount), sizeof(int));

    /* Saving rest of first block */
    file_VFS.write(buffer, BLOCK_SIZE - (sizeof(FSI.name) + 4 * sizeof(int)));

    /* Saving other blocks */
    for(int i {1};i < size;i++) {   // Writing the rest of the blocks
        file_VFS.write(buffer, BLOCK_SIZE);
    }
    file_VFS.close();
}

void VirtualFileSystem::open_VFS(const std::string &name) {
    std::ifstream file_VFS;
    file_VFS.open(correct_path(name));
    if(!file_VFS.good()) {
        throw FileNotOpenedException("Opening Virtual File System", name);
    }

    file_VFS.read(reinterpret_cast<char *>(&FSI.name), sizeof(FSI.name));
    file_VFS.read(reinterpret_cast<char *>(&FSI.blocks_amount), sizeof(int));
    file_VFS.read(reinterpret_cast<char *>(&FSI.blocks_used), sizeof(int));
    file_VFS.read(reinterpret_cast<char *>(&FSI.files_amount), sizeof(int));

    if(FSI.files_amount > 0) {
        for(int i {0};i < FSI.files_amount;i++) {
            FileInfo file;

            file_VFS.read(reinterpret_cast<char *>(&file.name), sizeof(file.name));
            file_VFS.read(reinterpret_cast<char *>(&file.start_address), sizeof(int));
            file_VFS.read(reinterpret_cast<char *>(&file.end_address), sizeof(int));
            file_VFS.read(reinterpret_cast<char *>(&file.size), sizeof(int));

            files.push_back(file);
        }
    }
}

int VirtualFileSystem::find_file(const std::string &name) const {
    int index {-1};
    for(int i {0};i < files.size();i++) {
        if(files.at(i).name == name) {
            index = i;
        }
    }

    if(index == -1) {        // Checking if file was found in VFS
        throw FileNotFoundException("Copying File To Linux", name);
    }

    return index;
}

void VirtualFileSystem::copy_from_Linux_to_VFS(const std::string &linux_name, const std::string &VFS_name) {
    /* Checking it operation is possible */
    if(VFS_name.length() > 48 || VFS_name.length() == 0) {
        throw InvalidNameException("Copying File To Virtual File System", VFS_name);
    }

    // First check size of file if it will fit in VFS
    int linux_file_size = std::filesystem::file_size(correct_path(linux_name));
    if(linux_file_size > get_VFS_free_size()) {
        throw NotEnoughSpaceException("Copying File To Virtual File System", linux_name);
    }

    /* Defining file info */
    FileInfo file;
    strcpy(file.name, VFS_name.c_str());

    file.start_address = FSI.blocks_used * BLOCK_SIZE;      // Always start new file at the beginning of first free block
    file.end_address = file.start_address + linux_file_size;
    file.size = linux_file_size;

    FSI.blocks_used += std::ceil(static_cast<float>(file.size) / BLOCK_SIZE);       // File might leave some free space in last block, but we will assume it is whole used
    FSI.files_amount++;

    /* Adding file to VFS */
    std::ifstream current_file_VFS;
    std::ofstream modified_file_VFS;
    std::ifstream file_linux;

    current_file_VFS.open(correct_path(FSI.name));
    if(!current_file_VFS.good()) {
        throw FileNotOpenedException("Copying File To Virtual File System", FSI.name);
    }

    modified_file_VFS.open(correct_path("modified.txt"));
    if(!modified_file_VFS.good()) {
        current_file_VFS.close();
        throw FileNotOpenedException("Copying File To Virtual File System", "modified.txt");
    }

    file_linux.open(correct_path(linux_name));
    if(!file_linux.good()) {
        current_file_VFS.close();
        modified_file_VFS.close();
        throw FileNotOpenedException("Copying File To Virtual File System", linux_name);
    }

    /* Rewriting VFS so it can be freely modified */
    char buffer[BLOCK_SIZE];
    for(int i {0};i < FSI.blocks_amount;i++) {
        current_file_VFS.read(buffer, BLOCK_SIZE);
        modified_file_VFS.write(buffer, BLOCK_SIZE);
    }

    modified_file_VFS.seekp(0);         // Setting cursor at the beginning to overwrite FSI data
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.name), sizeof(FSI.name));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_amount), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.blocks_used), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&FSI.files_amount), sizeof(int));

    modified_file_VFS.seekp(get_FSI_FI_size());     // Setting cursor at the end of File Info segments
    modified_file_VFS.write(reinterpret_cast<const char *>(&file.name), sizeof(file.name));
    modified_file_VFS.write(reinterpret_cast<const char *>(&file.start_address), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&file.end_address), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<const char *>(&file.size), sizeof(int));

    modified_file_VFS.seekp(file.start_address);      // Setting cursor at the beginning of first free block
    int current_block_size = file.size;
    for(int i {0};i < std::ceil(static_cast<float>(file.size) / BLOCK_SIZE);i++) {
        if(current_block_size >= BLOCK_SIZE) {
            file_linux.read(buffer, BLOCK_SIZE);
            modified_file_VFS.write(buffer, BLOCK_SIZE);
        } else {
            file_linux.read(buffer, current_block_size);
            modified_file_VFS.write(buffer, current_block_size);
        }
        current_block_size -= BLOCK_SIZE;
    }

    current_file_VFS.close();
    modified_file_VFS.close();
    file_linux.close();

    // Deleting current and renaming modified
    std::remove(correct_path(FSI.name).c_str());
    std::rename(correct_path("modified.txt").c_str(), correct_path(FSI.name).c_str());

    files.push_back(file);
}

void VirtualFileSystem::copy_from_VFS_to_Linux(const std::string &VFS_name, const std::string &linux_name) {
    /* Finding file in VFS */
    int index {find_file(VFS_name)};

    /* Creating file in linux */
    std::ifstream file_VFS;
    std::ofstream file_linux;

    file_VFS.open(correct_path(FSI.name));
    if(!file_VFS.good()) {
        throw FileNotOpenedException("Copying File To Linux", FSI.name);
    }

    file_linux.open(correct_path(linux_name));
    if(!file_linux.good()) {
        file_VFS.close();
        throw FileNotOpenedException("Copying File To Linux", linux_name);
    }

    /* Copying file to linux */
    file_VFS.seekg(files.at(index).start_address);      // Setting cursor at the beginning of file data
    int current_block_size = files.at(index).size;
    char buffer[BLOCK_SIZE];

    for(int i {0};i < std::ceil(static_cast<float>(files.at(index).size) / BLOCK_SIZE);i++) {
        if(current_block_size >= BLOCK_SIZE) {
            file_VFS.read(buffer, BLOCK_SIZE);
            file_linux.write(buffer, BLOCK_SIZE);
        } else {
            file_VFS.read(buffer, current_block_size);
            file_linux.write(buffer, current_block_size);
        }
        current_block_size -= BLOCK_SIZE;
    }

    file_VFS.close();
    file_linux.close();
}

void VirtualFileSystem::remove_file(const std::string &name) {
    int index {find_file(name)};
    FileInfo file_to_delete = files.at(index);

    std::ifstream current_file_VFS;
    std::ofstream modified_file_VFS;

    current_file_VFS.open(correct_path(FSI.name));
    if(!current_file_VFS.good()) {
        throw FileNotOpenedException("Removing File From Virtual File System", FSI.name);
    }

    modified_file_VFS.open(correct_path("modified.txt"));
    if(!modified_file_VFS.good()) {
        current_file_VFS.close();
        throw FileNotOpenedException("Removing File From Virtual File System", "modified.txt");
    }

    /* Rewriting VFS so it can be freely modified */
    char buffer[BLOCK_SIZE];
    for(int i {0};i < BLOCK_SIZE;i++) {
        buffer[i] = '\0';
    }

    /* Saving empty Info segment */
    for(int i {0};i < INFO_BLOCK_AMOUNT;i++) {
        modified_file_VFS.write(buffer, BLOCK_SIZE);
    }

    /* Saving original Data segment */
    current_file_VFS.seekg(BLOCK_SIZE * INFO_BLOCK_AMOUNT);
    for(int i {0};i < FSI.blocks_amount - INFO_BLOCK_AMOUNT;i++) {
        current_file_VFS.read(buffer, BLOCK_SIZE);
        modified_file_VFS.write(buffer, BLOCK_SIZE);
    }

    /* removing file from files vector */
    files.erase(files.begin() + index);
    FSI.files_amount--;

    /* Rewriting FSI info */
    modified_file_VFS.seekp(0);
    modified_file_VFS.write(reinterpret_cast<char *>(&FSI.name), sizeof(FSI.name));
    modified_file_VFS.write(reinterpret_cast<char *>(&FSI.blocks_amount), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<char *>(&FSI.blocks_used), sizeof(int));
    modified_file_VFS.write(reinterpret_cast<char *>(&FSI.files_amount), sizeof(int));

    if(FSI.files_amount > 0) {
        for(const auto &file : files) {
            modified_file_VFS.write(reinterpret_cast<const char *>(&file.name), sizeof(file.name));
            modified_file_VFS.write(reinterpret_cast<const char *>(&file.start_address), sizeof(int));
            modified_file_VFS.write(reinterpret_cast<const char *>(&file.end_address), sizeof(int));
            modified_file_VFS.write(reinterpret_cast<const char *>(&file.size), sizeof(int));
        }
    }

    /* rewriting files data */
    for(int i {0};i < BLOCK_SIZE;i++) {     // Setting buffer to array of NULL
        buffer[i] = '\0';
    }

    modified_file_VFS.seekp(file_to_delete.start_address);
    for(int i {0};i < std::ceil(static_cast<float>(file_to_delete.size) / BLOCK_SIZE);i++) {
        modified_file_VFS.write(buffer, BLOCK_SIZE);
    }

    /* Deleting current and renaming modified */
    std::remove(correct_path(FSI.name).c_str());
    std::rename(correct_path("modified.txt").c_str(), correct_path(FSI.name).c_str());

    current_file_VFS.close();
    modified_file_VFS.close();

    defragmentation();      // Calling this function in order to remove external fragmentation from FVS
}

void VirtualFileSystem::remove_VFS(const std::string &name) {
    std::remove(correct_path(name).c_str());
}

void VirtualFileSystem::display_VFS_content() const {
    std::cout << "\nVirtual File System Name:" << FSI.name
              << "\nBlocks Amount:" << FSI.blocks_amount
              << "\nBlocks Used:" << FSI.blocks_used
              << "\nFiles Amount:" << FSI.files_amount << "\n";

    if(FSI.files_amount > 0) {
        std::cout << "\nFiles\n";
        for(const auto &file : files) {
            std::cout << "Name:" << file.name
                      << " Start:" << file.start_address
                      << " End:" << file.end_address
                      << " Size:" << file.size << "\n";
        }
    }
}

void VirtualFileSystem::display_VFS_structure() const {
    /* FSI segment */
    int size = sizeof(FileSystemInfo) + files.size() * sizeof(FileInfo);
    for(int i {0};i < INFO_BLOCK_AMOUNT;i++) {
        std::cout << "\nBlock:" << i
                  << "\nAddress:" << i*BLOCK_SIZE
                  << "\nType:Virtual File System Info"
                  << "\nSize:" << BLOCK_SIZE;
        if(size > 0) {
            std::cout << "\nState:Allocated\n";
        } else {
            std::cout << "\nState:Free\n";
        }
        size -= BLOCK_SIZE;
    }

    /* File data segment */
    std::ifstream file_VFS;
    file_VFS.open(correct_path(FSI.name));
    if(!file_VFS.good()) {
        throw FileNotOpenedException("Displaying Virtual File System Structure", FSI.name);
    }

    char tab[1];
    for(int i {3};i < FSI.blocks_amount;i++) {
        file_VFS.seekg(i*BLOCK_SIZE);
        file_VFS.read(tab, 1);

        std::cout << "\nBlock:" << i
                  << "\nAddress:" << i*BLOCK_SIZE
                  << "\nType:File Data"
                  << "\nSize:" << BLOCK_SIZE;
        if(tab[0] != '\0') {
            std::cout << "\nState:Allocated\n";
        } else {
            std::cout << "\nState:Free\n";
        }
    }

    file_VFS.close();
}
