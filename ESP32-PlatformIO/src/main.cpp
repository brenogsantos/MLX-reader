#include "manager.h"



void setup() {
    Serial.begin(115200);
    WIFI::ConnectToWiFi();
    SPIDMA::setup();
    MQTT::setup(MQTT::mqttClient, MQTT::BROKER_ADDRESS, MQTT::mqttPort);
    RTOS::RTOS_init();



   
}

void loop() {

    MQTT::mqttClient.loop();

}