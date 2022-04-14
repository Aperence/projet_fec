#include "test_system.h"
#include "test_tinymt32.h"
#include "test_block.h"
#include "test_message.h"
#include <CUnit/Basic.h>
#include "main.h"

args_t args;

int main(int argc, char const *argv[])
{

    args.input_dir = NULL;
    args.verbose = 0;
    args.nb_threads = 4;
    args.output_stream = NULL;
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
