#ifndef FS_INDEXED_H
#define FS_INDEXED_H

#include <stddef.h>

// FS configuration
#define FS_TOTAL_BLOCKS 64
#define FS_BLOCK_SIZE 1024
#define FS_MAX_FILES 16
#define FS_MAX_FILENAME 64
#define FS_MAX_DATA_BLOCKS_PER_FILE 256

// Free block list node
typedef struct BlockNode {
    int blockNumber;
    struct BlockNode *next;
} BlockNode;

// File metadata and indexed block pointers
typedef struct FileInformationBlock {
    int inUse;
    int fibId;
    char filename[FS_MAX_FILENAME];
    size_t sizeBytes;
    int indexBlock;
    int dataBlockCount;
    int dataBlocks[FS_MAX_DATA_BLOCKS_PER_FILE];
} FileInformationBlock;

// System state tracking
typedef struct VolumeControlBlock {
    int totalBlocks;
    int blockSize;
    int freeBlockCount;
    int availableFibIds[FS_MAX_FILES];
    int fibIdHead;
    int fibIdTail;
    int fibIdCount;
    BlockNode *freeHead;
    BlockNode *freeTail;
} VolumeControlBlock;

// Main system structure
typedef struct FileSystem {
    VolumeControlBlock vcb;
    FileInformationBlock files[FS_MAX_FILES];
} FileSystem;

extern FileSystem fs;

// Lifecycle
void initFS(void);
void destroyFS(void);

// Operations
int createFile(const char *filename, size_t sizeBytes);
int deleteFile(const char *filename);
void listFiles(void);

// Utility
int allocateFreeBlock(void);
void returnFreeBlock(int blockNumber);
int getFileInformationBlockID(void);
void returnFileInformationBlockID(int fibId);
void printFreeBlocks(void);
int findFileIndex(const char *filename);

#endif