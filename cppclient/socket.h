
//
//  socket.cpp
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

# pragma once

#include "socket.hpp"

#define PORT "8080"
#define IP "127.0.0.1"

using namespace std;
using namespace serversock;

int sockfd, n;
struct sockaddr_in serv_addr;
char buffer[1024];

// https://www.bogotobogo.com/cplusplus/sockets_server_client.php
// https://www.geeksforgeeks.org/socket-programming-cc/

void serversock::createConnection() 
{
    // Create socket (file descriptor - fd).
    // domain=AF_INET, type (tcp=SOCK_STREAM/udp=SOCK_DGRAM), protocol=0
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    } else if (sockfd > 0) {
        cout << "SOCKET OPENED" << endl;
    }
    
    // Create connection to socket.
    // Specifies the address family, usually the constant AF_INET (IPv4)
    serv_addr.sin_family = AF_INET;
    // Specifies the port number and must be used with htons() function that converts the host byte order to network byte order
    serv_addr.sin_port = htons(atoi(PORT));
    // sin_addr = holds the IP address returned by inet_addr() to be used in the socket connection
    inet_pton(AF_INET, IP, &(serv_addr.sin_addr.s_addr));
    cout << "attempting to connect to server" << endl;
    // fd=sockfd, remote_host=serv_addr, addr_length=sizeof(serv_addr)
    int conn_success = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (conn_success < 0) 
    {
        perror("ERROR connecting");
    } else 
    {
        cout << "connection successful" << endl;
    }
}

void serversock::close_connection()
{
    close(sockfd);
    cout << "Connection closed" << endl;
}

int serversock::readValues(objectData *a) 
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
        cout << "Client received buffer with the lenght of: " << n << endl;
        if (n != sizeof(struct objectData)) 
        {
            cout << "Received data not valid!" << endl;
            return 0;
        }
        struct objectData data = *((struct objectData *) buffer);
        *a = *((struct objectData *) buffer);
        cout << "Human readable received buffer value: " << a->value << endl;
    } else 
    {
        //cout << "nothing received" << endl;
    }
    return 0;
}

int serversock::send_values(objectData *data)
{
    // Pack data to buffer.
    memcpy(buffer, data, sizeof(*data));
    // Sent to server.
    n = send(sockfd, buffer, sizeof(struct objectData), 0);
    if (n < 0) // or n != sizeof(struct objectData)?
    {
        cout << "Sent data not valid!" << endl;
        cout << "ERROR writing to socket" << endl;
    }
    else
    {
        cout << "Data sent!" << endl;
    }
        
    return 0;
}
