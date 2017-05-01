import serial
from time import sleep
import R2Protocol

#print(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10001M20001"))
try:
	s = serial.Serial("COM4", 9600, timeout=2)
except serial.SerialException:
	print("No serial port found")
	exit()
	
try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10100M20100"))
except serial.SerialTimeoutException:
	print("Serial port write timed out 1!")
	
sleep(5)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10000M20000"))
except serial.SerialTimeoutException:
	print("Serial port write timed out 2!")
	
sleep(5)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10100M20100"))
except serial.SerialTimeoutException:
	print("Serial port write timed out 3!")
	
sleep(5)

try:
	s.write(R2Protocol.encode(b"NUC", b"MOTOR", b"", b"M10000M20000"))
except serial.SerialTimeoutException:
	print("Serial port write timed out 4!")
