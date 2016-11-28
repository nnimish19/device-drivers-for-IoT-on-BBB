import socket
import time
import paho.mqtt.client as mqtt

address = ('192.168.1.104', 8000)             #ip,port it keeps listening at
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(address)

mqttc = mqtt.Client("python_pub")
mqttc.connect("test.mosquitto.org", 1883)

print "----python---"
while True:
    #mqttc.publish("LED", data)
    mqttc.publish("Button", '1')
    time.sleep(5)
    mqttc.publish("Button", '0')
    time.sleep(5)

mqttc.loop_forever()
s.close()
