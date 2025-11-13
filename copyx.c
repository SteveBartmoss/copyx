#include <stdio.h>
#include <stdlib.h>
#include <dfcntl.h>
#include <unistd.h>

int copy_file(const char *src, const char *dst){

    int in_fd = open(src, O_RDONLY);

    if(in_fd < 0) return -1;

    int out_fd = open(dst, O_RDONLY | O_CREAT | O_TRUNC, 0644);

    if(out_fd < 0){
        close(in_fd);
        return -1;
    }

    char buffer[4096];
    


}
