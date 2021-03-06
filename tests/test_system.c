#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gf256_tables.h"
#include "test_system.h"
#include "testRun.h"
#include "system.h"

void test_add_full_vector(){
    uint32_t n = 1000;   // size of the vector
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *u = malloc(n*sizeof(uint8_t));
    uint8_t *u_copy = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t i = 0; i < n; i++)
    {
        uint8_t r1 = rand() % 256;
        uint8_t r2 = rand() % 256;
        *(v+i) = r1;
        *(u+i) = r2;
        *(u_copy+i) = r2;
        *(exp+i) = r1 ^ r2;
    }

    gf_256_full_add_vector(v, u, n);

    for (uint32_t i = 0; i < n; i++)
    {
        CU_ASSERT_EQUAL(*(exp+i), *(v+i));
        CU_ASSERT_EQUAL(*(u+i), *(u_copy+i));
    }
    
    free(exp);
    free(u);
    free(v);
    free(u_copy);
}

void test_add_full_vector_ret(){
    uint32_t n = 1000;  // size of the vector
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *u = malloc(n*sizeof(uint8_t));
    uint8_t *v_copy = malloc(n*sizeof(uint8_t));
    uint8_t *u_copy = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t i = 0; i < n; i++)
    {
        uint8_t r1 = rand() % 256;
        uint8_t r2 = rand() % 256;
        *(v+i) = r1;
        *(u+i) = r2;
        *(v_copy+i) = r1;
        *(u_copy+i) = r2;
        *(exp+i) = r1 ^ r2;
    }

    uint8_t *res = gf_256_full_add_vector_ret(v, u, n);

    for (uint32_t i = 0; i < n; i++)
    {
        CU_ASSERT_EQUAL(*(res+i), *(exp+i));
        CU_ASSERT_EQUAL(*(v+i), *(v_copy+i));
        CU_ASSERT_EQUAL(*(u+i), *(u_copy+i));
    }
    
    free(exp);
    free(u);
    free(v);
    free(u_copy);
    free(v_copy);
    free(res);
}

void test_mult_full_vector(){
    uint32_t n = 1000;    // size of the vector
    uint32_t m_times = 25;   // test m times with different factors
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t j = 0; j < m_times; j++)
    {
        uint8_t coef = rand() % 256;
        for (uint32_t i = 0; i < n; i++)
        {
            uint8_t r1 = rand() % 256;
            *(v+i) = r1;
            *(exp+i) = gf256_mul_table[r1][coef];
        }

        gf_256_mul_vector(v, coef, n);

        for (uint32_t i = 0; i < n; i++)
        {
            CU_ASSERT_EQUAL(*(exp+i), *(v+i));
        }
    }
    
    free(exp);
    free(v);
}

void test_mult_full_vector_ret(){
    uint32_t n = 1000;    // size of the vector
    uint32_t m_times = 25;   // test m times with different factors
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *v_copy = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t j = 0; j < m_times; j++)
    {
        uint8_t coef = rand() % 256;
        for (uint32_t i = 0; i < n; i++)
        {
            uint8_t r1 = rand() % 256;
            *(v+i) = r1;
            *(v_copy+i) = r1;
            *(exp+i) = gf256_mul_table[r1][coef];
        }

        uint8_t *res = gf_256_mul_vector_ret(v, coef, n);

        for (uint32_t i = 0; i < n; i++)
        {
            CU_ASSERT_EQUAL(*(exp+i), *(res+i));
            CU_ASSERT_EQUAL(*(v_copy+i), *(v+i));
        }

        free(res);
    }
    
    free(exp);
    free(v);
    free(v_copy);
}

void test_inv_full_vector(){
    uint32_t n = 1000;   // size of the vector
    uint32_t m_times = 25;   // test m times with different dividers
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t j = 0; j < m_times; j++)
    {
        uint8_t coef = rand() % 256;
        for (uint32_t i = 0; i < n; i++)
        {
            uint8_t r1 = rand() % 256;
            *(v+i) = r1;
            *(exp+i) = gf256_mul_table[r1][gf256_inv_table[coef]];
        }

        gf_256_inv_vector(v, coef, n);

        for (uint32_t i = 0; i < n; i++)
        {
            CU_ASSERT_EQUAL(*(exp+i), *(v+i));
        }
    }
    
    free(exp);
    free(v);
}

