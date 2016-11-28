import socket
import paho.mqtt.client as mqtt

#Edge: reserve port for BBB communication----------------------------------
address = ('192.168.1.104', 8001)       #Edge ip,port through which it will send
b = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
b.bind(address)

dst = ('192.168.1.101', 8001)           #destination(XINU) ip,port

#Egde: subscribe to MQTT broker---------------------------------------------
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("Button")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print "Topic: ", msg.topic+'\nMessage: ' +str(msg.payload)
    print dst
    b.sendto(str(msg.payload), dst)

#client = mqtt.Client()
#client.on_connect = on_connect
#client.on_message = on_message

#client.connect("test.mosquitto.org", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
#client.loop_forever()

mqttc = mqtt.Client("", True, b)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("test.mosquitto.org", 1883, 60)
mqttc.loop_start()
#--------------------------------------------------------------------------------

print "Start listening to device status change"

while True:
    data, addr = b.recvfrom(2048)
    if not data:
        continue
    print "received: ", data, " from ", addr
    #device, status = data.split(' ')
    #mqttc.publish(device, status)

b.close()
