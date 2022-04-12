#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include "gf256_tables.h"
#include "test_system.h"
#include "testRun.h"
#include "system.h"
#include "portable_endian.h"

char *getFileInfo(const char *path, const char *fileRequest, uint32_t *size_filename, uint64_t *size_message){
    uint32_t offset = 0;
    FILE *f = fopen(path, "r");
    struct stat st;
    stat(path, &st);

    while (offset < st.st_size){
        fread(size_filename, 4, 1, f);
        *size_filename = be32toh(*size_filename);
        //printf("Size processed filename %d\n", *size_filename);

        
        fread(size_message, 8, 1, f);
        *size_message = be64toh(*size_message);
        //printf("Size processed message %ld\n", *size_message);

        char *filename = malloc(*size_filename);
        fread(filename, 1, *size_filename, f);

        //printf("File process : %s\n", filename);
        char *message = malloc(*size_message);
        fread(message, 1, *size_message, f);

        //printf("File Loop : %s\n", filename);
        uint8_t isSame = 1;
        for (uint32_t k = 0; k < *size_filename; k++)
        {
            if (*(fileRequest + k) != *(filename + k)){
                isSame = 0;
                break;
            }
        }

        free(filename);
        
        if (isSame){
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

    while (offset < st.st_size){
        uint32_t size_filename;
        fread(&size_filename, 4, 1, f);
        size_filename = be32toh(size_filename);

        uint64_t size_message;
        fread(&size_message, 8, 1, f);
        size_message = be64toh(size_message);
        
        //printf("Filename size : %d\tMessage size : %ld\n", size_filename, size_message);
        char *filename = malloc(size_filename+1);
        fread(filename, 1, size_filename, f);

        //printf("File Request %s\n", filename);
        uint32_t *size_filename_exp = malloc(sizeof(uint32_t));
        uint64_t *size_message_exp = malloc(sizeof(uint64_t));
        char *message_exp = getFileInfo(expectedResPath, filename, size_filename_exp, size_message_exp);


        char *message = malloc(size_message);
        fread(message, 1, size_message, f);

        if ( message_exp != NULL){

            CU_ASSERT_EQUAL(size_message, *size_message_exp);
            //printf("Size filename : %d\tSize expected : %d\n", size_filename, size_filename_exp);
            CU_ASSERT_EQUAL(size_filename, *size_filename_exp);
            //printf("Size message : %ld\tSize expected : %d\n", size_message, size_message_exp);

            
            for (uint32_t i = 0; i < size_message; i++)
            {
                CU_ASSERT_EQUAL(*(message+i), *(message_exp+i));   // compare the characters of output files
            }
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

void addSuiteMessage(){
    printf("Loaded Message !\n");
    CU_pSuite suite = CU_add_suite("message", 0, 0);
    CU_add_test(suite, "test recovered message", test_Message);
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