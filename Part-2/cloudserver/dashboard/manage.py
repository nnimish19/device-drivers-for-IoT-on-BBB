#!/usr/bin/env python
import os
import sys
import paho.mqtt.client as mqtt
import dashboard.xmlparser as parser
import urllib2
import dsb.settings as gl

def on_connect(client, userdata, rc):
    topics=parser.parse()
    for topic in topics:
        print topic
        client.subscribe(topic)

def on_message(client, userdata, msg):
    print "on_message called with topic: ", msg.topic + ", message: " + str(msg.payload)
    urllib2.urlopen("http://127.0.0.1:8000/showDashboard/?name="+msg.topic+'&data='+msg.payload)

if __name__ == "__main__":
    topics = parser.parse()
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "dsb.settings")
    gl.SENSOR_DATA = {}
    for topic in topics:
        gl.SENSOR_DATA[topic] = ''
    server_address = "128.227.170.197"
    # connect to the server
    # client_id = "", clean_session = True, userdata = s (in on_message)
    mqttc = mqtt.Client("", True, server_address)
    mqttc.on_connect = on_connect
    mqttc.on_message = on_message

    mqttc.connect(server_address)
    mqttc.loop_start()  # start a new thread to handle network loop

    try:
        from django.core.management import execute_from_command_line
    except ImportError:
        # The above import may fail for some other reason. Ensure that the
        # issue is really that Django is missing to avoid masking other
        # exceptions on Python 2.
        try:
            import django
        except ImportError:
            raise ImportError(
                "Couldn't import Django. Are you sure it's installed and "
                "available on your PYTHONPATH environment variable? Did you "
                "forget to activate a virtual environment?"
            )
        raise
    execute_from_command_line(sys.argv)
