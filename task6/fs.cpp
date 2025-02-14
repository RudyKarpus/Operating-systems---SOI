#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;

constexpr int BLOCK_SIZE = 64;
constexpr int NUM_BLOCKS = 56;
constexpr int MAX_FILE_DESCRIPTORS = 16;

struct CriticalSection {
    int operation = -1; // -1 = empty, 0 = delete, 1 = write
    int descriptor_index = -1;
    int new_index = -1;
    int size = 0;
};

struct FileDescriptor {
    int index = -2; // -2 = unused, -1 = created but memory not allocated
    int size = 0;
    char name[5] = "";
};

class FileSystem {
private:
    vector<vector<char>> memory;
    vector<int> free_map;
    vector<FileDescriptor> file_descriptors;
    CriticalSection critical_section;

public:
    FileSystem() {
        memory.resize(NUM_BLOCKS, vector<char>(BLOCK_SIZE, 0));
        free_map.resize(NUM_BLOCKS, 0);
        file_descriptors.resize(MAX_FILE_DESCRIPTORS);
    }

    void write(int descriptor_index, const vector<char>& data, bool break1, bool break2, bool break3, bool break4) {
        if (descriptor_index < 0 || descriptor_index >= MAX_FILE_DESCRIPTORS) {
            throw invalid_argument("Invalid descriptor index");
        }
        FileDescriptor& fd = file_descriptors[descriptor_index];
        if (fd.index == -2) {
            throw invalid_argument("File descriptor not in use");
        }

        int data_size = data.size();
        vector<int> free_blocks;
        int new_index;
        int needed_blocks = round((data_size + BLOCK_SIZE - 1) / BLOCK_SIZE);
        int taken_blocks = round(fd.size / BLOCK_SIZE);
        int k = -1;
        if (data_size > fd.size) {
            for (int i = 0; i < NUM_BLOCKS; ++i) {
                if (free_map[i] == 0) {
                    free_blocks.push_back(i);
                    if (free_blocks.size() == needed_blocks) break;
                }
                else if (fd.index == i) {
                    k = 0;
                    free_blocks.push_back(i);
                    if (free_blocks.size() == needed_blocks) break;
                }
                else if (k!=-1 && k<taken_blocks) {
                    free_blocks.push_back(i);
                    if (free_blocks.size() == needed_blocks) break;
                    k++;
                }
                else {
                    free_blocks.clear();
                }
            }
            if (free_blocks.size() < needed_blocks) {
                throw runtime_error("Not enough free space to write the file");
            }
            new_index = free_blocks[0];
        }
        else {
            new_index = fd.index;
        }

        // Allocate new blocks
        for (int i = 0; i < needed_blocks; ++i) {
            free_map[new_index + i] = 1;
        }
        critical_section = { 1, descriptor_index, new_index, data_size };
        if (break1) return;
        for (int i = 0; i < needed_blocks; ++i) {
            free_map[new_index + i] = 1;
        }
        if (break2) return;
        // Release old blocks if different
        if (fd.index != new_index) {
            int old_blocks = (fd.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            for (int i = 0; i < old_blocks; ++i) {
                free_map[fd.index + i] = 0;
            }
        }
        // Release old blocks if less blocks saved
        if (fd.index == new_index && fd.size > data_size) {
            int old_blocks = round(fd.size/ BLOCK_SIZE) - needed_blocks;
            for (int i = 0; i <= old_blocks; ++i) {
                free_map[fd.index + needed_blocks + i] = 0;
            }
        }
        if (break3) return;
        // Update file descriptor
        fd.index = new_index;
        if (break4) return;
        fd.size = data_size;
        // Clear critical section
        critical_section.operation = -1;
    }

    void repair() {
        if (critical_section.operation == -1) {
            return;
        }

        int operation = critical_section.operation;
        int descriptor_index = critical_section.descriptor_index;
        int new_index = critical_section.new_index;
        int size = critical_section.size;
        int needed_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

        if (operation == 1) {
            FileDescriptor& fd = file_descriptors[descriptor_index];
            if (fd.index != new_index) {
                int old_blocks = (fd.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
                for (int i = 0; i < old_blocks; ++i) {
                    free_map[fd.index + i] = 0;
                }
            }
            if (fd.index == new_index && fd.size > size) {
                int old_blocks = round(fd.size / BLOCK_SIZE) - needed_blocks;
                for (int i = 0; i <= old_blocks; ++i) {
                    free_map[fd.index + needed_blocks + i] = 0;
                }
            }
            fd.index = new_index;
            fd.size = size;

            for (int i = 0; i < needed_blocks; ++i) {
                free_map[new_index + i] = 1;
            }
        }
        else if (operation == 0) {
            FileDescriptor& fd = file_descriptors[descriptor_index];

            int old_blocks = (fd.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            for (int i = 0; i < old_blocks; ++i) {
                free_map[fd.index + i] = 0;
            }
            fd.index = -2;
        }

        // Clear the critical section
        critical_section.operation = -1;
    }

    int createFile(const char* name) {
        if (strlen(name) > 4) {
            throw invalid_argument("Too long file name");
        }

        for (int i = 0; i < MAX_FILE_DESCRIPTORS; ++i) {
            FileDescriptor& fd = file_descriptors[i];
            if (fd.index == -2) {
                fd.index = -1;
                fd.size = 0;
                strncpy_s(fd.name, sizeof(fd.name), name, 4);
                return i;
            }
        }
    }

    void print() {
        cout << "Critical section:\n";
        cout << critical_section.operation << "\n" << critical_section.descriptor_index << "\n" << critical_section.new_index << "\n" << critical_section.size << "\n";
        cout << "File descriptors:\n";
        for (int i = 0; i < MAX_FILE_DESCRIPTORS; i++) {
            FileDescriptor& fd = file_descriptors[i];
            cout << "File descriptor " << i << ":\n";
            cout << fd.index << "\n" << fd.name << "\n" << fd.size << "\n";
            
        }
        cout << "Free blocks map:\n";
        for (int i = 0; i < NUM_BLOCKS; i++) {
            cout << i + 1 << ": " << free_map[i] << "\n";
        }
    }
};