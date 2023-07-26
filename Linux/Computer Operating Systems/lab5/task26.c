#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    char msg[] = "we are trying something!\n";
    int fd = dup(STDOUT_FILENO);
    write(fd, msg, sizeof(msg));

    FILE *fake_message = fdopen(fd, "w");
    fprintf(fake_message, "Is it work?\n");
    close(fd);

    exit(EXIT_SUCCESS);
}
