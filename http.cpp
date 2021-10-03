#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include <iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 
#include <pthread.h>


#define PORT 8103
#define BUFSIZE 65536


void* handle_connection(void* p_client_socket){
    int client_socket = *((int*)p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    // char actual_path
    while((bytes_read = read(client_socket,buffer+msgsize,sizeof(buffer)-msgsize-1)) > 0){
        msgsize += bytes_read;
        break;
        // if(msgsize > BUFSIZE-1 || buffer[msgsize-1] == "\n") break;
    }
    buffer[msgsize-1] = 0;

    FILE *fp = fopen("index.html","r");
    if(fp == NULL){
        std::cout << "error";
        close(client_socket);
        return NULL;
    }

    while((bytes_read = fread(buffer,1,BUFSIZE,fp))>0){
        write(client_socket,buffer,bytes_read);
    }

    close(client_socket);
    fclose(fp);
    std::cout << "close conn";
    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
 
    
    std::string hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:1000\n\n";
    
    fd_set readfds;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    while(1){  

        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        pthread_t t;
        int *p_client = (int *)malloc(sizeof(int));
        *p_client = new_socket;
        pthread_create(&t, NULL,handle_connection,p_client);    
    }
    return 0;
}

 