import socket
import json
import time

HOST = '192.168.4.201'
PORT = 10000

sendSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sendSocket.connect((HOST, PORT))


# data = {}
data = '1'

while True:
    time.sleep(0.1)
    sendSocket.send(data.encode())
