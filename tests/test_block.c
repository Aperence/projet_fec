#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gf256_tables.h"
#include "test_block.h"
#include "testRun.h"
#include "block.h"
#include "stack.h"

void testSymbolLost(){
    srand( time( NULL ) );
    uint32_t n_times = 1000;
    uint32_t m = 500;
    uint8_t *symbol = malloc(m*sizeof(uint8_t));
    uint32_t exp = 0;
    for (uint32_t i = 0; i < n_times; i++)
    {
        int r = rand() % 10;
        if (r<=1){
            exp = 1;
        }else{
            exp = 0;
        }
        for (int j = 0; j < m; j++)
        {
            if (r<=1){
                *(symbol+j) = 0;
            }else{
                *(symbol+j) = rand() % 256;
            }
        }

        CU_ASSERT_EQUAL(exp, verifySymbolLost(symbol, m));
    }
    free(symbol);
}

void testBlockLost(){
    srand( time( NULL ) );
    int n = 10000;
    int m = 500;
    uint8_t **list_symbol = malloc(n*sizeof(uint8_t*));
    uint8_t *expected = malloc(n*sizeof(uint8_t));
    uint32_t nb_unk = 0;
    for (int i = 0; i < n; i++)
    {
        uint8_t *symbol = malloc(m*sizeof(uint8_t));
        int r = rand() % 10;
        for (int j = 0; j < m; j++)
        {
            if (r<=1){
                *(symbol+j) = 0;
            }else{
                *(symbol+j) = rand() % 256;
            }
        }

        *(list_symbol+i) = symbol;

        if (r<=1){
            nb_unk++;
            *(expected+i) = 1;
        }else{
            *(expected+i) = 0;
        }
        
    }
    block_t block = {n, 0, m, list_symbol};
    uint32_t lost;
    uint8_t *res = verifyBlock(block, &lost);

    for (int i = 0; i < n; i++)
    {
        CU_ASSERT_EQUAL(*(res+i), *(expected + i));
    }
    CU_ASSERT_EQUAL(lost, nb_unk);

    free(res);
    free(expected);
    for (int i = 0; i < n; i++)
    {
        free(*(list_symbol+i));
    }
    free(list_symbol);
}

void addSuiteBlock(){
    printf("Loaded Block !\n");
    CU_pSuite suite = CU_add_suite("Block", 0, 0);
    CU_add_test(suite, "test check symbol lost", testSymbolLost);
    CU_add_test(suite, "Block with lost symbol", testBlockLost);

}

#if MULTIPLE
int main(int argc, char const *argv[])
{
    CU_initialize_registry();
    addSuiteBlock();

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return 0;
}
#endif