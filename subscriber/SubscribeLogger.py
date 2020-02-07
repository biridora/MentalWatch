# _*_ coding: utf-8 _*
import paho.mqtt.client as mqttClient
import time
import json
from datetime import datetime

def on_connect(client, userdata, flags, rc):

    if rc == 0:
 
        print("Connected to broker")
 
        global Connected                #Use global variable
        Connected = True                #Signal connection 
 
    else:
 
        print("Connection failed")
 
def on_message(client, userdata, message):
    print("Message received: ")
    payload_str = message.payload
    json_dict = json.loads(payload_str.decode())
    json_data = {"time_sensor": datetime.now().strftime('%Y-%m-%d %H:%M:%S'),"xAccl":json_dict['xAccl'],"yAccl":json_dict['yAccl'],"zAccl":json_dict['zAccl'],"light":json_dict['LIGHT'],"menheraflg":json_dict['menheraflg'],"heart":json_dict['heart'],"Temp":json_dict['Temp'],"Humid":json_dict['Humid']}
    #print(json_data)
    encode_json_data = json.dumps(json_data)
    print(encode_json_data)
    f = open(datetime.now().strftime('%Y-%m-%d')+'.log','a')
    f.write(encode_json_data)
    f.write("\n")
    f.close()

Connected = False   #global variable for the state of the connection
 
broker_address= "hoge.cloudmqtt.com"  #Broker address
port = portNo                      #Broker port
user = "username"                    #Connection username
password = "password"            #Connection password
 
client = mqttClient.Client("Python")               #create new instance
client.username_pw_set(user, password=password)    #set username and password
client.on_connect= on_connect                      #attach function to callback
client.on_message= on_message                      #attach function to callback
 
client.connect(broker_address, port=port)          #connect to broker
 
client.loop_start()        #start the loop
 
while Connected != True:    #Wait for connection
    time.sleep(0.1)
 
client.subscribe("MENTAL")
 
try:
    while True:
        time.sleep(1)
 
except KeyboardInterrupt:
    print("exiting")
    client.disconnect()
    client.loop_stop()