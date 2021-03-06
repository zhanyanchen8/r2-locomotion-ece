#!/usr/bin/env python

import select
import serial
import signal
import socket
import struct
import sys
import time
import traceback
import readchar
import sys
import json
import terabee
from threading import Thread

sys.path.insert(0, "../r2d2/protocol/reference")
import R2Protocol2 as R2Protocol

'''comment
HOST = '0.0.0.0'
PORT = 10000
voice_data = -1



class ListenThread(Thread):
    def __init__(self):
        Thread.__init__(self)


    def run(self):
        self.recvSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.recvSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.recvSocket.bind((HOST, PORT))
        self.recvSocket.listen(0)
        conn, addr = self.recvSocket.accept()
        print(conn,addr)
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)
            # data = json.loads(data1.decode('utf-8'))
            if not data:
                break
                print("Here")
            try:
                global voice_data
                voice_data = (data.decode())
            except Exception as e:
                print (e)

thread = ListenThread()
thread.start()

'''
HOST = '0.0.0.0'
PORT = 11000
lidar_data = -1



class ListenThread(Thread):
    def __init__(self):
        Thread.__init__(self)


    def run(self):
        self.recvSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.recvSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.recvSocket.bind((HOST, PORT))
        self.recvSocket.listen(0)
        conn, addr = self.recvSocket.accept()
        print(conn,addr)
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)
            # data = json.loads(data1.decode('utf-8'))
            if not data:
                break
                print("Here")
            try:
                global lidar_data
                lidar_data = (data.decode())
                print(lidar_data)
            except Exception as e:
                print (e)

thread = ListenThread()
thread.start()



#TCP_IP = '0.0.0.0'
#TCP_PORT = 9000
#BUFFER_SIZE = 64

MOVE_DATA = R2Protocol.encode(b"BM", b"\x00a\x00a")
STOP_DATA = R2Protocol.encode(b"BM", b"\x00\x19\x00\x19")

def dir(x):
  return 1 if x < 0 else 0

def clamp(x, l, u):
  if x < l:
    return l
  if x > u:
    return u
  return x

motors = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)
#motors = serial.Serial('/dev/ttyS0', baudrate=115200, timeout=1)

def signal_handler(signal, frame):
  print("Exiting")
  motors.write(STOP_DATA)
  motors.close()
  sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

time.sleep(2)

#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.setblocking(0)
#s.bind((TCP_IP, TCP_PORT))
#s.listen(1)

print("Listening")


ls = int(255 * clamp(float(-50), -1.0, 1.0))
rs = int(255 * clamp(float(50), -1.0, 1.0))
send_data = R2Protocol.encode(b"BM", struct.pack("4B",
    dir(ls), clamp(abs(ls), 25, 230),
    dir(rs), clamp(abs(rs), 25, 230)))
last = time.time()
last_executed = time.time()
interval = 5.0
last_moved = -2

def run(ch, distance): 
    while True:
            #global voice_data
            global lidar_data
            time.sleep(0.1)
            #print (voice_data)
            
            x = 0
            y = 0
            
            #ch = readchar.readchar()
            if ch == 'x':
                print ("exit")
                sys.exit()
            if ch == 'w':
                x = 0 # -1
                y = 0 # -1
            if ch == 's':
                x = 1
                y = 1
            if ch == 'a':
                x = 1
                y = -1
            if ch == 'd':
                x = -1
                y = 1
            
            '''if int(lidar_data) == 1:
                print("stop")
                x = 0
                y = 0
            
            if int(voice_data) == 1 and int(time.time() - last_executed) > int(interval) and last_moved != 1:
                print ('vader')
                x = -1
                y = -1
                last_executed = time.time()
                last_moved = 1
            if int(voice_data) == 2 and time.time() - last_executed > interval and last_moved != 2:
                x = 1
                y = 1
                last_executed = time.time()
                last_moved = 2
            if int(voice_data) == 3 and time.time() - last_executed > interval and last_moved != 3:
                last_moved = 3
                x = 1
                y = -1
                last_executed = time.time()
            if int(voice_data) == 4 and time.time() - last_executed > interval and last_moved != 4:
                last_moved = 4
                x = -1
                y = 1
                last_executed = time.time()

            if time.time() - last > 180:
                print ("exit")
                sys.exit()
                    '''
            motor_command(x, y)
            while move_distance(ch, distance) == True:
                delay = True
            x = 0
            y = 0
            motor_command(x, y) #stop

def motor_command(x, y):
    print(x)
    print(y)
    global motors
    ls = int(255 * clamp(float(x*50), -1.0, 1.0))
    rs = int(255 * clamp(float(y*50), -1.0, 1.0))
    send_data = R2Protocol.encode(b"BM", struct.pack("4B",
        dir(ls), clamp(abs(ls), 25, 230),
        dir(rs), clamp(abs(rs), 25, 230)))

    print (send_data)
    motors.write(send_data)

def move_distance(ch, distance):
    sensors_data = terabee.check_distance()
    #sensors_data2 = terabee2.check_distance2()
    if ch == 'w':
        if sensors_data[0] < 5  or sensors_data[1] < 5 or sensors_data[2] < 5 or sensors_data[7] < 5:
            return False
        '''for i in range(0,8):
            if sensors_data2[i] < 200:
                return False
                '''
        if sensors_data[0] < distance or sensors_data[1] < distance:
            return False
        '''if sensors_data2[1] < distance or sensors_data2[2] < distance or sensors_data2[5] < distance or sensors_data2[6] < distance:
            return False
        
    elif ch == 's':
        if sensors_data[3] < 200 or sensors_data[4] < 200 or sensors_data[5] < 200 or sensors_data[6] < 200:
            return False
        if sensors_data[5] < distance or sensors_data[4] < distance:
            return False
    elif ch == 'd':
        if sensors_data[1] < 200 or sensors_data[2] < 200 or sensors_data[3] < 200 or sensors_data[4] < 200:
            return False
        if sensors_data2[2] < 200 or sensors_data2[3] < 200 or sensors_data2[6] < 200 or sensors_data2[7] < 200:
            return False
        if sensors_data[2] < distance or sensors_data[3] < distance:
            return False
        if sensors_data2[3] < distance or sensors_data2[7] < distance:
            return False
    elif ch == 'a':
        if sensors_data[0] < 200 or sensors_data[7] < 200 or sensors_data[6] < 200 or sensors_data[5] < 200:
            return False
        if sensors_data2[0] < 200 or sensors_data2[1] < 200 or sensors_data2[4] < 200 or sensors_data2[5] < 200:
            return False
        if sensors_data[7] < distance or sensors_data[6] < distance:
            return False
        if sensors_data2[0] < distance or sensors_data2[4] < distance:
            return False
            '''
    return True

run('w', 5)
