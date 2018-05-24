# -*- coding: UTF-8 -*-
import zmq
import sys

def debug_loop():
    while True:
        pass

reload(sys)
sys.setdefaultencoding('utf8')
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:4723")
with open("sendProblemExample.txt", "r") as f:
    context = f.read()
print context
for i in range(0, 100):
    socket.send_string(context)
    feedbackMSG = socket.recv()
