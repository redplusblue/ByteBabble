#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *send_handler(void *socket_desc)
{
    int sockfd = *(int *)socket_desc;
    char buffer[256];

    while (1)
    {
        printf("Client: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        send(sockfd, buffer, strlen(buffer), 0);
    }
}

void *receive_handler(void *socket_desc)
{
    int sockfd = *(int *)socket_desc;
    int n;
    char buffer[256];

    while (1)
    {
        bzero(buffer, 256);
        n = recv(sockfd, buffer, 255, 0);

        if (n > 0)
        {
            printf("Server Said: %s\n", buffer);
        }
        else if (n == 0)
        {
            printf("Server disconnected\n");
            fflush(stdout);
            break;
        }
        else
        {
            error("Error reading from socket");
        }
    }

    close(sockfd);
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection failed");
    }

    pthread_t send_thread, receive_thread;
    pthread_create(&send_thread, NULL, send_handler, (void *)&sockfd);
    pthread_create(&receive_thread, NULL, receive_handler, (void *)&sockfd);

    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    return 0;
}