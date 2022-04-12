#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>


/**
 * @brief Take the next free file that is not blocked by a semaphore and check that we do not exceed the number of files
 */

uint32_t getNextFile();

/**
 * @brief Function used by the threads to process the files. Each threads consume the filenames in the buffer from t_args.filenames
 *
 * @param args_file : always NULL in our code
 */

void *processFile(void *args_file);

#endif /* THREADS_H */