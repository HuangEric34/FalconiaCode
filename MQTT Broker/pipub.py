# python 3.6

import random
import time

from paho.mqtt import client as mqtt_client
import json


broker = 'mqtt.things.ph' #INPUT BROKER NAME
port = 1883
topic = "RaspiMQTT" #INPUT TOPIC NAME

# Generate a Client ID with the publish prefix.
client_id = f'publish-{random.randint(0, 1000)}'

username = '64ca784991ec1ac7a26e35cd' #INPUT USERNAME
password = '09vJQ0RRWiqcFk5l4nMH0Ea5' #INPUT PASSWORD

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def publish(client):
    msg_count = 1
    while True:
        time.sleep(1)
        
        msg = {
            "hardware_serial": "RaspiMQTT", 
            "payload_fields": {
                "switch": 1
            }
        } 
        
        ###############################################
        
        
        result = client.publish(topic, payload=json.dumps(msg),qos=0,retain=False)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
        #msg_count += 1
        #if msg_count > 5:
            #break


def run(): 
    client = connect_mqtt()
    client.loop_start()
    publish(client)
    client.loop_stop()


if __name__ == '__main__':
    run()
