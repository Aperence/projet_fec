#include "test_system.h"
#include "test_tinymt32.h"
#include <CUnit/Basic.h>

int main(int argc, char const *argv[])
{
    CU_initialize_registry();
    addSuiteSystem();
    addSuiteTinymt();

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    return 0;
}