void test_inv_full_vector_ret(){
    uint32_t n = 1000;     // size of the vector
    uint32_t m_times = 25;  // test m times with differents dividers
    srand( time( NULL ) );
    uint8_t *v = malloc(n*sizeof(uint8_t));
    uint8_t *v_copy = malloc(n*sizeof(uint8_t));
    uint8_t *exp = malloc(n*sizeof(uint8_t));
    for (uint32_t j = 0; j < m_times; j++)
    {
        uint8_t coef = rand() % 256;
        for (uint32_t i = 0; i < n; i++)
        {
            uint8_t r1 = rand() % 256;
            *(v+i) = r1;
            *(v_copy+i) = r1;
            *(exp+i) = gf256_mul_table[r1][gf256_inv_table[coef]];
        }

        uint8_t *res = gf_256_inv_vector_ret(v, coef, n);

        for (uint32_t i = 0; i < n; i++)
        {
            CU_ASSERT_EQUAL(*(exp+i), *(res+i));
            CU_ASSERT_EQUAL(*(v_copy+i), *(v+i));
        }

        free(res);
    }
    
    free(exp);
    free(v);
    free(v_copy);
}

void test_random_coeff_generation(){
    int size = 31;
    uint8_t expected_res[] = {
        171, 165, 55, 61, 69, 143, 152, 158, 168, 64, 5, 91, 215, 134, 85, 17, 29, 41, 122, 169, 75, 0, 84, 85, 231, 156, 156, 201, 100, 17, 96, 137, 207, 72, 15, 182, 183, 154, 222, 92, 143, 153, 105, 58, 56, 239, 37, 75, 40, 133, 106, 199, 200, 144, 136, 175, 219, 87, 100, 59, 62, 26, 208, 19, 194, 227, 31, 187, 176, 225, 189, 120, 189, 78, 13, 112, 12, 47, 80, 39, 149, 170, 184, 23, 227, 247, 99, 218, 163, 140, 58, 248, 171, 12, 99, 5, 78, 178, 155, 127, 138, 37, 12, 54, 142, 80, 96, 206, 24, 23, 127, 242, 106, 86, 135, 185, 255, 157, 1, 91, 213, 135, 75, 87, 216, 233, 11, 7, 235, 168, 31, 229, 231, 29, 51, 105, 163, 139, 156, 104, 119, 247, 158, 210, 158, 15, 120, 27, 237, 28, 31, 23, 212, 240, 44, 95, 201, 157, 168, 14, 20, 10, 222, 208, 34, 137, 217, 221, 111, 155, 4, 97, 119, 31, 132, 108, 62, 135, 76, 148, 18, 249, 169, 95, 162, 18, 68, 109, 50, 102, 216, 62, 204, 16, 160, 5, 205, 223, 75, 187, 239, 64, 64, 135, 1, 160, 141, 0, 132, 64, 59, 35, 24, 245, 245, 132, 126, 134, 41, 236, 88, 155, 108, 12, 100, 214, 87, 67, 172, 46, 71, 217, 97, 36, 37, 26, 87, 204, 17, 159, 91, 98, 64, 78, 87, 134, 18, 165, 233, 187, 71, 90, 71, 52, 203, 208, 145, 248, 42, 228, 214, 100, 243, 155, 183, 250, 126, 133, 243, 75, 236, 91, 177, 213, 20, 166, 198, 41, 112, 255, 152, 84, 32, 27, 76, 182, 47, 39, 226, 11, 226, 87, 162, 62, 51, 237, 107, 147, 230, 127, 149, 222, 94, 29, 194, 250, 60, 19, 215, 38, 187, 6, 254, 186, 227, 57, 158, 134, 230, 239, 185, 211, 91, 118, 187, 251, 221, 194, 54, 15, 90, 181, 219, 232, 180, 182, 94, 86, 43, 114, 132, 195, 222, 144, 118, 90, 135, 118, 204, 98, 116, 235, 198, 181, 229, 37, 208, 204, 55, 82, 123, 172, 185, 228, 57, 248, 248, 144, 154, 1, 172, 228, 233, 203, 137, 17, 4, 226, 23, 229, 119, 168, 223, 145, 203, 71, 54, 252, 34, 26, 36, 89, 35, 246, 2, 175, 191, 162, 60, 49, 171, 0, 2, 46, 248, 193, 114, 96, 174, 188, 240, 100, 238, 73, 142, 31, 14, 42, 161, 58, 193, 142, 116, 147, 9, 130, 60, 95, 155, 133, 200, 245, 250, 179, 29, 55, 208, 79, 110, 152, 214, 50, 115, 115, 224, 184, 74, 73, 209, 151, 127, 167, 22, 111, 127, 1, 93, 95, 196, 49, 17, 211, 71, 141, 45, 127, 2, 151, 190, 205, 1, 90, 76, 59, 176, 146, 33, 237, 71, 27, 227, 97, 70, 24, 87, 209, 237, 105, 8, 93, 81, 144, 171, 189, 16, 9, 34, 196, 201, 205, 179, 224, 34, 122, 118, 117, 199, 169, 109, 65, 180, 219, 186, 39, 221, 87, 151, 246, 243, 29, 172, 162, 233, 226, 96, 253, 140, 203, 181, 167, 49, 200, 72, 23, 232, 58, 45, 60, 14, 128, 78, 228, 204, 152, 80, 101, 105, 178, 44, 99, 218, 209, 97, 54, 5, 178, 246, 247, 232, 244, 130, 146, 67, 2, 219, 175, 142, 158, 174, 21, 26, 168, 203, 86, 242, 93, 239, 164, 26, 64, 150, 184, 71, 165, 152, 109, 163, 3, 42, 16, 213, 73, 160, 188, 74, 104, 219, 167, 197, 207, 227, 108, 3, 144, 150, 49, 174, 233, 119, 151, 161, 33, 242, 249, 94, 57, 101, 61, 241, 93, 86, 240, 245, 111, 87, 110, 230, 134, 205, 255, 37, 163, 156, 93, 150, 68, 131, 11, 8, 24, 46, 196, 151, 88, 89, 93, 241, 28, 194, 15, 176, 91, 60, 92, 126, 238, 251, 95, 153, 174, 108, 251, 118, 202, 216, 96, 207, 176, 7, 174, 1, 68, 116, 0, 252, 225, 248, 0, 85, 247, 222, 93, 57, 61, 224, 53, 210, 1, 150, 47, 165, 152, 185, 253, 6, 213, 158, 90, 29, 210, 130, 39, 11, 125, 1, 75, 16, 145, 203, 247, 8, 140, 72, 168, 199, 40, 59, 222, 183, 133, 102, 65, 161, 156, 163, 8, 27, 136, 185, 146, 44, 128, 64, 8, 73, 124, 241, 51, 157, 250, 110, 114, 213, 36, 153, 83, 24, 59, 38, 33, 9, 46, 232, 221, 136, 181, 11, 234, 247, 177, 206, 34, 232, 136, 10, 73, 190, 50, 198, 140, 132, 107, 83, 71, 146, 210, 198, 4, 104, 118, 246, 200, 168, 179, 136, 116, 215, 60, 71, 250, 225, 56, 199, 96, 181, 110, 138, 57, 115, 121, 123, 103, 136, 82, 21, 212, 236, 49, 26, 88, 239, 204, 147, 213, 9, 4, 112, 82, 12, 230, 193, 43, 10, 250, 172, 29, 101, 26, 101, 122, 96, 113, 164, 29, 43, 32, 5, 255, 124, 10, 43, 190, 207, 19, 210, 61, 143, 106, 229, 73, 63, 110, 38, 112, 243, 99, 121, 43, 148, 221, 141, 253, 90, 238, 58, 240, 172, 79, 8, 101, 139, 251, 216, 8, 67, 68, 164, 119, 160, 76, 54, 239, 1, 204, 208, 105, 93, 26, 199, 139, 231, 102, 183, 70, 168, 193, 179, 154, 83, 38, 11, 110, 174, 140, 14, 15, 35, 43, 217, 31, 174, 124, 237, 144, 185, 47, 48, 51, 170, 189, 156, 37, 234, 121, 10, 220, 17, 198, 81, 168, 93, 11, 240, 121, 97, 105, 188, 114, 133, 188, 20, 45, 130, 214, 241, 24, 100, 134, 112, 225, 203, 121, 141, 239, 31, 125, 13, 215, 251, 91, 165, 178, 160, 86, 152, 207, 185, 134, 21, 127, 101, 208, 217, 159, 134, 242, 35, 231, 123, 183, 172, 84, 111, 132, 74, 89, 1, 85, 217, 138, 206, 139, 222, 224, 126, 221, 96, 118, 138, 81
    };

    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, 42);

    uint8_t **mat = gen_coefs(prng, size, size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            CU_ASSERT_EQUAL(*(*(mat+i)+j), expected_res[i*size+j]);
        }
        free(*(mat+i));
    }
    free(mat);
    return;
}

