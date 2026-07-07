#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>
#include <pthread.h>
#include <string.h>

#define PORT 8080

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024] = {0};
    char msg[1024];
    char *argv[3];;

    while(1){
        fgets(msg, sizeof(msg), stdin);
        if(strcmp(msg, "exit\n") == 0){
            printf("Exiting...\n");
            break;
        }
        write(client_socket, msg, strlen(msg));
        read(client_socket, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);
    }
    
    

    
    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        // TO-DO: CRIAR NOVA THREAD PARA CADA NOVA CONEXAO, PARA QUE O SERVIDOR NAO FIQUE BLOQUEADO
        pthread_t thread_id;

    }

    
    read(new_socket, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    
    close(new_socket);
    close(server_fd);

    return 0;
}