/**
  * Developer : Nashid P
  * Description : Simple TCP Socket Client in C
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void help_menu();

// thread function for listening 
void *listen_sock(void *argp);

int sock; 
struct sockaddr_in host;

int main(int argc, char *argv[]) {
    int port;
    pthread_t listen_thread;
    char message[1024];

    if(argc == 3) {
        for(int i=1;i<argc;i++) {
            if(argv[i][0] == '-' && argv[0][1] == 'h')
                help_menu();
        }
        port = atoi(argv[2]);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        if(sock == -1) {
            printf("Could not create socket");
        }

        host.sin_addr.s_addr = inet_addr(argv[1]);
        host.sin_family = AF_INET;
        host.sin_port = htons(port);

        if(connect(sock, (struct sockaddr *)&host, sizeof(host)) < 0) {
            perror("Failed to connect \n");
            return 1;
        }

        printf("Connection Established \n");
        
        // create thread for listening messages
        pthread_create(&listen_thread, NULL, listen_sock, NULL);

        while(1) {
            fgets(message, 1024, stdin);
            
            // send data
            if(send(sock, message, strlen(message), 0) < 0) {
                printf("Failed to send message \n");
                return 1;
            }
        } 
        close(sock);
    } else {
        help_menu();
    }
    pthread_exit(NULL);
    return 0;
}

void help_menu() {
    printf("Usage : netchat [IP] [PORT] \n");
}

void *listen_sock(void *argp) {
    char reply[2048];
    while(1) {
        // receive data 
        if(recv(sock, reply, 2048, 0) < 0) {
            printf("Failed to recieve message \n");
            exit(1);
        }
        printf("%s", reply);
        // reset contents of reply
        memset(reply, 0, 2048);
    }
}