void test_gaussian_reduction_fixed(){
    /*     
    [171  55]       [146 217 107]
    [ 61 143]       [ 42  80 157]

    Sol
    [  1  0  ]      [112 114 111]
    [  0  1  ]      [109 109 105]

    Taken from the python implementation
     */
    uint8_t **A = malloc(2*sizeof(uint8_t *));
    uint8_t A1[] = {171, 55};
    uint8_t A2[] = {61, 143};
    *A = A1;
    *(A+1) = A2;

    uint8_t **B = malloc(2*sizeof(uint8_t *));
    uint8_t B1[] = {146, 217, 107};
    uint8_t B2[] = {42, 80, 157};
    *B = B1;
    *(B+1) = B2;

    uint8_t **expected = malloc(2*sizeof(uint8_t *));
    uint8_t expected1[] = {112, 114, 111};
    uint8_t expected2[] = {109, 109, 105};
    *expected = expected1;
    *(expected+1) = expected2;

    gf_256_gaussian_elimination(A, B, 3, 2);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i==j){
                CU_ASSERT_EQUAL(*(*(A+i)+j), 1);
            }else{
                CU_ASSERT_EQUAL(*(*(A+i)+j), 0);
            }
        }

        for (int j = 0; j < 3; j++)
        {
            CU_ASSERT_EQUAL(*(*(B+i)+j), *(*(expected+i)+j));
        }
        
    }
    
    free(A);
    free(B);
    free(expected);
}

