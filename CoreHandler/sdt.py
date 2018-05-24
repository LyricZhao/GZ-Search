# -*- coding: UTF-8 -*-

import re
import zmq
import time

class MsgSwap:
    def __init__(self):
        self.port = "4723"
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:%s" % self.port)

    def sendData(self, strData):
        # print "Sending:"
        # print strData
        self.socket.send_string(strData)
        feedbackMSG = self.socket.recv()

ZMQ_Swaper = MsgSwap()

for i in range(0, 1):

    print "sending data:"
    sendContext = u"@GZ_TYPE@0@GZ_TITLE@普京检阅总统警卫团@GZ_URL@http://www.chinanews.com/shipin/2018/05-07/news767569.shtml@GZ_CONTEXT@好的我可真是太帅了词语习近平@GZ_RELATED@@"
    print sendContext
    ZMQ_Swaper.sendData(sendContext)
    print "send ok !"

    time.sleep(1)


