all:
	idf.py build
	idf.py --port /dev/ttyACM0 flash monitor
