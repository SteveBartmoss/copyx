#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "cadena.h"

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