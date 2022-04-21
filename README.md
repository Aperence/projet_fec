# Project LEPL1503 : FEC

## Directories organisation

```bash
|
|-- binary_exemple           : contain exemples of binary files with lost datas
|-- expected                 : contains an exemple of expected result used by the tests
|-- headers                  : contain headers of C files
|-- src                      : contain all the C source code files   
|-- tests                    : contains all the CUnit tests
|-- auto.sh                  : automatic launch of CUnit tests (need export LD_LIBRARY_PATH)
|-- main.c                   : main program calling the other files
|-- executionSpeed.sh        : script to test the time taken to run our program
```

## Functions of source files

Each C file is responsible for a different task:

|File       | Function                                         |
|:----------|:------------------------------------------------:|
|[block.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/src/block.c)    | Handle the recovering of lost blocks and the generation of linear system to recover them. Also implements methods to handle blocks (print, free).
|[message.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/src/message.c)  | Handling of opening files and extracting blocks and symbols from them. Also allow the write to the output file.
|[system.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/src/system.c)   | Solving of the linear system to recover the lost symbols, and operations on vectors and matrix
|[my_threads.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/src/my_threads.c)| Definition of the function used by threads and handling the consuming of the shared buffer
|[tinymt32.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/src/tinymt32.c) | Generate random coefficients

## Tests files 

The files in ./tests are all run by the main test file testRun.c. They each implement a function called addSuite*NameTestFile* which is then called by testRun.c to run these tests at once.

|File       | Function                                         |
|:----------|:------------------------------------------------:|
|[test_block.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/tests/test_block.c)    | Tests to check if block has lost symbols (with lost indexes) + check if symbol is lost
|[test_message.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/tests/test_message.c)  | Tests to check if the output file contains the expected content
|[test_system.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/tests/test_system.c)   | Tests for gaussian reduction + vector operations
|[test_tinymt32.c](https://forge.uclouvain.be/p3-2022/group-v/lepl-1503-2022-skeleton-group-v-2/-/blob/main/tests/test_tinymt32.c) | Tests for random generation (already given at start of project)

## Execution Speed test

The file executionSpeed.sh is used to mesure the time taken by our program *fec* to run. To be able to run it, install the `bc` extension for bash using the following command in terminal : 
```bash
$ sudo apt install bc
```

You can then launch the execution time test by launching the following command in this directory : 

```bash
$ ./executionSpeed.sh
```

For example, the average speed we mesured for our program to run was ~ 10.817 ms.

## Known issues

Frequently, when we runned the scripts `auto.sh` or `executionSpeed.sh`, we encountered the following errors:
```bash
`$'make\r': command not found
`$'\r''
```

To fix these issues, try the following commands that worked for us (issues due to the encoding format of the scripts)

```bash
$ sudo apt install dos2unix
$ dos2unix auto.sh
$ dos2unix executionSpeed.sh
```

You should then be able to run these scripts without any problem
