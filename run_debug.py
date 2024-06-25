import os

if os.system("python ./build-system/main.py debug") != 0:
	exit (1)
os.system("make -C ../cpe2Example C2ARGS=-Wno-unimplemented")
