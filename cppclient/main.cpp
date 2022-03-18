
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

// STB image.
#include "image.h"

using namespace std;
using namespace serversock;

struct serversock::objectData data;

int main(int argc, char *argv[]) 
{
    // Read image.
    string path = "../client_data/IEEE_spring.png";
    uint8_t* rgb_image;
    int width, height, bpp;
    read_image(path, rgb_image, &width, &height, &bpp);
    cout << "Client read image with resolution: " << width << "x" << height <<" bpp " << bpp << endl;

    // Create connection.
    serversock::createConnection();

    // Send the size of time image.
    int size = width * height; // bpp?
    cout << "Sending image size: " << to_string(width) + "x" + to_string(height) << " to server..." << endl;
    string size_message = "SIZE: " + to_string(width) + " " + to_string(height) + "\n";
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
    cout << "Sending image to server..." << endl;
    int img_size = width * height;
    // TODO.
    string dummy = "DUMMY";
    serversock::send_values(dummy);
    //int n = serversock::send_values(rgb_image, img_size);

    // Wait for server to confirm that image has been received.
    cout << "Waiting for server image confirmation..." << endl;
    server_listen = true;
    while(server_listen)
    {
        int msg_size = serversock::read_values();
        if (msg_size > 0)
        {
            server_listen = false;
        }
    }


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
