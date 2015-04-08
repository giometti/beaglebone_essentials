#!/usr/bin/env python

from __future__ import print_function
import os
import sys
import getopt
import string
import serial

NAME = os.path.basename(sys.argv[0])

#
# Local functions
#

def err(*objs):
    print(NAME, ": ", *objs, file=sys.stderr)


def client(ser):
	while True:
		# Wait for the user input
		line = raw_input(">> ")

		# Send the data + "\n" to the echo server
		ser.write(line + "\n")

		# Check for string "exit"
		if line == "exit":
			break

		# Wait for the server answer
		line = ser.readline()

		# Print echoed data dropping the "\n"
		print("got: ", line.replace("\n", "")),

def server(ser):
	while True:
		# Wait for the data to be echoed till "\n"
		line = ser.readline()

		# Display just received data but the "\n"
		print("echoing: ", line.replace("\n", "")),

		# Send back the received data as a good-echo(TM) should do
		ser.write(line)

def usage():
        print("usage: ", NAME, " [-h] client|server <serdev>", file=sys.stderr)
        sys.exit(2);

#
# Main
#

try:
        opts, args = getopt.getopt(sys.argv[1:], "h",
                        ["help"])
except getopt.GetoptError, err:
        # Print help information and exit:
        print(str(err), file=sys.stderr)
        usage()

for o, a in opts:
        if o in ("-h", "--help"):
                usage()
        else:
                assert False, "unhandled option"

# Check command line
if len(args) < 2:
	usage()
mode = args[0]
dev = args[1]

# Configure the serial connections
ser = serial.Serial(
	port		= dev,
	baudrate	= 115200,
	bytesize	= 8,
	parity		= 'N',
	stopbits	= 1,
	timeout		= None,
	xonxoff		= 0,
	rtscts		= 0
)

if mode == "client":
	client(ser)
elif mode == "server":
	server(ser)
else:
	print("invalid mode", file=sys.stderr)
	usage()

ser.close()
