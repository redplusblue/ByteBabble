#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *send_handler(void *socket_desc)
{
    int client_sock = *(int *)socket_desc;
    char response[256];

    while (1)
    {
        printf("Enter a response: ");
        fgets(response, 256, stdin);
        send(client_sock, response, strlen(response), 0);
    }
}

void *receive_handler(void *socket_desc)
{
    int client_sock = *(int *)socket_desc;
    int read_size;
    char client_message[256];

    while ((read_size = recv(client_sock, client_message, 256, 0)) > 0)
    {
        client_message[read_size] = '\0';
        printf("Client Said: %s\n", client_message);
    }

    if (read_size == 0)
    {
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else
    {
        error("Error reading from socket");
    }

    close(client_sock);
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t threads[MAX_CLIENTS * 2];
    int thread_count = 0;
    socklen_t clilen;

    if (argc < 2)
    {
        fprintf(stderr, "Error, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding failed");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            error("Error on accept");
        }

        if (thread_count < MAX_CLIENTS * 2)
        {
            pthread_t send_thread, receive_thread;
            pthread_create(&send_thread, NULL, send_handler, (void *)&newsockfd);
            pthread_create(&receive_thread, NULL, receive_handler, (void *)&newsockfd);
            threads[thread_count++] = send_thread;
            threads[thread_count++] = receive_thread;
        }
        else
        {
            printf("Too many clients, dropping connection\n");
            close(newsockfd);
        }
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    close(sockfd);
    return 0;
}