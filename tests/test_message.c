#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "gf256_tables.h"
#include "test_system.h"
#include "testRun.h"
#include "system.h"
#include "portable_endian.h"

/**
 * @brief Get the File Info and store them
 * 
 * @param path          : path of the output file (typically here ./expected/output.txt)
 * @param fileRequest   : the name of the file we want to get the information
 * @param size_filename : the size of the filename to be expected => we store in the pointer the value expected
 * @param size_message  : the size of the message to be expected => we store in the pointer the value expected
 * @return char* : the message expected to be store for this fileRequest
 */
char *getFileInfo(const char *path, const char *fileRequest, uint32_t *size_filename, uint64_t *size_message){
    uint32_t offset = 0;
    FILE *f = fopen(path, "r");
    struct stat st;
    stat(path, &st);

    while (offset < st.st_size-1){  // while there is still files
        if (fread(size_filename, 4, 1, f) != 1) {CU_ASSERT_TRUE(0); exit(1);}
        *size_filename = be32toh(*size_filename);
        //printf("Size processed filename %d\n", *size_filename);

        
        if(fread(size_message, 8, 1, f) != 1) {CU_ASSERT_TRUE(0); exit(1);}
        *size_message = be64toh(*size_message);
        //printf("Size processed message %ld\n", *size_message);

        char *filename = malloc((*size_filename)+1);
        if (fread(filename, 1, *size_filename, f) != *size_filename) {CU_ASSERT_TRUE(0); exit(1);}
        *(filename + *size_filename) = '\0';

        //printf("File process : %s\n", filename);
        char *message = malloc((*size_message) +1);
        if (fread(message, 1, *size_message, f) != *size_message) {CU_ASSERT_TRUE(0); exit(1);}
        *(message + *size_message) = '\0';

        //printf("File Loop : %s\n", filename);
        uint8_t isSame = strcmp(fileRequest, filename);

        free(filename);
        
        if (isSame == 0){   // we found the file => return the messsage
            fclose(f);
            return message;
        }

        free(message);

        offset = offset + 12 + *size_filename + *size_message;
    }
    fclose(f);
    return NULL;
}

void test_Message(){
    const char *outputFilename = "./here.txt";

    const char *expectedResPath = "./expected/output.txt";

    FILE *f = fopen(outputFilename, "r");

    struct stat st;
    stat(outputFilename, &st);

    uint32_t offset = 0;

    while (offset < st.st_size -1){
        uint32_t size_filename;
        if (fread(&size_filename, 4, 1, f) != 1) {CU_ASSERT_TRUE(0); exit(1);}
        size_filename = be32toh(size_filename);

        uint64_t size_message;
        if (fread(&size_message, 8, 1, f) != 1) {CU_ASSERT_TRUE(0); exit(1);}
        size_message = be64toh(size_message);
        
        //printf("Filename size : %d\tMessage size : %ld\n", size_filename, size_message);
        char *filename = malloc(size_filename+1);
        if (fread(filename, 1, size_filename, f) != size_filename) {CU_ASSERT_TRUE(0); exit(1);}
        *(filename + size_filename) = '\0';

        //printf("File Request %s\n", filename);
        uint32_t *size_filename_exp = malloc(sizeof(uint32_t));
        uint64_t *size_message_exp = malloc(sizeof(uint64_t));
        char *message_exp = getFileInfo(expectedResPath, filename, size_filename_exp, size_message_exp);


        char *message = malloc(size_message+1);
        if (fread(message, 1, size_message, f) != size_message) {CU_ASSERT_TRUE(0); exit(1);}
        *(message+size_message) = '\0';

        if ( message_exp != NULL){

            CU_ASSERT_EQUAL(size_message, *size_message_exp);
            //printf("Size filename : %d\tSize expected : %d\n", size_filename, size_filename_exp);
            CU_ASSERT_EQUAL(size_filename, *size_filename_exp);
            //printf("Size message : %ld\tSize expected : %d\n", size_message, size_message_exp);
            CU_ASSERT_EQUAL(strcmp(message, message_exp), 0);
            
            free(message_exp);
        }else{
            CU_ASSERT_TRUE(0);   // no filename found in expected => shouldn't be here
        }

        free(size_filename_exp);
        free(size_message_exp);
        free(message);
        free(filename);
            
        offset = offset + 12 + size_filename + size_message;
    }
    fclose(f);
}


void test_readdir(){
    
}

// seed = 40
// block_size = 4
// symbol_size = 3
// redundancy = 2

void test_makeBlockList(){
    FILE *expected = fopen("./expected/makeblocklist_expected.txt", "r");

    FILE *f = fopen("./expected/makeblocklist.bin", "r");

    fclose(f);

    fclose(expected);
}

void addSuiteMessage(){
    printf("Loaded Message !\n");
    CU_pSuite suite = CU_add_suite("message", 0, 0);
    CU_add_test(suite, "test recovered message", test_Message);
    CU_add_test(suite, "test readdir", test_readdir);
    CU_add_test(suite, "test make block list", test_makeBlockList);
}

#if MULTIPLE
int main(int argc, char const *argv[])
{
    CU_initialize_registry();
    addSuiteMessage()

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return 0;
}
#endif