#include "fs_indexed.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileSystem fs;

static BlockNode *makeBlockNode(int blockNumber) {
    BlockNode *newNode = (BlockNode *)malloc(sizeof(BlockNode));
    if (newNode == NULL) {
        return NULL;
    }

    newNode->blockNumber = blockNumber;
    newNode->next = NULL;
    return newNode;
}

static void clearFileEntry(FileInformationBlock *file) {
    int i;

    file->inUse = 0;
    file->fibId = -1;
    file->filename[0] = '\0';
    file->sizeBytes = 0;
    file->indexBlock = -1;
    file->dataBlockCount = 0;

    for (i = 0; i < FS_MAX_DATA_BLOCKS_PER_FILE; i++) {
        file->dataBlocks[i] = -1;
    }
}

static int blocksNeeded(size_t sizeBytes) {
    if (sizeBytes == 0) {
        return 0;
    }

    return (int)((sizeBytes + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE);
}

void destroyFS(void) {
    BlockNode *curr = fs.vcb.freeHead;
    BlockNode *temp;
    int i;

    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }

    fs.vcb.totalBlocks = 0;
    fs.vcb.blockSize = 0;
    fs.vcb.freeBlockCount = 0;
    fs.vcb.fibIdHead = 0;
    fs.vcb.fibIdTail = 0;
    fs.vcb.fibIdCount = 0;
    fs.vcb.freeHead = NULL;
    fs.vcb.freeTail = NULL;

    for (i = 0; i < FS_MAX_FILES; i++) {
        fs.vcb.availableFibIds[i] = 0;
        clearFileEntry(&fs.files[i]);
    }
}

void initFS(void) {
    int i;
    BlockNode *newNode;

    destroyFS();

    fs.vcb.totalBlocks = FS_TOTAL_BLOCKS;
    fs.vcb.blockSize = FS_BLOCK_SIZE;
    fs.vcb.freeBlockCount = 0;
    fs.vcb.fibIdHead = 0;
    fs.vcb.fibIdTail = 0;
    fs.vcb.fibIdCount = FS_MAX_FILES;
    fs.vcb.freeHead = NULL;
    fs.vcb.freeTail = NULL;

    for (i = 0; i < FS_MAX_FILES; i++) {
        fs.vcb.availableFibIds[i] = i;
        clearFileEntry(&fs.files[i]);
    }

    for (i = 0; i < FS_TOTAL_BLOCKS; i++) {
        newNode = makeBlockNode(i);

        if (newNode == NULL) {
            printf("Error: could not initialize file system.\n");
            destroyFS();
            return;
        }

        if (fs.vcb.freeHead == NULL) {
            fs.vcb.freeHead = newNode;
            fs.vcb.freeTail = newNode;
        } else {
            fs.vcb.freeTail->next = newNode;
            fs.vcb.freeTail = newNode;
        }

        fs.vcb.freeBlockCount++;
    }

    printf("File system initialized: %d blocks, %d bytes each.\n",
           fs.vcb.totalBlocks, fs.vcb.blockSize);
}

int allocateFreeBlock(void) {
    BlockNode *temp;
    int blockNumber;

    if (fs.vcb.freeHead == NULL) {
        return -1;
    }

    temp = fs.vcb.freeHead;
    blockNumber = temp->blockNumber;
    fs.vcb.freeHead = fs.vcb.freeHead->next;

    if (fs.vcb.freeHead == NULL) {
        fs.vcb.freeTail = NULL;
    }

    free(temp);
    fs.vcb.freeBlockCount--;

    return blockNumber;
}

void returnFreeBlock(int blockNumber) {
    BlockNode *newNode = makeBlockNode(blockNumber);

    if (newNode == NULL) {
        printf("Error: could not return block %d\n", blockNumber);
        return;
    }

    if (fs.vcb.freeTail == NULL) {
        fs.vcb.freeHead = newNode;
        fs.vcb.freeTail = newNode;
    } else {
        fs.vcb.freeTail->next = newNode;
        fs.vcb.freeTail = newNode;
    }

    fs.vcb.freeBlockCount++;
}

int getFileInformationBlockID(void) {
    int fibId;

    if (fs.vcb.fibIdCount == 0) {
        return -1;
    }

    fibId = fs.vcb.availableFibIds[fs.vcb.fibIdHead];
    fs.vcb.fibIdHead = (fs.vcb.fibIdHead + 1) % FS_MAX_FILES;
    fs.vcb.fibIdCount--;

    return fibId;
}

void returnFileInformationBlockID(int fibId) {
    if (fibId < 0 || fibId >= FS_MAX_FILES) {
        return;
    }

    if (fs.vcb.fibIdCount >= FS_MAX_FILES) {
        return;
    }

    fs.vcb.availableFibIds[fs.vcb.fibIdTail] = fibId;
    fs.vcb.fibIdTail = (fs.vcb.fibIdTail + 1) % FS_MAX_FILES;
    fs.vcb.fibIdCount++;
}

