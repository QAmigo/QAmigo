#!/usr/bin/python3

import serial
import time
import sys

bufferHex = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
bufferStr = "abcdefghijklmnopqrstuvwxyz"
bufferSend = bufferStr.encode('utf-8')

if __name__ == '__main__':
	if (len(sys.argv) != 4):
		print("There should be three arguments, [portname] [baudrate] [interval]")
	else:
		baudrate = int(sys.argv[2])
		interval = int(sys.argv[3])
		print("Opening port: %s, baudrate: %d, interval(ms): %d" % (sys.argv[1], baudrate, interval))
		interval = interval / 1000
		try:
			port = serial.Serial(sys.argv[1], baudrate)  # open serial port
		except Exception as e:
			print("Port open failed.")
			print(e)
			exit(-1)
		while True:
			port.write(bufferSend)
			time.sleep(interval)
