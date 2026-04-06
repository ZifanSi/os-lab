# A4
## Group Members
- Zifan Si
- Stanislav Serbezov

## Build
`make`

## Run
`./fs_sim`

## CLean
`make clean`

## Files
- fs_indexed.h
- fs_indexed.c
- main.c
- Makefile
- Readme

## Extra File
- workflow.puml


## Output

frank@ubuntu:~/Desktop/3sh3$ cd a4
frank@ubuntu:~/Desktop/3sh3/a4$ make clean
rm -f fs_sim
frank@ubuntu:~/Desktop/3sh3/a4$ make
gcc -Wall -Wextra -std=c11 main.c fs_indexed.c -o fs_sim
frank@ubuntu:~/Desktop/3sh3/a4$ ./fs_sim
Filesystem initialized with 64 blocks of 1024 bytes each.

Root Directory Listing (0 files):

Free Blocks (64): [0] -> [1] -> [2] -> [3] -> [4] -> [5] -> [6] -> [7] -> [8] -> [9] -> [10] -> [11] -> [12] -> [13] -> [14] -> [15] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> NULL
File 'alpha.txt' created with 3 data blocks + 1 index block.

Root Directory Listing (1 files):
  alpha.txt  |   3072 bytes |  3 data blocks | FIBID=0

Free Blocks (60): [4] -> [5] -> [6] -> [7] -> [8] -> [9] -> [10] -> [11] -> [12] -> [13] -> [14] -> [15] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> NULL
File 'beta.txt' created with 5 data blocks + 1 index block.

Root Directory Listing (2 files):
  alpha.txt  |   3072 bytes |  3 data blocks | FIBID=0
  beta.txt   |   5120 bytes |  5 data blocks | FIBID=1

Free Blocks (54): [10] -> [11] -> [12] -> [13] -> [14] -> [15] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> NULL
File 'alpha.txt' deleted.

Root Directory Listing (1 files):
  beta.txt   |   5120 bytes |  5 data blocks | FIBID=1

Free Blocks (58): [10] -> [11] -> [12] -> [13] -> [14] -> [15] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> NULL
File 'gamma.txt' created with 4 data blocks + 1 index block.

Root Directory Listing (2 files):
  beta.txt   |   5120 bytes |  5 data blocks | FIBID=1
  gamma.txt  |   4096 bytes |  4 data blocks | FIBID=2

Free Blocks (53): [15] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> NULL
File 'delta.txt' created with 8 data blocks + 1 index block.

Root Directory Listing (3 files):
  beta.txt   |   5120 bytes |  5 data blocks | FIBID=1
  gamma.txt  |   4096 bytes |  4 data blocks | FIBID=2
  delta.txt  |   8192 bytes |  8 data blocks | FIBID=3

Free Blocks (44): [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> NULL
File 'beta.txt' deleted.

Root Directory Listing (2 files):
  gamma.txt  |   4096 bytes |  4 data blocks | FIBID=2
  delta.txt  |   8192 bytes |  8 data blocks | FIBID=3

Free Blocks (50): [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> [5] -> [6] -> [7] -> [8] -> [9] -> [4] -> NULL
File 'gamma.txt' deleted.

Root Directory Listing (1 files):
  delta.txt  |   8192 bytes |  8 data blocks | FIBID=3

Free Blocks (55): [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> [5] -> [6] -> [7] -> [8] -> [9] -> [4] -> [11] -> [12] -> [13] -> [14] -> [10] -> NULL
File 'delta.txt' deleted.

Root Directory Listing (0 files):

Free Blocks (64): [24] -> [25] -> [26] -> [27] -> [28] -> [29] -> [30] -> [31] -> [32] -> [33] -> [34] -> [35] -> [36] -> [37] -> [38] -> [39] -> [40] -> [41] -> [42] -> [43] -> [44] -> [45] -> [46] -> [47] -> [48] -> [49] -> [50] -> [51] -> [52] -> [53] -> [54] -> [55] -> [56] -> [57] -> [58] -> [59] -> [60] -> [61] -> [62] -> [63] -> [1] -> [2] -> [3] -> [0] -> [5] -> [6] -> [7] -> [8] -> [9] -> [4] -> [11] -> [12] -> [13] -> [14] -> [10] -> [16] -> [17] -> [18] -> [19] -> [20] -> [21] -> [22] -> [23] -> [15] -> NULL
frank@ubuntu:~/Desktop/3sh3/a4$ 



## UML WORLFLOW
@startuml
start

:ID1: Initialize file system;
:ID1: Show empty directory and free blocks;

:ID2: Create alpha.txt;
:ID2: Allocate index and data blocks;
:ID2: List files and print free blocks;

:ID3: Create beta.txt;
:ID3: Allocate index and data blocks;
:ID3: List files and print free blocks;

:ID4: Delete alpha.txt;
:ID4: Return data blocks and index block;
:ID4: List files and print free blocks;

:ID5: Create gamma.txt;
:ID5: Allocate index and data blocks;
:ID5: List files and print free blocks;

:ID6: Create delta.txt;
:ID6: Allocate index and data blocks;
:ID6: List files and print free blocks;

:ID7: Delete beta.txt;
:ID7: Return file blocks and FIB ID;
:ID7: List files and print free blocks;

:ID8: Delete gamma.txt;
:ID8: Return file blocks and FIB ID;
:ID8: List files and print free blocks;

:ID9: Delete delta.txt;
:ID9: Show final directory and free blocks;
stop
@enduml




## Individual Contributions
Both members worked together on the code, tests, and debugging.