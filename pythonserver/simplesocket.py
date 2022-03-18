
import socket
import struct
import json

# Useful links:
# https://docs.python.org/3/library/socket.html#example
# https://realpython.com/python-sockets/
# https://stackoverflow.com/questions/42458475/sending-image-over-sockets-only-in-python-image-can-not-be-open

def pack_string(string_in):
    # https://stackoverflow.com/questions/3753589/packing-and-unpacking-variable-length-array-string-using-the-struct-module-in-py
    string_in_bytes = bytes(string_in, 'utf-8')    # or other appropriate encoding
    return struct.pack("I%ds" % (len(string_in_bytes),), len(string_in_bytes), string_in_bytes)

PORT = 8080 # integer from 1 to 65535, uperuser privileges if the port number is less than 1024
HOST_IP = '127.0.0.1' # standard IPv4 address for the loopback interface
RECV_BUFFER_SIZE = 1024

# Create socket file descriptor (fd) given address family, socket type and protocol number
# AF_INET -> IPv4, SOCK_STREAM -> TCP, protocol=0
with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as server_socket:
    # Avoid bind() exception: OSError: [Errno 48] Address already in use
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Bind socket to address.
    server_socket.bind((HOST_IP, PORT))
    # Enable a server to accept connections from clients.
    print("Starting to listen...")
    server_socket.listen()
    # Accept a connection. Return: conn (a new socket object usable to send and receive data on the connection),  address - address bound to the socket on the other end of the connection.
    # NOTE:  "connection" is the socket that we use to communicate with the client!
    # NOTE: we have a communication with only one client, therefore select.select() is not needed.
    connection, client_address = server_socket.accept()
    # Infinite while loop is used to loop over blocking calls to connection.recv()
    # NOTE: again, we use only one connection to one client which can send and recv.
    image_width = 0
    image_height = 0
    image_size = 0
    with connection:
        print("Connected to client:", client_address)
        while True:
            # Read recv() data.
            data = connection.recv(RECV_BUFFER_SIZE)
            bytes_encoding = json.detect_encoding(data)
            try:
                txt = data.decode(bytes_encoding)
            except:
                txt = "IMAGE"
            # Based on prefix decide:
            if txt.startswith("SIZE"):
                tmp = txt.split(" ")
                image_width = int(tmp[1])
                image_height = int(tmp[2])
                image_size = image_width * image_height
                print("Client sent image size:", image_width, image_height)
                connection.sendall(pack_string("GOT SIZE\n"))
                # NOW set new buffer size because we are expecting image in the next recv()!
                RECV_BUFFER_SIZE = image_size
            elif txt.startswith("BYE"):
                print("Client sent 'BYE'. Shutting connection down.")
                connection.shutdown(socket.SHUT_RDWR)
                break
            else:
                # In this case "data" contains image!
                #print(data)
                print("Client sent image!")
                connection.sendall(pack_string("GOT IMAGE\n"))
                # Perform processing on image.
                # TODO...
                print("Server is processing image...")
                print("Image processing done!")
                # Send image back.
                print("Sending image back to the client...")
                connection.sendall(data)
                # NOW return the old buffer size because we can expect "SIZE" or "BYE" in next recv()!
                RECV_BUFFER_SIZE = 1024
                # sock.shutdown() here and new connection start in while loop?

    print("Closing connection.")
    connection.close()

"""
data_int = int.from_bytes(data, byteorder='little') # NOTE: cpp client written unsigned int in buffer.
print("Server got:", data_int)
# ... make computation ...
print("Server computation...")
result = data_int + 100
print("Server result", result)
# ... send the result to client.
result_byte_like = struct.pack('I', result)
connection.sendall(result_byte_like) # NOTE: byte-like object are required!
"""
