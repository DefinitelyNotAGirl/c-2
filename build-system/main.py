#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import os
import sys

target = "debug"

#+  ██████  ███████ ████████      ██████ ██      ██      █████  ██████   ██████  ███████
#+ ██       ██         ██        ██      ██      ██     ██   ██ ██   ██ ██       ██
#+ ██   ███ █████      ██        ██      ██      ██     ███████ ██████  ██   ███ ███████
#+ ██    ██ ██         ██        ██      ██      ██     ██   ██ ██   ██ ██    ██      ██
#+  ██████  ███████    ██         ██████ ███████ ██     ██   ██ ██   ██  ██████  ███████
if sys.argv.__len__() == 2:
	#check for other options
	#else:
	target = sys.argv[1]

#+ ██████  ██    ██ ███    ██     ████████  █████  ██████   ██████  ███████ ████████
#+ ██   ██ ██    ██ ████   ██        ██    ██   ██ ██   ██ ██       ██         ██
#+ ██████  ██    ██ ██ ██  ██        ██    ███████ ██████  ██   ███ █████      ██
#+ ██   ██ ██    ██ ██  ██ ██        ██    ██   ██ ██   ██ ██    ██ ██         ██
#+ ██   ██  ██████  ██   ████        ██    ██   ██ ██   ██  ██████  ███████    ██
import targets.debug
import targets.release
import targets.clean

if(target == "debug"):
	exit (targets.debug.run())
elif(target == "release"):
	exit (targets.release.run())
elif(target == "clean"):
	exit (targets.clean.run())
else:
	print("\033[31mERROR:\033[0m target: \"",target,"\" not found",sep = '')
	exit (1)
