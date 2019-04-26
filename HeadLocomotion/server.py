import socket

UDP_IP = "0.0.0.0"
UDP_PORT = 9000
MESSAGE = "Hello, World!"

# print "UDP target IP:", UDP_IP
# print "UDP target port:", UDP_PORT
# print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
while True:
    sock.sendto(bytes(MESSAGE, "utf=8"), (UDP_IP, UDP_PORT))
