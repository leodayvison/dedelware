#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        // Parent process
        printf("Ola! eu sou o projeto de Sistemas Operacionais com PID = %d\nNao se preocupe, eu sou inofensivo e nao estou fazendo nada com seu dispositivo :)", getpid());
    }

    
    return 0;
}
