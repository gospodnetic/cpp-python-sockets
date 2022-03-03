
import socket
import struct

# https://docs.python.org/3/library/socket.html#example
# https://realpython.com/python-sockets/

PORT = 8080 # integer from 1 to 65535, uperuser privileges if the port number is less than 1024
HOST_IP = '127.0.0.1' # standard IPv4 address for the loopback interface
#key = 4 # ???
RECV_BUFFER_SIZE = 1024

# Create socket file descriptor (fd) given address family, socket type and protocol number
# AF_INET -> IPv4, SOCK_STREAM -> TCP, protocol=0
with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as socket1:
    # Bind socket to address.
    socket1.bind((HOST_IP, PORT))
    # Enable a server to accept connections from clients.
    print("starting to listen")
    socket1.listen()
    # Accept a connection. Return: conn (a new socket object usable to send and receive data on the connection),  address - address bound to the socket on the other end of the connection.
    # NOTE:  "connection" is the socket that we use to communicate with the client!
    connection, addr = socket1.accept()
    # Infinite while loop is used to loop over blocking calls to connection.recv()
    with connection:
        print('Connected by', addr)
        while True:
            # Wait for input from client...
            data = connection.recv(RECV_BUFFER_SIZE)
            data_int = int.from_bytes(data, byteorder='little') # NOTE: cpp client written unsigned int in buffer.
            print("Server got:", data_int)
            # ... make computation ...
            print("Server computation...")
            result = data_int + 100
            print("Server result", result)
            # ... send the result to client.
            result_byte_like = struct.pack('I', result)
            connection.sendall(result_byte_like) # NOTE: byte-like object are required!

            # Wait for input from server terminal...
            #t = int(input("value (int): "))
            #assert t >= 0
            #d = struct.pack('I', t)
            # ... send that input to client.
            #connection.sendall(d)