void test_gaussian_reduction_bigger(){
    const int size = 16;   // do not exceed 15/16 => because then random number aren't any more and lines become linearly dependant
    const int seed = 14;

    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);

    uint8_t **A = gen_coefs(prng, size, size);
    uint8_t **B = gen_coefs(prng, size, size);

    gf_256_gaussian_elimination(A, B, size, size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i==j){
                CU_ASSERT_EQUAL(*(*(B+i)+j), 1);
                CU_ASSERT_EQUAL(*(*(A+i)+j), 1);
            }else{
                CU_ASSERT_EQUAL(*(*(B+i)+j), 0);
                CU_ASSERT_EQUAL(*(*(A+i)+j), 0);
            }
        }
    }
    

    for (int i = 0; i < size; i++)
    {
        free(*(A+i));
        free(*(B+i));
    }
    free(A);
    free(B);
}

void addSuiteSystem(){
    printf("Loaded System !\n");
    CU_pSuite suite = CU_add_suite("system", 0, 0);
    CU_add_test(suite, "test full add vector", test_add_full_vector);
    CU_add_test(suite, "test full add vector return", test_add_full_vector_ret);
    CU_add_test(suite, "test full mult vector", test_mult_full_vector);
    CU_add_test(suite, "test full mult vector return", test_mult_full_vector_ret);
    CU_add_test(suite, "test full inv vector", test_inv_full_vector);
    CU_add_test(suite, "test full inv vector return", test_inv_full_vector_ret);
    CU_add_test(suite, "correct_coeffs", test_random_coeff_generation);
    CU_add_test(suite, "Basic gaussian reduction", test_gaussian_reduction_fixed);
    CU_add_test(suite, "Bigger gaussian reduction", test_gaussian_reduction_bigger);
}

#if MULTIPLE
int main(int argc, char const *argv[])
{
    CU_initialize_registry();
    addSuiteSystem();

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return 0;
}
#endif