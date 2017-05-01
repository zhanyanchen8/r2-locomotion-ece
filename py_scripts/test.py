import serial
from time import sleep
import R2Protocol

#print(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10001M20001"))
try:
	s = serial.Serial("COM3", 9600, timeout=2)
	print("Connected")
except serial.SerialException:
	print("No serial port found")
	exit()
	
try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10100M2-100"))
	print("1")
except serial.SerialTimeoutException:
	print("Serial port write timed out 1!")
	
sleep(2)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10000M20000"))
	print("2")
except serial.SerialTimeoutException:
	print("Serial port write timed out 2!")
	
sleep(1)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10100M20100"))
	print("3")
except serial.SerialTimeoutException:
	print("Serial port write timed out 3!")
	
sleep(2)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10000M20000"))
	print("4")
except serial.SerialTimeoutException:
	print("Serial port write timed out 4!")