int findFileIndex(const char *filename) {
    int i;

    if (filename == NULL) {
        return -1;
    }

    for (i = 0; i < FS_MAX_FILES; i++) {
        if (fs.files[i].inUse && strcmp(fs.files[i].filename, filename) == 0) {
            return i;
        }
    }

    return -1;
}

int createFile(const char *filename, size_t sizeBytes) {
    int fibId;
    int indexBlock;
    int numDataBlocks;
    int totalNeeded;
    int i;

    if (filename == NULL || filename[0] == '\0') {
        printf("Create failed: invalid filename.\n");
        return -1;
    }

    if (strlen(filename) >= FS_MAX_FILENAME) {
        printf("Create failed: filename too long.\n");
        return -1;
    }

    if (findFileIndex(filename) != -1) {
        printf("Create failed: file already exists.\n");
        return -1;
    }

    if (sizeBytes == 0) {
        printf("Create failed: file size must be greater than 0.\n");
        return -1;
    }

    numDataBlocks = blocksNeeded(sizeBytes);

    if (numDataBlocks > FS_MAX_DATA_BLOCKS_PER_FILE) {
        printf("Create failed: file too large.\n");
        return -1;
    }

    totalNeeded = numDataBlocks + 1;

    if (fs.vcb.fibIdCount == 0) {
        printf("Create failed: max files reached.\n");
        return -1;
    }

    if (fs.vcb.freeBlockCount < totalNeeded) {
        printf("Create failed: not enough free blocks.\n");
        return -1;
    }

    fibId = getFileInformationBlockID();
    if (fibId == -1) {
        printf("Create failed: no free FIB ID.\n");
        return -1;
    }

    indexBlock = allocateFreeBlock();
    if (indexBlock == -1) {
        returnFileInformationBlockID(fibId);
        printf("Create failed: could not get index block.\n");
        return -1;
    }

    clearFileEntry(&fs.files[fibId]);

    fs.files[fibId].fibId = fibId;
    fs.files[fibId].inUse = 1;
    fs.files[fibId].sizeBytes = sizeBytes;
    fs.files[fibId].indexBlock = indexBlock;
    fs.files[fibId].dataBlockCount = numDataBlocks;
    strncpy(fs.files[fibId].filename, filename, FS_MAX_FILENAME - 1);
    fs.files[fibId].filename[FS_MAX_FILENAME - 1] = '\0';

    for (i = 0; i < numDataBlocks; i++) {
        int block = allocateFreeBlock();

        if (block == -1) {
            int j;

            for (j = 0; j < i; j++) {
                returnFreeBlock(fs.files[fibId].dataBlocks[j]);
            }

            returnFreeBlock(indexBlock);
            returnFileInformationBlockID(fibId);
            clearFileEntry(&fs.files[fibId]);

            printf("Create failed: block allocation error.\n");
            return -1;
        }

        fs.files[fibId].dataBlocks[i] = block;
    }

    printf("File created: %s\n", filename);
    return 0;
}

int deleteFile(const char *filename) {
    int index;
    int i;
    int fibId;

    if (filename == NULL || filename[0] == '\0') {
        printf("Delete failed: invalid filename.\n");
        return -1;
    }

    index = findFileIndex(filename);
    if (index == -1) {
        printf("Delete failed: file not found.\n");
        return -1;
    }

    for (i = 0; i < fs.files[index].dataBlockCount; i++) {
        returnFreeBlock(fs.files[index].dataBlocks[i]);
    }

    returnFreeBlock(fs.files[index].indexBlock);

    fibId = fs.files[index].fibId;
    clearFileEntry(&fs.files[index]);
    returnFileInformationBlockID(fibId);

    printf("File deleted: %s\n", filename);
    return 0;
}

void listFiles(void) {
    int i;
    int count = 0;

    for (i = 0; i < FS_MAX_FILES; i++) {
        if (fs.files[i].inUse) {
            count++;
        }
    }

    printf("\nRoot Directory Listing (%d files):\n", count);

    for (i = 0; i < FS_MAX_FILES; i++) {
        if (fs.files[i].inUse) {
            printf("%s | %llu bytes | %d data blocks | FIBID=%d\n",
                   fs.files[i].filename,
                   (unsigned long long)fs.files[i].sizeBytes,
                   fs.files[i].dataBlockCount,
                   fs.files[i].fibId);
        }
    }

    printf("\n");
}

void printFreeBlocks(void) {
    BlockNode *curr = fs.vcb.freeHead;

    printf("Free Blocks: ");
    while (curr != NULL) {
        printf("%d ", curr->blockNumber);
        curr = curr->next;
    }
    printf("\n");

    printf("Total Free Blocks: %d\n\n", fs.vcb.freeBlockCount);
}