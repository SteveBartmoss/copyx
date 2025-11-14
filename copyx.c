#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

// Método clásico
int copy_file(const char *src, const char *dst) {
    int in_fd = open(src, O_RDONLY);
    if (in_fd < 0) return -1;

    int out_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        close(in_fd);
        return -1;
    }

    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(in_fd, buffer, sizeof(buffer))) > 0) {
        if (write(out_fd, buffer, bytes) != bytes) {
            close(in_fd);
            close(out_fd);
            return -1;
        }
    }

    close(in_fd);
    close(out_fd);
    return 0;
}

// sendfile()
int copy_file_fast(const char *src, const char *dst) {
    int in_fd = open(src, O_RDONLY);
    int out_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    off_t offset = 0;
    struct stat stat_buf;
    fstat(in_fd, &stat_buf);

    ssize_t result = sendfile(out_fd, in_fd, &offset, stat_buf.st_size);

    close(in_fd);
    close(out_fd);

    return (result == stat_buf.st_size) ? 0 : -1;
}

// copy_file_range()
int copy_file_fast_range(const char *src, const char *dst) {
    int in_fd = open(src, O_RDONLY);
    int out_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    off_t offset_in = 0;
    struct stat st;
    fstat(in_fd, &st);

    ssize_t result = copy_file_range(in_fd, &offset_in, out_fd, NULL, st.st_size, 0);

    close(in_fd);
    close(out_fd);
    return (result == st.st_size) ? 0 : -1;
}

int copy_strategy( const char *src, const char *dst) { 
    int result = copy_file_fast_range(src, dst); 
    if(result == -1 && errno == ENOSYS) { 
        result = copy_file_fast(src, dst); 
        if(result == -1 && errno == ENOSYS) { result = copy_file(src, dst); 
        } 
    } 
    return result; 
}

int copi_recursive(const char *src, const char *dst) {
    
    struct stat info;

    if(stat(src, &info) < 0) return -1;


    if(S_ISREG(info.st_mode)){
        return copy_strategy(src, dst);
    }

    if(S_ISDIR(info.st_mode)){

        mkdir(dst, 0755);

        DIR *dir = opendir(src);

        if(!dir) = return -1;

        struct dirent *entry;
        char srcPath[1024];
        char dstPath[1024];

        while((entry = readdir(dir)) != NULL){

            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
                continue;

            snprintf(srcPath, sizeof(srcPath), "%s/%s", src, entry->d_name);

            snprintf(dstPath, sizeof(dstPath), "%s/%s", dst, entry->d_name);

            copy_recursive(srcPath, dstPath);
        }

        closedir(dir)

        return 0;
        
    }

    return -1;

}

int main(int argc, char *argv[]) { 
    if (argc < 3) { 
        printf("Uso: mycopy <origen> <destino>\n"); return 1; 
    } 
    
    const char *src = argv[1]; 
    const char *dst = argv[2]; 
    int result = copy_strategy(src, dst); 
    if (result == 0) printf("Archivo copiado con éxito.\n"); 
    else printf("Error al copiar el archivo.\n"); return result; 
}

