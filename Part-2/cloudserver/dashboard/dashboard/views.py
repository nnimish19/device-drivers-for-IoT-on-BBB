from django.shortcuts import render_to_response
import paho.mqtt.client as mqtt
from django.http import HttpResponse
import dsb.settings as gl

# Create your views here.
def showDashboard(request):
    sid = request.GET.get('name', '')
    data = request.GET.get('data','')
    if sid != '':
        gl.SENSOR_DATA[sid] = data
    return render_to_response('../templates/db.html',{'sensors': gl.SENSOR_DATA})

def publishButton1(request):
    # connect to the server
    # client_id = "", clean_session = True, userdata = s (in on_message)
    mqttc = mqtt.Client("", True)

    mqttc.connect("128.227.170.197")
    mqttc.loop_start()
    mqttc.publish('Button', '1')
    return HttpResponse('');

def publishButton2(request):
    # connect to the server
    # client_id = "", clean_session = True, userdata = s (in on_message)
    mqttc = mqtt.Client("", True)

    mqttc.connect("128.227.170.197")
    mqttc.loop_start()
    mqttc.publish('Button', '2')
    return HttpResponse('');

