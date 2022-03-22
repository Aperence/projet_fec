#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <dirent.h>


int OpenReadDir(const char *directoryname){
    DIR *folder;
    struct dirent *entry;
    int files = 0;
    DIR* FD;
    folder = opendir(directoryname);

    if(folder == NULL){
        puts("Unable to read directory");
        return 1;
    }

    while (entry =readdir(folder)){
        files ++;
        fprintf("File %3d", files);
    }
    closedir(folder);

    return 0;
}


/*
return -1 if file can't be openned

*/
int openFile(const char *filename){
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        return -1;
    }
    
    
    
}
