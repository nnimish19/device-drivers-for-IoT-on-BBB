import sys
import socket
import time
import paho.mqtt.client as mqtt
import xml.etree.ElementTree as ET
import xparser

def on_connect(client, userdata, rc):
    print "Connected with result code " + str(rc)
    for address, topics in xparser.BBBAddr2DepTopics.items():
        for topic in topics:
            client.subscribe(topic)

def on_message(client, userdata, msg):
    print "Callback with topic: " + msg.topic + ", data: " + str(msg.payload)
    for address,topics in xparser.BBBAddr2DepTopics.items():
        if msg.topic in topics:
            dst = (address, 8001)
            userdata.sendto(str(msg.payload), dst)

if __name__ == '__main__':
    xparser.parse(sys.argv[1:])

    # Set the socket of edge
    address = (xparser.Edge_Address, 8001)
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(address)

    # connect to the server
    # client_id = "", clean_session = True, userdata = s (in on_message)
    mqttc = mqtt.Client("", True, s)
    mqttc.on_connect = on_connect
    mqttc.on_message = on_message

    mqttc.connect(xparser.Server_Address)
    mqttc.loop_start() # start a new thread to handle network loop

    print "Start listening to device status change"

    while True:
        data, addr = s.recvfrom(2048)
        if not data:
            continue
        print "Received: ", data, " from ", addr
        if data.find(',') != -1:
            device_id, data = data.split(',')
            topic = xparser.Topics[int(device_id)]
        else:
            topic = xparser.Topics[0]
        mqttc.publish(topic, data)

    s.close()
