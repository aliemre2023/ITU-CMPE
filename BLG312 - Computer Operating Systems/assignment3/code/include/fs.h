#ifndef FS_H
#define FS_H

#define BLOCK_SIZE 1024

// Superblock
typedef struct { 
    int magic_number; 
    int num_blocks; 
    int num_inodes; 
    int bitmap_start; 
    int inode_start; 
    int data_start;
} SuperBlock;

// Inode
typedef struct { 
    int is_valid;               // 4
    int size;                   // 4
    int direct_blocks [4];      // 4 * 4
    int is_directory;           // 4
    int owner_id;               // 4
} Inode;                        // == 32 bytes

// DirectoryEntry
typedef struct { 
    int inode_number;
    char name [28];
} DirectoryEntry;

void mkfs(const char *diskfile);
int mkdir_fs(const char *path);
int create_fs(const char *path);
int write_fs(const char *path, const char *data);
int read_fs(const char *path, char *buf, int bufsize);
int delete_fs(const char *path);
int rmdir_fs(const char *path); 
int ls_fs(const char *path, DirectoryEntry *entries , int max_entries);



#endif