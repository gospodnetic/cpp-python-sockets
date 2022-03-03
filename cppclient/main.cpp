
//
//  main.cpp
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

#include <iostream>
#include "socket.h"
#include "socket.hpp"

using namespace std;
using namespace serversock;

struct serversock::objectData data;

int main(int argc, char *argv[]) 
{
    // Create connection.
    serversock::createConnection();

    // Create our data.
    struct serversock::objectData data_to_send;
    data_to_send.value = 101;
    cout << "Human readable data to send: " << data_to_send.value << endl;

    // Send data to server.
    serversock::send_values(&data_to_send);

    // Read data from server.
    objectData data_to_receive;
    while (1 == 1) 
    {
        serversock::readValues(&data_to_receive);
    }
    // cout << "Human readable received buffer value: " << data_to_receive.value << endl;
    serversock::close_connection();
    return 0;
}
