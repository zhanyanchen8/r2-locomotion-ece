# -*- coding: utf-8 -*-
"""
Example TeraRanger MultiFlex configuration script
"""
from r2_protocol import *
import sys
import serial
import binascii
import time

def check_distance2():

    port_name = "/dev/ttyACMPort1" #check port name
    multiflex = serial.Serial(port_name, 115200, timeout=10, writeTimeout=5)

    print 'Connected to TeraRanger MultiFlex'
    multiflex.flushInput()
    print('flushOutput')
    multiflex.flushOutput()
    print('flushInput')
    multiflex.write(bytearray([0x00, 0x11, 0x01, 0x45]))

    response = multiflex.read(16)
    response = binascii.hexlify(response)

    if response.find("52451100d4") != -1:
        print 'ACK'

    if response.find("5245110027") != -1:
        print 'NACK'

    sensors_data = []
    mf_str_data = str(multiflex.read(80))
    start_index = mf_str_data.find('M')
    print(start_index)
    end_index = mf_str_data.find('\r', start_index)
    print(end_index)
    mf_str_data = mf_str_data[start_index:end_index]
    print(mf_str_data)
    tab = mf_str_data.find('\t')
    tab2 = 0
    for x in range(0,7):
        tab2 = mf_str_data.find('\t', tab+1)
        sensors_data.append(mf_str_data[tab+1:tab2])
        tab = tab2
        if sensors_data[x] == '-1':
            sensors_data[x] = '5000' #out of range
        print(x+1, sensors_data[x])
        sensors_data[x] = int(sensors_data[x])
    sensors_data.append(mf_str_data[tab2+1:])
    if sensors_data[7] == '-1':
        sensors_data[7] = '5000' #out of range
    sensors_data[7] = int(sensors_data[7])
    print(8, sensors_data[7])
    return sensors_data


    multiflex.close()
    sys.exit(0)
