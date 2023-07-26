#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void child_Proc() {
    execl("/usr/bin/cal", "cal", "1410", NULL);
}

int main() {
    pid_t pid;
    if ((pid=fork()) < 0) {
        perror("Fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        child_Proc();
        exit(EXIT_SUCCESS);
    }

    int status;
    wait(&status);

    exit(EXIT_SUCCESS);
}
