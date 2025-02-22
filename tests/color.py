#!/usr/local/bin/python3
from sys import stdout, stdin

def color_it():
	cs = 15
	while True:
		cs += 1
		if cs >= 231:
			cs = 16
		yield cs

color = color_it()

for line in stdin:
	stdout.write(f"\033[38;5;{color.__next__()}m")
	stdout.write(line)
	stdout.write("\033[0m")
