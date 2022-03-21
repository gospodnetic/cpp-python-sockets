
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

# pragma once

// STL includes.
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <arpa/inet.h> // https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
#include <sys/socket.h> // https://pubs.opengroup.org/onlinepubs/007904975/basedefs/sys/socket.h.html e.g. recv()

#define PORT "8080"
#define IP "127.0.0.1"


// Useful links:
// https://www.bogotobogo.com/cplusplus/sockets_server_client.php
// https://www.geeksforgeeks.org/socket-programming-cc/
// https://stackoverflow.com/questions/3125080/sending-images-over-c-sockets-linux
namespace serversock 
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    struct objectData 
    {
        const char *size_info;
        unsigned int value;
    };

    void createConnection() 
    {
        // Create socket (file descriptor - fd).
        // domain=AF_INET, type (tcp=SOCK_STREAM/udp=SOCK_DGRAM), protocol=0
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
        {
            perror("ERROR opening socket");
            exit(1);
        } else if (sockfd > 0) {
            std::cout << "SOCKET OPENED" << std::endl;
        }
        
        // Create connection to socket.
        // Specifies the address family, usually the constant AF_INET (IPv4)
        serv_addr.sin_family = AF_INET;
        // Specifies the port number and must be used with htons() function that converts the host byte order to network byte order
        serv_addr.sin_port = htons(atoi(PORT));
        // sin_addr = holds the IP address returned by inet_addr() to be used in the socket connection
        inet_pton(AF_INET, IP, &(serv_addr.sin_addr.s_addr));
        std::cout << "Attempting to connect to server..." << std::endl;
        // fd=sockfd, remote_host=serv_addr, addr_length=sizeof(serv_addr)
        int conn_success = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if (conn_success < 0) 
        {
            perror("ERROR connecting");
        } else 
        {
            std::cout << "Connection successful!" << std::endl;
        }
    };

    void close_connection()
    {
        close(sockfd);
        std::cout << "Connection closed" << std::endl;
    }

    int readValues(objectData *a) 
    {    
        fd_set fds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        select(sockfd+1, &fds, NULL, NULL, &tv);
        
        if (FD_ISSET(sockfd, &fds)) 
        {
            // Receive a message from a connection  (connected sockets)
            // ARGS: socket file descriptor, buffer for storing message, length of buffer, flags
            // RETURN: return the length of the message written to the buffer 
            n = recv(sockfd, buffer, sizeof(buffer), 0);
            std::cout << "Client received buffer with the lenght of: " << n << std::endl;
            if (n != sizeof(struct objectData)) 
            {
                std::cout << "Received data not valid!" << std::endl;
                return 0;
            }
            struct objectData data = *((struct objectData *) buffer);
            *a = *((struct objectData *) buffer);
            std::cout << "Human readable received buffer value: " << a->value << std::endl;
        } else 
        {
            //std::cout << "nothing received" << std::endl;
        }
        return 0;
    };

    // Short messages communication.
    int read_values()
    {
        char buffer[1024];
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        std::string str_data(buffer, n);
        std::cout << "Received message from server: " << str_data << std::endl;
        return n;
    };

    int read_values(uint8_t *data, int data_size)
    {
        uint8_t *buffer = new uint8_t[data_size];
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        *data = *((uint8_t *)buffer);
        return n;
    };

    int send_values(objectData *data)
    {
        // Pack data to buffer.
        memcpy(buffer, data, sizeof(*data));
        // Sent to server.
        n = send(sockfd, buffer, sizeof(struct objectData), 0);
        if (n < 0) // or n != sizeof(struct objectData)?
        {
            std::cout << "Sent data not valid!" << std::endl;
            std::cout << "ERROR writing to socket" << std::endl;
        }
        else
        {
            std::cout << "Data sent!" << std::endl;
        }
            
        return 0;
    };

    // Short messages communication.
    int send_values(std::string data)
    {
        char buffer[1024];
        memcpy(buffer, data.c_str(), data.size());
        n = send(sockfd, buffer, data.size(), 0);
        return n;
    };

    // Send a byte buffer of size `data_size`
    int send_values(uint8_t *data, int data_size)
    {
        uint8_t *buffer = new uint8_t[data_size];
        memcpy(buffer, data, data_size);
        n = send(sockfd, buffer, data_size, 0);
        return n;
    };

    void wait_server_response()
    {
        bool server_listen = true;
        while(server_listen)
        {
            int msg_size = read_values();
            if (msg_size > 0)
            {
                server_listen = false;
            }
        }
    };
}
