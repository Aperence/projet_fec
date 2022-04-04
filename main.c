#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include "./headers/tinymt32.h"
#include "message.h"

typedef struct args
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

args_t args;

char **filenames;   //list of filenames from which threads will consume
uint32_t nextFile = 0;   // index of next file to be processed
uint32_t numberFiles;

void usage(char *prog_name)
{
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [OPTIONS] input_dir\n", prog_name);
    fprintf(stderr, "    input_dir: path to the directory containing the instance files with the encoded messages\n");
    fprintf(stderr, "    -f output_file: path to the output file containing all decoded messages\n");
    fprintf(stderr, "    -n n_threads (default: 4): set the number of computing threads that will be used to execute the RLC algorithm\n");
    fprintf(stderr, "    -v : enable debugging messages. If not set, no such messages will be displayed (except error messages on failure)\n");
}

int parse_args(args_t *args, int argc, char *argv[])
{
    memset(args, 0, sizeof(args_t));

    // Default values of the arguments
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    int opt;
    while ((opt = getopt(argc, argv, "n:vf:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            args->nb_threads = atoi(optarg);
            if (args->nb_threads == 0)
            {
                fprintf(stderr, "The number of computing threads must be a positive integer, got: %s\n", optarg);
                return -1;
            }
            break;
        case 'v':
            args->verbose = true;
            break;
        case 'f':
            args->output_stream = fopen(optarg, "w");
            if (args->output_stream == NULL)
            {
                fprintf(stderr, "Impossible to open the output file %s: %s\n", optarg, strerror(errno));
                return -1;
            }
            break;
        case '?':
            usage(argv[0]);
            return 1;
        default:
            usage(argv[0]);
        }
    }

    if (optind == argc)
    {
        fprintf(stderr, "You must provide an input directory containing the instance files!\n");
        return -1;
    }

    // Source: https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
    if (NULL == (args->input_dir = opendir(argv[optind])))
    {
        fprintf(stderr, "Impossible to open the directory containing the input instance files %s: %s\n", argv[optind], strerror(errno));
        return -1;
    }
    // The following line is not very secure... Ask Prof. Legay and/or wait for LINGI2144 for more information :-)
    strcpy(args->input_dir_path, argv[optind++]);

    return 0;
}

int main(int argc, char *argv[])
{
    int err = parse_args(&args, argc, argv);

    if (err == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (err == 1)
    {
        exit(EXIT_SUCCESS);
    }

    filenames = readDir(args.input_dir, args.input_dir_path, &numberFiles);

    // threads here
    while (nextFile < numberFiles)
    {
        
        char *path = malloc(PATH_MAX);
        strcpy(path, args.input_dir_path);
        strcat(path, "/");
        strcat(path, *(filenames + nextFile));
        if (args.verbose){
            printf(">>>>>> filename : %s\n", *(filenames + nextFile));
            printf(">>>>>> path     : %s\n", path);
        }
        message_t *message = openFile(path);
        processBlock(message->listBlock, message->numberBlocks, message->seed, message->size_redundance, message->size_symbol);
        writeToFile(args.output_stream, message, *(filenames+nextFile));
        free(*(filenames + nextFile));
        nextFile++;
    }
    
    free(filenames);
    
    /**
    // Close the input directory and the output file
    err = closedir(args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }*/
    if (args.output_stream != stdout)
    {
        fclose(args.output_stream);
    }
    return 0;
}