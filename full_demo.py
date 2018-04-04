#!/usr/bin/env python3
import serial
import time

from gamepad.bus import Bus
from gamepad.parser_core import ParserCore

# Taken from the library
states = { 'A': 0, 'B': 0, 'X': 0, 'Y': 0,
           'Back': 0, 'Start': 0, 'Middle': 0,
           'Left': 0, 'Right': 0, 'Up': 0, 'Down': 0,
           'LB': 0, 'RB': 0, 'LT': 0, 'RT': 0,
           'LJ/Button': 0, 'RJ/Button': 0,
           'LJ/Left': 0, 'LJ/Right': 0, 'LJ/Up': 0, 'LJ/Down': 0,
           'RJ/Left': 0, 'RJ/Right': 0, 'RJ/Up': 0, 'RJ/Down': 0,
           'Byte0': 0, 'Byte1': 0, 'Byte2': 0, 'Byte3': 0,
           'Byte4': 0, 'Byte5': 0, 'Byte6': 0, 'Byte7': 0,
           'Byte0/INT': 0, 'Byte1/INT': 0, 'Byte2/INT': 0,
           'Byte3/INT': 0, 'Byte4/INT': 0, 'Byte5/INT': 0,
           'Byte6/INT': 0, 'Byte7/INT': 0}

bus = Bus()
parser_core = ParserCore(bus, states)
parser_core.start() # Starts a new thread for the parser

for i in range(10):
    port = "/dev/ttyACM%d" % i
    try:
        arduino = serial.Serial(port)
        print("Found arduino (or something) on port %s" % port)
        break
    except:
        print("Unable to find Arduino on port %s" % port)


def flushed_write(message):
    arduino.write(message)
    arduino.flush()


while True:
    # Handle the states
    if states['A']:
        print("A button pressed!")
        flushed_write(b'A')
        while(states['A']): pass

    elif states['B']:
        print("B button pressed!")
        flushed_write(b'B')
        while(states['B']): pass

    elif states['X']:
        print("X button pressed!")
        flushed_write(b'X')
        while(states['X']): pass

    elif states['Y']:
        print("Y button pressed!")
        flushed_write(b'Y')
        while(states['Y']): pass

    elif states['Up']:
        print("U button pressed!")
        flushed_write(b'U')
        while(states['Up']): pass
    
    elif states['Down']:
        print("D button pressed!")
        flushed_write(b'D')
        while(states['Down']): pass
    
    elif states['Left']:
        print("L button pressed!")
        flushed_write(b'L')
        while(states['Left']): pass
    
    elif states['Right']:
        print("R button pressed!")
        flushed_write(b'R')
        while(states['Right']): pass
