#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

int _close(int file) {
    return -1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _read(int file, char *ptr, int len) {
    errno = EINVAL;
    return -1;
}

int _write(int file, char *ptr, int len) {
    // Agar tum UART ke through printf karna chahte ho to yahan HAL_UART_Transmit() call kar sakte ho
    return len;
}

caddr_t _sbrk(int incr) {
    extern char _end;   // Linker script me define hota hai
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

void _exit(int status) {
    while (1) { }
}

int _kill(int pid, int sig) {
    return -1;
}

int _getpid(void) {
    return 1;
}
