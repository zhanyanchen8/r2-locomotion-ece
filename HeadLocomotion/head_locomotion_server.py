#!/usr/bin/env python

import select
import serial
import signal
import socket
import struct
import sys
import time
import traceback

sys.path.insert(0, "../../protocol/reference")
import R2Protocol

TCP_IP = '0.0.0.0'
TCP_PORT = 9000
BUFFER_SIZE = 64

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

def signal_handler(signal, frame):
  print("Exiting")
  motors.write(STOP_DATA)
  motors.close()
  sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

time.sleep(2)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setblocking(0)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

print("Listening")

try:
  while True:
    motors.write(STOP_DATA)
    try:
      conn, addr = s.accept()
      print("Accept")
      last_time = time.time()
      data = b""
      while True:
        try:
          r = conn.recv(BUFFER_SIZE)
          if not r:
            print("No data")
            break

          data += r
          if data[-1] == ord('\n'):
            data = data.strip().decode("utf8")
            print(data)
            speeds = data.split(",")
            if len(speeds) >= 2:
              try:
                ls = int(255 * clamp(float(speeds[0]), -1.0, 1.0))
                rs = int(255 * clamp(float(speeds[1]), -1.0, 1.0))
                send_data = R2Protocol.encode(b"BM", struct.pack("4B",
                    dir(ls), clamp(abs(ls), 25, 230),
                    dir(rs), clamp(abs(rs), 25, 230)))
                motors.write(send_data)
              except ValueError:
                motors.write(STOP_DATA)

              if len(speeds) == 3:
                try:
                  hs = int(45 * clamp(float(speeds[2]), -1.0, 1.0)) + 90
                  send_data = R2Protocol.encode(b"HM", struct.pack("3B", hs, 0, 100))
                  motors.write(send_data)
                except ValueError:
                  pass

            data = b""
            last_time = time.time()
        except BlockingIOError:
          curr_time = time.time()
          if curr_time - last_time > 1.0:
            print("Stale data")
            last_time = curr_time
            motors.write(STOP_DATA)

      motors.write(STOP_DATA)
      print("Close")
      conn.close()
    except BlockingIOError:
      pass
except Exception as e:
  motors.write(STOP_DATA)
  print(e)
  traceback.print_exc()
