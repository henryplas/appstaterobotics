#! /usr/bin/env python

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)
GPIO.setup(27, GPIO.OUT)
GPIO.setup(22, GPIO.OUT)
import rospy
from sensor_msgs.msg import LaserScan
import time
import threading

turn_left = False


def callback(msg):
	global turn_left
	for x in range (165, 195):
		if msg.ranges[x] != 0 and msg.ranges[x] <  1:
			left()
			break
	else:
		forward()

def left():
	print('left')
	GPIO.output(17, 1)
	GPIO.output(27, 1)
	GPIO.output(22, 0)

def forward():
	print('forward')
	GPIO.output(17, 1)
	GPIO.output(27, 0)
	GPIO.output(22, 0)
 
def drive():
	print('start driving')
	for i in range(60):
		if(turn_left):
			print('turn left')
		else:
			GPIO.output(17, 1)
			GPIO.output(27, 0)
			GPIO.output(22, 0)
			print('move forward')
		time.sleep(1)
	print('exit')

	#if msg.ranges[180] != 0 and msg.ranges[180] < 0.5:
	#	GPIO.output(16, 0)
	#	GPIO.output(20, 0)
	#	GPIO.output(21, 0)
	#	print msg.ranges[180]                                                                          
	#else:
	#	print '0'
	#	GPIO.output(16, 1)
	#	GPIO.output(20, 0)
	#	GPIO.output(21, 0)

#threading.Thread(target=drive).start()
rospy.init_node('scan_values')
sub = rospy.Subscriber('/scan', LaserScan, callback)
rospy.spin()

