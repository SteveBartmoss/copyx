#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: mycopy <origen> <destino>\n");
        return 1;
    }

    const char *src = argv[1];
    const char *dst = argv[2];

    int result = copy_file(src, dst);

    if (result == 0)
        printf("Archivo copiado con éxito.\n");
    else
        printf("Error al copiar el archivo.\n");

    return result;
}

