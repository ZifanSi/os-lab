#ifndef FS_INDEXED_H
#define FS_INDEXED_H

#include <stddef.h>

// FS configuration
#define FS_TOTAL_BLOCKS 64
#define FS_BLOCK_SIZE 1024
#define FS_MAX_FILES 16
#define FS_MAX_FILENAME 64
#define FS_MAX_DATA_BLOCKS_PER_FILE 256


typedef struct BlockNode {
    int blockNumber;
    struct BlockNode *next;
} BlockNode;

typedef struct FileInformationBlock {
    int inUse;
    int fibId;
    char filename[FS_MAX_FILENAME];
    size_t sizeBytes;
    int indexBlock;
    int dataBlockCount;
    int dataBlocks[FS_MAX_DATA_BLOCKS_PER_FILE];
} FileInformationBlock;


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


typedef struct FileSystem {
    VolumeControlBlock vcb;
    FileInformationBlock files[FS_MAX_FILES];
} FileSystem;

extern FileSystem fs;

// Args: none
// Returns: nothing
void initFS(void);

// Args: none
// Returns: nothing
void destroyFS(void);

// Args: filename, sizeBytes
// Returns: 0 on success, -1 on failure
int createFile(const char *filename, size_t sizeBytes);

// Args: filename
// Returns: 0 on success, -1 on failure
int deleteFile(const char *filename);

// Args: none
// Returns: nothing
void listFiles(void);

// Args: none
// Returns: block number, or -1
int allocateFreeBlock(void);

// Args: blockNumber
// Returns: nothing
void returnFreeBlock(int blockNumber);

// Args: none
// Returns: FIB ID, or -1
int getFileInformationBlockID(void);

// Args: fibId
// Returns: nothing
void returnFileInformationBlockID(int fibId);

// Args: none
// Returns: nothing
void printFreeBlocks(void);

// Args: filename
// Returns: file index, or -1
int findFileIndex(const char *filename);

#endif