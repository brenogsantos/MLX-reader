from flask import Flask
import paho.mqtt.client as mqtt
import time

downlink_payload = ""


def on_message(client, userdata, message):
    global downlink_payload
    downlink_payload = str(message.payload.decode("utf-8"))
    print("received message: " , downlink_payload)

mqttBroker ="broker.hivemq.com"

client = mqtt.Client("cliente_br")

client.connect(mqttBroker) 
client.subscribe("breno/ir/temperatures")


app = Flask(__name__)
@app.route("/members")
def members():
    global downlink_payload
    client.loop_start()
    client.on_message=on_message 
    return downlink_payload


if __name__ == "__main__":
    app.run(debug=True)