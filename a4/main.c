#include "fs_indexed.h"

int main(void) {
    /*
    Start:
    - 64 free blocks
    - blocks 0 to 63
    - no files
    */
    initFS();
    listFiles();
    printFreeBlocks();

    /*
    alpha.txt, 3072 bytes:
    - 3 data blocks
    - index = 0
    - data = 1, 2, 3
    - free count = 60
    */
    createFile("alpha.txt", 3072);
    listFiles();
    printFreeBlocks();

    /*
    beta.txt, 5120 bytes:
    - 5 data blocks
    - index = 4
    - data = 5, 6, 7, 8, 9
    - free count = 54
    */
    createFile("beta.txt", 5120);
    listFiles();
    printFreeBlocks();

    /*
    delete alpha.txt:
    - return 1, 2, 3, 0
    - free count = 58
    */
    deleteFile("alpha.txt");
    listFiles();
    printFreeBlocks();

    /*
    gamma.txt, 4096 bytes:
    - 4 data blocks
    - index = 10
    - data = 11, 12, 13, 14
    - free count = 53
    */
    createFile("gamma.txt", 4096);
    listFiles();
    printFreeBlocks();

    /*
    delta.txt, 8192 bytes:
    - 8 data blocks
    - index = 15
    - data = 16, 17, 18, 19, 20, 21, 22, 23
    - free count = 44
    */
    createFile("delta.txt", 8192);
    listFiles();
    printFreeBlocks();

    /*
    delete beta.txt:
    - return 5, 6, 7, 8, 9, 4
    - free count = 50
    */
    deleteFile("beta.txt");
    listFiles();
    printFreeBlocks();

    /*
    delete gamma.txt:
    - return 11, 12, 13, 14, 10
    - free count = 55
    */
    deleteFile("gamma.txt");
    listFiles();
    printFreeBlocks();

    /*
    delete delta.txt:
    - return 16, 17, 18, 19, 20, 21, 22, 23, 15
    - free count = 64
    - all files deleted
    */
    deleteFile("delta.txt");
    listFiles();
    printFreeBlocks();

    destroyFS();
    return 0;
}