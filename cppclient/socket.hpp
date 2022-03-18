
//
//  socket.hpp
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <cstring> // memcpy()
#include <arpa/inet.h> // https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
#include <sys/socket.h> // https://pubs.opengroup.org/onlinepubs/007904975/basedefs/sys/socket.h.html e.g. recv()
#include <unistd.h>
#include <string>

namespace serversock 
{

    struct objectData 
    {
        const char *size_info;
        unsigned int value;
    };
    
    void createConnection();
    void close_connection();
    int readValues(objectData *a);
    int read_values();
    int read_values(uint8_t *data, int data_size);
    int send_values(objectData *data);
    int send_values(std::string data);
    int send_values(uint8_t *data, int data_size);
    

} // namespace serversock 
