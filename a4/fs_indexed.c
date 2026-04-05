#include "fs_indexed.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FileSystem fs;

static BlockNode *createNode(int blockNumber) {
    BlockNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->blockNumber = blockNumber;
    node->next = NULL;
    return node;
}

static void resetFileEntry(FileInformationBlock *file) {
    file->inUse = 0;
    file->fibId = -1;
    file->filename[0] = '\0';
    file->sizeBytes = 0;
    file->indexBlock = -1;
    file->dataBlockCount = 0;

    for (int i = 0; i < FS_MAX_DATA_BLOCKS_PER_FILE; ++i) {
        file->dataBlocks[i] = -1;
    }
}

static int getDataBlockCount(size_t sizeBytes) {
    if (sizeBytes == 0) {
        return 0;
    }

    return (int) ((sizeBytes + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE);
}

void destroyFS(void) {
    BlockNode *current = fs.vcb.freeHead;

    while (current != NULL) {
        BlockNode *next = current->next;
        free(current);
        current = next;
    }

    fs.vcb.totalBlocks = 0;
    fs.vcb.blockSize = 0;
    fs.vcb.freeBlockCount = 0;
    fs.vcb.fibIdHead = 0;
    fs.vcb.fibIdTail = 0;
    fs.vcb.fibIdCount = 0;
    fs.vcb.freeHead = NULL;
    fs.vcb.freeTail = NULL;

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        fs.vcb.availableFibIds[i] = 0;
        resetFileEntry(&fs.files[i]);
    }
}

void initFS(void) {
    BlockNode *node;

    destroyFS();

    fs.vcb.totalBlocks = FS_TOTAL_BLOCKS;
    fs.vcb.blockSize = FS_BLOCK_SIZE;
    fs.vcb.fibIdHead = 0;
    fs.vcb.fibIdTail = 0;
    fs.vcb.fibIdCount = FS_MAX_FILES;

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        fs.vcb.availableFibIds[i] = i;
        resetFileEntry(&fs.files[i]);
    }

    for (int blockNumber = 0; blockNumber < FS_TOTAL_BLOCKS; ++blockNumber) {
        node = createNode(blockNumber);

        if (node == NULL) {
            fprintf(stderr, "Could not initialize the free block list.\n");
            destroyFS();
            return;
        }

        if (fs.vcb.freeTail == NULL) {
            fs.vcb.freeHead = node;
        } else {
            fs.vcb.freeTail->next = node;
        }

        fs.vcb.freeTail = node;
        fs.vcb.freeBlockCount++;
    }

    printf("Filesystem initialized with %d blocks of %d bytes each.\n",
           fs.vcb.totalBlocks,
           fs.vcb.blockSize);
}

int allocateFreeBlock(void) {
    BlockNode *head = fs.vcb.freeHead;
    int blockNumber;

    if (head == NULL) {
        return -1;
    }

    blockNumber = head->blockNumber;
    fs.vcb.freeHead = head->next;

    if (fs.vcb.freeHead == NULL) {
        fs.vcb.freeTail = NULL;
    }

    free(head);
    fs.vcb.freeBlockCount--;
    return blockNumber;
}

void returnFreeBlock(int blockNumber) {
    BlockNode *node = createNode(blockNumber);

    if (node == NULL) {
        fprintf(stderr, "Could not return block %d to the free list.\n", blockNumber);
        return;
    }

    if (fs.vcb.freeTail != NULL) {
        fs.vcb.freeTail->next = node;
    } else {
        fs.vcb.freeHead = node;
    }

    fs.vcb.freeTail = node;
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
    if (fibId < 0 || fibId >= FS_MAX_FILES || fs.vcb.fibIdCount >= FS_MAX_FILES) {
        return;
    }

    fs.vcb.availableFibIds[fs.vcb.fibIdTail] = fibId;
    fs.vcb.fibIdTail = (fs.vcb.fibIdTail + 1) % FS_MAX_FILES;
    fs.vcb.fibIdCount++;
}

int findFileIndex(const char *filename) {
    if (filename == NULL) {
        return -1;
    }

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (fs.files[i].inUse && strcmp(fs.files[i].filename, filename) == 0) {
            return i;
        }
    }

    return -1;
}

