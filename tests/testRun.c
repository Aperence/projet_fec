#include "test_system.h"
#include "test_tinymt32.h"
#include "test_block.h"
#include "test_message.h"
#include <CUnit/Basic.h>
#include "main.h"

args_t args;
threads_args_t t_args;

int main(int argc, char const *argv[])
{

    args.input_dir = NULL;
    args.verbose = 0;
    args.nb_threads = 4;
    args.output_stream = NULL;
    CU_initialize_registry();
    // add each suite of test
    addSuiteSystem();
    addSuiteTinymt();
    addSuiteBlock();
    addSuiteMessage();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return 0;
}
