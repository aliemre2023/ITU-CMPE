#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "fs.h"
#include "utils.h"

#define TOTAL_BLOCKS 1024
#define BLOCK_SIZE 1024

/*
inodes per block => 1024 / 32 = 32
total inodes = 32 * 9 = 288
*/
#define NUM_INODES 288 

#define BITMAP_BLOCK 1
#define INODE_BITMAP_SIZE (NUM_INODES / 8)
#define BLOCK_BITMAP_SIZE (BLOCK_SIZE - INODE_BITMAP_SIZE)

char log_message[256];

int search_directory_entries(FILE* disk, SuperBlock sb, int count, char** folders, Inode* current_inode, char* file_name) {
    int file_inode_index = -1;
    
    for (int j = 0; j < 4 && file_inode_index == -1; j++) {
        if (current_inode->direct_blocks[j] == -1) {
            continue;
        }
        
        DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb.data_start * BLOCK_SIZE + current_inode->direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
        if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not read directory entries\n");
            for (int k = 0; k < count; k++) {
                free(folders[k]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        for (int k = 0; k < (int)((int)(BLOCK_SIZE / sizeof(DirectoryEntry))); k++) {
            if (entries[k].inode_number != -1 && strcmp(entries[k].name, file_name) == 0) {
                file_inode_index = entries[k].inode_number;
                break;
            }
        }
    }
    
    return file_inode_index;  // Return: -1 if not found,
}

int read_root_inode(FILE* disk, SuperBlock sb, int count, char** folders, Inode* current_inode){
    fseek(disk, sb.inode_start * BLOCK_SIZE, SEEK_SET);
    if (fread(current_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read root inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }

    return 0; // success
}

int is_parent_exist(FILE* disk, SuperBlock sb, int count, char** folders, Inode current_inode, char* item){
    for (int j = 0; j < 4; j++) {
        if (current_inode.direct_blocks[j] == -1) {
            continue;
        }
        
        DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
        if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not read directory entries\n");
            for (int k = 0; k < count; k++) {
                free(folders[k]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        for (int k = 0; k < (int)((int)(BLOCK_SIZE / sizeof(DirectoryEntry))); k++) {
            if (entries[k].inode_number != -1 && strcmp(entries[k].name, item) == 0) {
                printf("Error: %s already exists\n", item);
                for (int m = 0; m < count; m++) {
                    free(folders[m]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
        }
    }

    return 0; // Exist
}

int find_parent_directory(FILE* disk, SuperBlock sb, int count, char** folders, Inode* current_inode, int* current_inode_index) {
    for (int i = 0; i < count - 1; i++) {
        char* folder = folders[i];
        int found = 0;
        
        // Check each block in the current directory
        for (int j = 0; j < 4 && !found; j++) {
            if (current_inode->direct_blocks[j] == -1) {  // Note the -> instead of .
                continue;
            }
            
            // Take directory infos
            DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
            fseek(disk, sb.data_start * BLOCK_SIZE + current_inode->direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
            if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
                printf("Error: Could not read directory entries\n");
                for (int k = 0; k < count; k++) {
                    free(folders[k]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            // Look for the folder
            for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); k++) {
                if (entries[k].inode_number != -1 && strcmp(entries[k].name, folder) == 0) {
                    *current_inode_index = entries[k].inode_number;  // Use * to modify the pointed value
                    
                    // Take new current inode
                    Inode entry_inode;
                    fseek(disk, sb.inode_start * BLOCK_SIZE + *current_inode_index * sizeof(Inode), SEEK_SET);
                    if (fread(&entry_inode, sizeof(Inode), 1, disk) != 1) {
                        printf("Error: Could not read directory inode\n");
                        for (int m = 0; m < count; m++) {
                            free(folders[m]);
                        }
                        free(folders);
                        fclose(disk);
                        return -1;
                    }
                    
                    // Is directory
                    if(!entry_inode.is_directory) {
                        printf("Error: %s is not a directory\n", folder);
                        for (int m = 0; m < count; m++) {
                            free(folders[m]);
                        }
                        free(folders);
                        fclose(disk);
                        return -1;
                    }
                    
                    *current_inode = entry_inode;  // Copy the entry_inode to current_inode
                    found = 1;
                    break;
                }
            }
        }
        
        // If a directory is not found
        if (!found) {
            printf("Error: Directory %s not found\n", folder);
            for (int j = 0; j < count; j++) {
                free(folders[j]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
    }
    return 0; // Success
}



char** split_path(const char* path, int* count) {
    if (path == NULL) {
        *count = 0;
        return NULL;
    }
    
    char* path_copy = strdup(path);
    if (path_copy == NULL) {
        *count = 0;
        return NULL;
    }
    
    char* token;
    char** folders = malloc(sizeof(char*) * 100);
    if (folders == NULL) {
        free(path_copy);
        *count = 0;
        return NULL;
    }
    
    int i = 0;

    token = strtok(path_copy, "/");
    while (token != NULL) {
        folders[i++] = strdup(token);
        token = strtok(NULL, "/");
    }

    folders[i] = NULL;
    *count = i;

    free(path_copy);
    return folders;
}


// Helper function to list directory entries
int list_directory_entries(FILE* disk, SuperBlock* sb, Inode directory_inode, DirectoryEntry* output_entries, int max_entries) {
    if (!directory_inode.is_directory) {
        return -1;
    }
    
    int entries_found = 0;
    
    for (int i = 0; i < 4 && entries_found < max_entries; i++) {
        if (directory_inode.direct_blocks[i] == -1) {
            continue;
        }
        
        DirectoryEntry dir_entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb->data_start * BLOCK_SIZE + directory_inode.direct_blocks[i] * BLOCK_SIZE, SEEK_SET);
        if (fread(dir_entries, BLOCK_SIZE, 1, disk) != 1) {
            return -1;
        }
        
        // Copy entries to output buffer
        for (int j = 0; j < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)) && entries_found < max_entries; j++) {
            if (dir_entries[j].inode_number != -1) {
                Inode entry_inode;
                fseek(disk, sb->inode_start * BLOCK_SIZE + dir_entries[j].inode_number * sizeof(Inode), SEEK_SET);
                if (fread(&entry_inode, sizeof(Inode), 1, disk) != 1) {
                    continue;
                }
                
                strcpy(output_entries[entries_found].name, dir_entries[j].name);
                output_entries[entries_found].inode_number = dir_entries[j].inode_number;
                
                printf("%s%s ", dir_entries[j].name, entry_inode.is_directory ? "/" : "");
                //printf("(%d bytes)", entry_inode.size);
                
                entries_found++;
            }
        }
        printf("\n");
    }
    
    return entries_found;
}

/**
 * Helper function to create a file or directory in the filesystem
 * @param path Path where to create the file or directory
 * @param is_directory 1 for directory, 0 for file
 * @return 0 on success, -1 on error
 */
int create_fs_item(const char *path, int is_directory) {
    if (path == NULL || *path == '\0') {
        printf("Error: Invalid path\n");
        return -1;
    }

    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }

    // Read superblock
    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }

    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL || count == 0) {
        printf("Error: Invalid path or unable to split path\n");
        fclose(disk);
        return -1;
    }

    int current_inode_index = 0;
    Inode current_inode;
    
    // Find root inode
    fseek(disk, sb.inode_start * BLOCK_SIZE, SEEK_SET);
    if (fread(&current_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read root inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }

    if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
        return -1;
    }
    // Parent directory found, stored in current_inode

    // Check if the new item already exists
    char* new_item_name = folders[count - 1];
    if(is_parent_exist(disk, sb, count, folders, current_inode, new_item_name) != 0){
        return -1;
    }
    
    // Allocate a new inode for the new item
    int new_inode_index = -1;
    for (int i = 0; i < sb.num_inodes; i++) {
        Inode inode;
        fseek(disk, sb.inode_start * BLOCK_SIZE + i * sizeof(Inode), SEEK_SET);
        if (fread(&inode, sizeof(Inode), 1, disk) != 1) {
            continue;
        }
        
        if (inode.is_valid == 0) {
            new_inode_index = i;
            break;
        }
    }
    
    if (new_inode_index == -1) {
        printf("Error: No free inodes available\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Allocate a block for the new item's content
    int new_block_index = -1;
    uint8_t bitmap[BLOCK_SIZE];
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fread(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    for (int i = 0; i < sb.num_blocks; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        
        if ((bitmap[byte_index] & (1 << bit_index)) == 0) { // Empty block
            new_block_index = i;
            // Mark block as used
            bitmap[byte_index] |= (1 << bit_index);
            break;
        }
    }
    
    if (new_block_index == -1) {
        printf("Error: No free blocks available\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Update bitmap 
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fwrite(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Initialize the new item's inode
    Inode new_inode;
    new_inode.is_valid = 1;
    new_inode.size = 0;
    new_inode.is_directory = is_directory;
    new_inode.owner_id = 150210097;
    for (int i = 0; i < 4; i++) {
        new_inode.direct_blocks[i] = -1;
    }
    new_inode.direct_blocks[0] = new_block_index;
    
    // Write the new inode
    fseek(disk, sb.inode_start * BLOCK_SIZE + new_inode_index * sizeof(Inode), SEEK_SET);
    if (fwrite(&new_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not write new inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }

    // Initialize the item's data block
    if (is_directory) {
        // For directories: Initialize directory entries
        DirectoryEntry new_entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        for (int i = 0; i < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); i++) {
            new_entries[i].inode_number = -1;
            strcpy(new_entries[i].name, "");
        }
        
        /*
        // with "." and ".."
        strcpy(new_entries[0].name, ".");
        new_entries[0].inode_number = new_inode_index;
        
        strcpy(new_entries[1].name, "..");
        new_entries[1].inode_number = current_inode_index;
        */
        
        fseek(disk, sb.data_start * BLOCK_SIZE + new_block_index * BLOCK_SIZE, SEEK_SET);
        if (fwrite(new_entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not write new directory entries\n");
            for (int i = 0; i < count; i++) {
                free(folders[i]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
    } else {
        // For files: Initialize empty block
        char empty_block[BLOCK_SIZE] = {0};
        fseek(disk, sb.data_start * BLOCK_SIZE + new_block_index * BLOCK_SIZE, SEEK_SET);
        if (fwrite(empty_block, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not write file block\n");
            for (int i = 0; i < count; i++) {
                free(folders[i]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
    }
    
    // Add entry for new item to parent directory
    int added = 0;
    for (int j = 0; j < 4 && !added; j++) {
        if (current_inode.direct_blocks[j] == -1) {
            // Need to allocate a new block for parent directory
            int parent_block_index = -1;
            
            for (int i = 0; i < sb.num_blocks; i++) {
                int byte_index = i / 8;
                int bit_index = i % 8;
                
                if ((bitmap[byte_index] & (1 << bit_index)) == 0) {
                    parent_block_index = i;
                    // Mark block as used
                    bitmap[byte_index] |= (1 << bit_index);
                    break;
                }
            }
            
            if (parent_block_index == -1) {
                printf("Error: No free blocks available for parent directory\n");
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            // Write updated bitmap
            fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
            if (fwrite(bitmap, BLOCK_SIZE, 1, disk) != 1) {
                printf("Error: Could not write bitmap\n");
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            // Initialize parent directory block
            DirectoryEntry parent_entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
            for (int i = 0; i < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); i++) {
                parent_entries[i].inode_number = -1;
                strcpy(parent_entries[i].name, "");
            }
            
            // Add new item entry
            strcpy(parent_entries[0].name, new_item_name);
            parent_entries[0].inode_number = new_inode_index;
            
            // Write parent directory entries
            fseek(disk, sb.data_start * BLOCK_SIZE + parent_block_index * BLOCK_SIZE, SEEK_SET);
            if (fwrite(parent_entries, BLOCK_SIZE, 1, disk) != 1) {
                printf("Error: Could not write parent directory entries\n");
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            // Update parent inode
            current_inode.direct_blocks[j] = parent_block_index;
            fseek(disk, sb.inode_start * BLOCK_SIZE + current_inode_index * sizeof(Inode), SEEK_SET);
            if (fwrite(&current_inode, sizeof(Inode), 1, disk) != 1) {
                printf("Error: Could not update parent inode\n");
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            added = 1;
        } else {
            // Try to find a free entry in existing block
            DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
            fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
            if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
                printf("Error: Could not read parent directory entries\n");
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)) && !added; k++) {
                if (entries[k].inode_number == -1) {
                    // Found a free slot
                    strcpy(entries[k].name, new_item_name);
                    entries[k].inode_number = new_inode_index;
                    
                    // Write updated entries
                    fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
                    if (fwrite(entries, BLOCK_SIZE, 1, disk) != 1) {
                        printf("Error: Could not write updated parent directory entries\n");
                        for (int i = 0; i < count; i++) {
                            free(folders[i]);
                        }
                        free(folders);
                        fclose(disk);
                        return -1;
                    }
                    
                    added = 1;
                }
            }
        }
    }
    
    if (!added) {
        printf("Error: Parent directory is full (no more direct blocks available)\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    
    //printf("%s '%s' created successfully\n", is_directory ? "Directory" : "File", new_item_name);
    fclose(disk);
    return 0;
}


// --- COMMANDS --- //
void mkfs(const char *diskfile) {
    FILE* disk = fopen(diskfile, "wb+");
    if(!disk) {
        fprintf(stderr, "Error: could not create disk image.");
        return;
    }

    // Empty blocks
    char zero = 0;
    fseek(disk, TOTAL_BLOCKS * BLOCK_SIZE - 1, SEEK_SET);
    fwrite(&zero, 1, 1, disk);

    // Superblock
    SuperBlock sb;
    sb.magic_number = 0x11111111;
    sb.num_blocks = TOTAL_BLOCKS;
    sb.num_inodes = NUM_INODES;
    sb.bitmap_start = 1;
    sb.inode_start = 2;
    sb.data_start = 11;
    fseek(disk, 0, SEEK_SET);
    fwrite(&sb, sizeof(SuperBlock), 1, disk);

    // Bitmap block
    char bitmap[BLOCK_SIZE] = {0};
    fseek(disk, 1 * BLOCK_SIZE, SEEK_SET);
    fwrite(bitmap, BLOCK_SIZE, 1, disk);

    // Inode blocks
    char block[BLOCK_SIZE] = {0};
    for(int i = 2; i <= 10; i++) {
        fseek(disk, i * BLOCK_SIZE, SEEK_SET);
        fwrite(block, BLOCK_SIZE, 1, disk);
    }

    // Root
    Inode root_inode;
    root_inode.is_valid = 1;
    root_inode.size = 0;
    root_inode.is_directory = 1;
    root_inode.owner_id = 150210097;
    for(int i = 0; i < 4; i++) {
        root_inode.direct_blocks[i] = -1;
    }
    fseek(disk, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&root_inode, sizeof(Inode), 1, disk);


    //printf("Filesystem created successfully.\n");
    char log_message[100];
    snprintf(log_message, sizeof(log_message), "Filesystem created successfully: %s", diskfile);
    write_log(log_message);
    fclose(disk);
}

int mkdir_fs(const char *path) {
    return create_fs_item(path, 1); 
}

int create_fs(const char *path) {
    return create_fs_item(path, 0);
}

int ls_fs(const char *path, DirectoryEntry *entries, int max_entries) {
    if (path == NULL) {
        printf("Error: Invalid path\n");
        return -1;
    }
    
    // Open the disk file
    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }
    
    // Read the superblock
    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }
    
    // Parse the path - handle both root and regular paths
    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL) {
        printf("Error: Failed to split path\n");
        fclose(disk);
        return -1;
    }
    
    // Start with the root inode
    int current_inode_index = 0;
    Inode current_inode;
    
    // Read root inode
    if (read_root_inode(disk, sb, count, folders, &current_inode) != 0) {
        fclose(disk);
        return -1;
    }
    
    // If path is root directory, we're already at the target
    if (count == 0 || (count == 1 && strcmp(folders[0], "") == 0)) {
        // We're already at root, proceed to listing
    } else {
        // Navigate to the target directory
        if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
            fclose(disk);
            return -1;
        }

        // target directory
        char* target_dir = folders[count-1];
        int found = 0;
        
        // Look for the target directory in the parent
        for (int j = 0; j < 4 && !found; j++) {
            if (current_inode.direct_blocks[j] == -1) {
                continue;
            }
            
            DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
            fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
            if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
                printf("Error: Could not read directory entries\n");
                for (int k = 0; k < count; k++) {
                    free(folders[k]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
            
            for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); k++) {
                if (entries[k].inode_number != -1 && strcmp(entries[k].name, target_dir) == 0) {
                    current_inode_index = entries[k].inode_number;
                    
                    // Read the target directory's inode
                    fseek(disk, sb.inode_start * BLOCK_SIZE + current_inode_index * sizeof(Inode), SEEK_SET);
                    if (fread(&current_inode, sizeof(Inode), 1, disk) != 1) {
                        printf("Error: Could not read directory inode\n");
                        for (int m = 0; m < count; m++) {
                            free(folders[m]);
                        }
                        free(folders);
                        fclose(disk);
                        return -1;
                    }
                    
                    found = 1;
                    break;
                }
            }
        }
        
        if (!found) {
            printf("Error: Directory %s not found\n", target_dir);
            for (int i = 0; i < count; i++) {
                free(folders[i]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
    }
    
    // Ensure we're listing a directory
    if (!current_inode.is_directory) {
        printf("Error: %s is not a directory\n", path);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // List the directory contents
    int result = list_directory_entries(disk, &sb, current_inode, entries, max_entries);
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    fclose(disk);

    return result;
}

int write_fs(const char *path, const char *data) {
    if (path == NULL || data == NULL) {
        printf("Error: Invalid path or data\n");
        return -1;
    }

    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }

    // Read the superblock
    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }

    // Parse the path
    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL || count == 0) {
        printf("Error: Invalid path or unable to split path\n");
        fclose(disk);
        return -1;
    }

    int current_inode_index = 0;
    Inode current_inode;
    
    if (read_root_inode(disk, sb, count, folders, &current_inode) != 0) {
        return -1;
    }

    if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
        return -1;
    }

    // Now current_inode in the parent directory, look for the file
    char* file_name = folders[count - 1];
    int file_inode_index = search_directory_entries(disk, sb, count, folders, &current_inode, file_name);
    
    if (file_inode_index == -1) {
        printf("Error: File %s not found\n", file_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Read the file's inode
    Inode file_inode;
    fseek(disk, sb.inode_start * BLOCK_SIZE + file_inode_index * sizeof(Inode), SEEK_SET);
    if (fread(&file_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read file inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Make sure it's a file, not a directory
    if (file_inode.is_directory) {
        printf("Error: %s is a directory, not a file\n", file_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Calculate the length of the data
    size_t data_length = strlen(data);
    // Check if we need more blocks
    int blocks_needed = (data_length + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocks_needed > 4) {
        printf("Error: File too large, maximum size is %d bytes\n", 4 * BLOCK_SIZE);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Bitmap
    uint8_t bitmap[BLOCK_SIZE];
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fread(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    for (int i = 1; i < 4; i++) {
        if (file_inode.direct_blocks[i] != -1) {
            // Free this block in the bitmap
            int byte_index = file_inode.direct_blocks[i] / 8;
            int bit_index = file_inode.direct_blocks[i] % 8;
            bitmap[byte_index] &= ~(1 << bit_index);
            
            // Mark it as unused
            file_inode.direct_blocks[i] = -1;
        }
    }
    
    // Allocate new blocks if needed
    for (int i = 0; i < blocks_needed; i++) {
        // Skip if first block is already allocated
        if (i == 0 && file_inode.direct_blocks[0] != -1) {
            continue;
        }
        
        // Find a free block
        int new_block_index = -1;
        for (int j = 0; j < sb.num_blocks; j++) {
            int byte_index = j / 8;
            int bit_index = j % 8;
            
            if ((bitmap[byte_index] & (1 << bit_index)) == 0) {
                new_block_index = j;
                // Mark block as used
                bitmap[byte_index] |= (1 << bit_index);
                break;
            }
        }
        
        if (new_block_index == -1) {
            printf("Error: No free blocks available\n");
            for (int j = 0; j < count; j++) {
                free(folders[j]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        // Assign the block to the file
        file_inode.direct_blocks[i] = new_block_index;
    }
    
    // Write updated bitmap
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fwrite(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Update file size
    file_inode.size = data_length;
    
    // Write updated inode
    fseek(disk, sb.inode_start * BLOCK_SIZE + file_inode_index * sizeof(Inode), SEEK_SET);
    if (fwrite(&file_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not write file inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Write the data to blocks
    size_t bytes_written = 0;
    for (int i = 0; i < blocks_needed; i++) {
        int bytes_to_write = (data_length - bytes_written > BLOCK_SIZE) ? BLOCK_SIZE : (data_length - bytes_written);
                              
        fseek(disk, sb.data_start * BLOCK_SIZE + file_inode.direct_blocks[i] * BLOCK_SIZE, SEEK_SET);
        if (fwrite(data + bytes_written, bytes_to_write, 1, disk) != 1) {
            printf("Error: Could not write data to block\n");
            for (int j = 0; j < count; j++) {
                free(folders[j]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        bytes_written += bytes_to_write;
    }
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    
    //printf("Successfully wrote %zu bytes to %s\n", data_length, file_name);
    fclose(disk);
    return data_length;
}

int read_fs(const char *path, char *buf, int bufsize) {
    if (path == NULL || buf == NULL || bufsize <= 0) {
        printf("Error: Invalid parameters\n");
        return -1;
    }

    // Open the disk file
    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }

    // Read the superblock
    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }

    // Parse the path
    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL || count == 0) {
        printf("Error: Invalid path or unable to split path\n");
        fclose(disk);
        return -1;
    }

    int current_inode_index = 0;
    Inode current_inode;
    
    if (read_root_inode(disk, sb, count, folders, &current_inode) != 0) {
        return -1;
    }

    if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
        return -1;
    }

    // Now current_inode in the parent directory
    char* file_name = folders[count - 1];
    int file_inode_index = search_directory_entries(disk, sb, count, folders, &current_inode, file_name);
    
    if (file_inode_index == -1) {
        printf("Error: File %s not found\n", file_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Read the file's inode
    Inode file_inode;
    fseek(disk, sb.inode_start * BLOCK_SIZE + file_inode_index * sizeof(Inode), SEEK_SET);
    if (fread(&file_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read file inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Make sure it's a file, not a directory
    if (file_inode.is_directory) {
        printf("Error: %s is a directory, not a file\n", file_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Check if the file size exceeds the buffer size
    int bytes_to_read = file_inode.size;
    if (bytes_to_read > bufsize) {
        printf("Warning: File size (%d bytes) exceeds buffer size (%d bytes), truncating\n", 
               bytes_to_read, bufsize);
        bytes_to_read = bufsize;
    }
    
    // Read the file data from blocks
    int bytes_read = 0;
    int blocks_to_read = (bytes_to_read + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    for (int i = 0; i < blocks_to_read && i < 4; i++) {
        if (file_inode.direct_blocks[i] == -1) {
            // This shouldn't happen for a valid file
            printf("Error: File has invalid block pointers\n");
            for (int j = 0; j < count; j++) {
                free(folders[j]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        // Calculate how many bytes to read from this block
        int block_bytes = (bytes_to_read - bytes_read > BLOCK_SIZE) ? BLOCK_SIZE : (bytes_to_read - bytes_read);
        
        // Read the block data
        fseek(disk, sb.data_start * BLOCK_SIZE + file_inode.direct_blocks[i] * BLOCK_SIZE, SEEK_SET);
        if (fread(buf + bytes_read, block_bytes, 1, disk) != 1) {
            printf("Error: Could not read file data\n");
            for (int j = 0; j < count; j++) {
                free(folders[j]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        bytes_read += block_bytes;
    }
    
    // Ensure null-termination if reading a text file
    if (bytes_read < bufsize) {
        buf[bytes_read] = '\0';
    }
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    
    //printf("Successfully read %d bytes from %s\n", bytes_read, file_name);
    fclose(disk);
    return bytes_read;
}

int delete_fs(const char *path) {
    if (path == NULL || *path == '\0') {
        printf("Error: Invalid path\n");
        return -1;
    }

    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }

    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }

    // Parse the path
    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL || count == 0) {
        printf("Error: Invalid path or unable to split path\n");
        fclose(disk);
        return -1;
    }

    // Start from the root inode
    int current_inode_index = 0;
    Inode current_inode;
    
    if (read_root_inode(disk, sb, count, folders, &current_inode) != 0) {
        return -1;
    }

    if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
        return -1;
    }

    // Now we're in the parent directory, look for the file/directory to delete
    char* target_name = folders[count - 1];
    int target_inode_index = -1;
    int target_entry_block = -1;
    int target_entry_index = -1;
    
    // Search for the target in parent directory's entries
    for (int j = 0; j < 4 && target_inode_index == -1; j++) {
        if (current_inode.direct_blocks[j] == -1) {
            continue;
        }
        
        DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
        if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not read directory entries\n");
            for (int k = 0; k < count; k++) {
                free(folders[k]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); k++) {
            if (entries[k].inode_number != -1 && strcmp(entries[k].name, target_name) == 0) {
                target_inode_index = entries[k].inode_number;
                target_entry_block = j;
                target_entry_index = k;
                break;
            }
        }
    }
    
    if (target_inode_index == -1) {
        printf("Error: %s not found\n", target_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Read the target's inode
    Inode target_inode;
    fseek(disk, sb.inode_start * BLOCK_SIZE + target_inode_index * sizeof(Inode), SEEK_SET);
    if (fread(&target_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read target inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }

    if (target_inode.is_directory) {
        printf("Error: %s is a directory, not a file. Use rmdir_fs to remove directories.\n", target_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Read the bitmap
    uint8_t bitmap[BLOCK_SIZE];
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fread(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Free all blocks used by the target
    for (int i = 0; i < 4; i++) {
        if (target_inode.direct_blocks[i] != -1) {
            // Mark block as free in bitmap
            int byte_index = target_inode.direct_blocks[i] / 8;
            int bit_index = target_inode.direct_blocks[i] % 8;
            bitmap[byte_index] &= ~(1 << bit_index);
        }
    }
    
    // Write updated bitmap
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fwrite(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Mark the inode as free
    target_inode.is_valid = 0;
    fseek(disk, sb.inode_start * BLOCK_SIZE + target_inode_index * sizeof(Inode), SEEK_SET);
    if (fwrite(&target_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not update inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Remove the entry from the parent directory
    DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
    fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[target_entry_block] * BLOCK_SIZE, SEEK_SET);
    if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read directory entries\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Mark the entry as free
    entries[target_entry_index].inode_number = -1;
    strcpy(entries[target_entry_index].name, "");
    
    // Write the updated directory entries
    fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[target_entry_block] * BLOCK_SIZE, SEEK_SET);
    if (fwrite(entries, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write directory entries\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    
    //printf("Successfully deleted %s\n", target_name);
    fclose(disk);
    return 0;
}

int rmdir_fs(const char *path) {
    if (path == NULL || *path == '\0') {
        printf("Error: Invalid path\n");
        return -1;
    }

    FILE* disk = fopen("disk.img", "r+b");
    if (!disk) {
        printf("Error: Could not open disk image\n");
        return -1;
    }

    SuperBlock sb;
    fseek(disk, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, disk) != 1) {
        printf("Error: Could not read superblock\n");
        fclose(disk);
        return -1;
    }

    // Parse the path
    int count;
    char** folders = split_path(path, &count);
    if (folders == NULL || count == 0) {
        printf("Error: Invalid path or unable to split path\n");
        fclose(disk);
        return -1;
    }

    // Check if trying to delete root directory
    if (count == 1 && strcmp(folders[0], "") == 0) {
        printf("Error: Cannot remove root directory\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }

    // Start from the root inode
    int current_inode_index = 0;
    Inode current_inode;
    
  
    if (read_root_inode(disk, sb, count, folders, &current_inode) != 0) {
        return -1;
    }

    if (find_parent_directory(disk, sb, count, folders, &current_inode, &current_inode_index) != 0) {
        return -1;
    }

    // Now we're in the parent directory, look for the directory to remove
    char* target_name = folders[count - 1];
    int target_inode_index = -1;
    int target_entry_block = -1;
    int target_entry_index = -1;
    
    // Search for the target in parent directory's entries
    for (int j = 0; j < 4 && target_inode_index == -1; j++) {
        if (current_inode.direct_blocks[j] == -1) {
            continue;
        }
        
        DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
        if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not read directory entries\n");
            for (int k = 0; k < count; k++) {
                free(folders[k]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); k++) {
            if (entries[k].inode_number != -1 && strcmp(entries[k].name, target_name) == 0) {
                target_inode_index = entries[k].inode_number;
                target_entry_block = j;
                target_entry_index = k;
                break;
            }
        }
    }
    
    if (target_inode_index == -1) {
        printf("Error: Directory %s not found\n", target_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Read the target's inode
    Inode target_inode;
    fseek(disk, sb.inode_start * BLOCK_SIZE + target_inode_index * sizeof(Inode), SEEK_SET);
    if (fread(&target_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not read target inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Make sure it's a directory
    if (!target_inode.is_directory) {
        printf("Error: %s is not a directory\n", target_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Check if the directory is empty (don't check "." and "..")
    int is_empty = 1;
    
    for (int j = 0; j < 4 && is_empty; j++) {
        if (target_inode.direct_blocks[j] == -1) {
            continue;
        }
        
        DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
        fseek(disk, sb.data_start * BLOCK_SIZE + target_inode.direct_blocks[j] * BLOCK_SIZE, SEEK_SET);
        if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
            printf("Error: Could not read directory entries\n");
            for (int k = 0; k < count; k++) {
                free(folders[k]);
            }
            free(folders);
            fclose(disk);
            return -1;
        }
        
        for (int k = 0; k < (int)(BLOCK_SIZE / sizeof(DirectoryEntry)); k++) {
            if (entries[k].inode_number != -1) {
                // Found a non-empty directory
                is_empty = 0;
                printf("Error: Directory %s is not empty. Found: %s\n", target_name, entries[k].name);
                for (int i = 0; i < count; i++) {
                    free(folders[i]);
                }
                free(folders);
                fclose(disk);
                return -1;
            }
        }
    }
    
    /*
    if (!is_empty) {
        printf("Error: Directory %s is not empty\n", target_name);
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    */
    
    // Read the bitmap
    uint8_t bitmap[BLOCK_SIZE];
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fread(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Free all blocks used by the directory
    for (int i = 0; i < 4; i++) {
        if (target_inode.direct_blocks[i] != -1) {
            // Mark block as free in bitmap
            int byte_index = target_inode.direct_blocks[i] / 8;
            int bit_index = target_inode.direct_blocks[i] % 8;
            bitmap[byte_index] &= ~(1 << bit_index);
        }
    }
    
    // Write updated bitmap
    fseek(disk, sb.bitmap_start * BLOCK_SIZE, SEEK_SET);
    if (fwrite(bitmap, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write bitmap\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Mark the inode as free
    target_inode.is_valid = 0;
    fseek(disk, sb.inode_start * BLOCK_SIZE + target_inode_index * sizeof(Inode), SEEK_SET);
    if (fwrite(&target_inode, sizeof(Inode), 1, disk) != 1) {
        printf("Error: Could not update inode\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Remove the entry from the parent directory
    DirectoryEntry entries[(int)(BLOCK_SIZE / sizeof(DirectoryEntry))];
    fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[target_entry_block] * BLOCK_SIZE, SEEK_SET);
    if (fread(entries, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not read directory entries\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Mark the entry as free
    entries[target_entry_index].inode_number = -1;
    strcpy(entries[target_entry_index].name, "");
    
    // Write the updated directory entries
    fseek(disk, sb.data_start * BLOCK_SIZE + current_inode.direct_blocks[target_entry_block] * BLOCK_SIZE, SEEK_SET);
    if (fwrite(entries, BLOCK_SIZE, 1, disk) != 1) {
        printf("Error: Could not write directory entries\n");
        for (int i = 0; i < count; i++) {
            free(folders[i]);
        }
        free(folders);
        fclose(disk);
        return -1;
    }
    
    // Clean up
    for (int i = 0; i < count; i++) {
        free(folders[i]);
    }
    free(folders);
    
    //printf("Successfully removed directory %s\n", target_name);
    fclose(disk);
    return 0;
}