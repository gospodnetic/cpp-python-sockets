
//
//  socket.hpp
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

namespace serversock 
{

    struct objectData 
    {
        unsigned int value;
    };
    
    void createConnection();
    int readValues(objectData *a);
    
} // namespace serversock 
