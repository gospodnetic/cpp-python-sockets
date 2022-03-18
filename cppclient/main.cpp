
//
//  main.cpp
//
//  Created by Johnathan Chiu on 2/21/19.
//  Copyright © 2019 Johnathan Chiu. All rights reserved.
//

// Application includes.
#include "socket.h"
#include "socket.hpp"

// STL includes.
#include <iostream>
#include <string>


using namespace std;
using namespace serversock;

struct serversock::objectData data;

int main(int argc, char *argv[]) 
{
    // Create connection.
    serversock::createConnection();

    // Send the size of time image.
    int size = 1024 * 1024;
    cout << "Sending image size: " << size << " to server..." << endl;
    string size_message = "SIZE " + to_string(size) + "\n";
    serversock::send_values(size_message);

    // Listen for server response.
    bool server_listen = true;
    while(server_listen)
    {
        int msg_size = serversock::read_values();
        if (msg_size > 0)
        {
            server_listen = false;
        }
    }

    // Send image.
    // TODO.
    cout << "Sending image to server..." << endl;

    // Wait for image from server.
    server_listen = true;
    while(server_listen)
    {
        // TODO: int msg_size = serversock::read_values();
        int msg_size = 1;
        if (msg_size > 0)
        {
            server_listen = false;
            cout << "Got image back from server!" << endl;
        }
    }

    // Send message for closing connection.
    string close_message = "BYE"; // note: this is predefined message know on server side.
    cout << "Sending closing msg: " << close_message << " to server...." << endl;
    serversock::send_values(close_message);

    // Close connection.
    cout << "Closing connection and exiting..." << endl;
    serversock::close_connection();
    return 0;
}