int createFile(const char *filename, size_t sizeBytes) {
    int fibId;
    int indexBlock;
    int dataBlockCount;
    int totalBlocksNeeded;
    FileInformationBlock *fileSlot;

    if (filename == NULL || filename[0] == '\0') {
        fprintf(stderr, "Create failed: filename is missing.\n");
        return -1;
    }

    if (strlen(filename) >= FS_MAX_FILENAME) {
        fprintf(stderr, "Create failed: filename '%s' is too long.\n", filename);
        return -1;
    }

    if (findFileIndex(filename) != -1) {
        fprintf(stderr, "Create failed: '%s' already exists.\n", filename);
        return -1;
    }

    dataBlockCount = getDataBlockCount(sizeBytes);

    if (dataBlockCount > FS_MAX_DATA_BLOCKS_PER_FILE) {
        fprintf(stderr, "Create failed: '%s' is too large for one index block.\n", filename);
        return -1;
    }

    totalBlocksNeeded = dataBlockCount + 1;

    if (fs.vcb.fibIdCount == 0) {
        fprintf(stderr, "Create failed: maximum number of files reached.\n");
        return -1;
    }

    if (totalBlocksNeeded > fs.vcb.freeBlockCount) {
        fprintf(stderr, "Create failed: not enough free blocks for '%s'.\n", filename);
        return -1;
    }

    fibId = getFileInformationBlockID();

    if (fibId == -1) {
        fprintf(stderr, "Create failed: could not allocate a FIB ID.\n");
        return -1;
    }

    indexBlock = allocateFreeBlock();

    if (indexBlock == -1) {
        returnFileInformationBlockID(fibId);
        fprintf(stderr, "Create failed: could not allocate the index block.\n");
        return -1;
    }

    fileSlot = &fs.files[fibId];
    resetFileEntry(fileSlot);
    fileSlot->fibId = fibId;
    fileSlot->indexBlock = indexBlock;
    fileSlot->sizeBytes = sizeBytes;
    fileSlot->dataBlockCount = dataBlockCount;
    strncpy(fileSlot->filename, filename, FS_MAX_FILENAME - 1);
    fileSlot->filename[FS_MAX_FILENAME - 1] = '\0';

    /* The sample output assumes the index block is taken first. */
    for (int i = 0; i < dataBlockCount; ++i) {
        int blockNumber = allocateFreeBlock();

        if (blockNumber == -1) {
            for (int j = 0; j < i; ++j) {
                returnFreeBlock(fileSlot->dataBlocks[j]);
            }

            returnFreeBlock(indexBlock);
            returnFileInformationBlockID(fibId);
            resetFileEntry(fileSlot);
            fprintf(stderr, "Create failed: block allocation rolled back for '%s'.\n", filename);
            return -1;
        }

        fileSlot->dataBlocks[i] = blockNumber;
    }

    fileSlot->inUse = 1;

    printf("File '%s' created with %d data blocks + 1 index block.\n",
           fileSlot->filename,
           fileSlot->dataBlockCount);
    return 0;
}

int deleteFile(const char *filename) {
    int fileIndex;
    int fibId;
    int dataBlockCount;
    int indexBlock;
    char deletedName[FS_MAX_FILENAME];

    if (filename == NULL || filename[0] == '\0') {
        fprintf(stderr, "Delete failed: filename is required.\n");
        return -1;
    }

    fileIndex = findFileIndex(filename);

    if (fileIndex == -1) {
        fprintf(stderr, "Delete failed: '%s' was not found.\n", filename);
        return -1;
    }

    fibId = fs.files[fileIndex].fibId;
    dataBlockCount = fs.files[fileIndex].dataBlockCount;
    indexBlock = fs.files[fileIndex].indexBlock;
    strncpy(deletedName, fs.files[fileIndex].filename, FS_MAX_FILENAME - 1);
    deletedName[FS_MAX_FILENAME - 1] = '\0';

    /* Return data blocks first so the free-list order matches the sample. */
    for (int i = 0; i < dataBlockCount; ++i) {
        returnFreeBlock(fs.files[fileIndex].dataBlocks[i]);
    }

    returnFreeBlock(indexBlock);
    returnFileInformationBlockID(fibId);
    resetFileEntry(&fs.files[fileIndex]);

    printf("File '%s' deleted.\n", deletedName);
    return 0;
}

void listFiles(void) {
    int fileCount = 0;

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (fs.files[i].inUse) {
            fileCount++;
        }
    }

    printf("\nRoot Directory Listing (%d files):\n", fileCount);

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (!fs.files[i].inUse) {
            continue;
        }

        printf("  %-10s | %6llu bytes | %2d data blocks | FIBID=%d\n",
               fs.files[i].filename,
               (unsigned long long) fs.files[i].sizeBytes,
               fs.files[i].dataBlockCount,
               fs.files[i].fibId);
    }

    printf("\n");
}

void printFreeBlocks(void) {
    BlockNode *current = fs.vcb.freeHead;

    printf("Free Blocks (%d): ", fs.vcb.freeBlockCount);

    while (current != NULL) {
        printf("[%d] -> ", current->blockNumber);
        current = current->next;
    }

    printf("NULL\n");
}
