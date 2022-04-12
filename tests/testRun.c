#include "test_system.h"
#include "test_tinymt32.h"
#include "test_block.h"
#include "test_message.h"
#include <CUnit/Basic.h>

int main(int argc, char const *argv[])
{
    CU_initialize_registry();
    addSuiteSystem();
    addSuiteTinymt();
    addSuiteBlock();
    addSuiteMessage();

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return 0;
}
