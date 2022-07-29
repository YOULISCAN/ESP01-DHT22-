import paho.mqtt.client as mqtt

broker = 'xxx.xxx.xxx.xxx'
port = 1883
topic = 'xxx'
client_id = 'python_mqtt'

def on_connect(client, userdata, flags, rc):
    # if rc == 0:
    #     print("Connected to MQTT Broker!")
    # else:
    #     print(f"Failed to connect, return code {rc}\n")
    # client = mqtt.Client(client_id)
    # client.on_connect = on_connect
    # client.connect(broker, port)
    # return client
    global topic
    client.subscribe(topic)

def on_message(client, userdate, msg):
    all_str = str(msg.payload)
    message = "温度:" + all_str[14:18] + "℃  " + "湿度:" + all_str[-9:-4] + "%RH"
    print(msg.topic+" "+message)

mqttc = mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect(broker, port, 60)
mqttc.loop_forever()

