#!/usr/local/bin/python3
from sys import stdout, stdin, argv, exit

stdout.write("Hello\n")
stdout.write("I am just a simple test message\n")
stdout.write("Were working with testfiles from python\n")
stdout.write("See you soon\n")
for arg in argv:
	stdout.write(arg + "\n")
stdout.close()

exit(3)