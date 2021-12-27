/*
 *  manager.h
 *  Created on: dez 2021
 *  Version: 1.0
 *  License: MIT
 *
 */

#ifndef MY_SERVICES_H
#define MY_SERVICES_H

// Includes ================================================================================

//Já incluso nas bibliotecas padrões
#include <Arduino.h>
#include <stdio.h>
#include <WiFi.h>

//Necessário baixar para importar 
#include <PubSubClient.h>
#include <ESP32DMASPISlave.h>


// WIFI FUnctions =========================================================================

namespace WIFI 
{
	
	const char * const SSID = "***";
	const char * const PASSWORD = "***";

	void ConnectToWiFi();
}

//RTOS FUnctions =========================================================================
namespace RTOS 
{
    void RTOS_init(void);
    void keepWiFiAlive(void * parameters);
    void MQTTsend(void * parameters);
}

namespace MQTT 
{

	extern WiFiClient espClient; // Cria o objeto espClient
	extern PubSubClient mqttClient; // Instancia o Cliente MQTT passando o objeto espClient

// MQTT Constants ==========================================================================

	const uint32_t MAX_PACKET_SIZE = 65E3; // Máximo valor suportado pelo broker HiveMQ

// MQTT Broker =============================================================================
	
	const char * const BROKER_ADDRESS = "broker.hivemq.com";
	const int mqttPort = 1883;


// MQTT Publishers =========================================================================

	const char * const PUB_IR_TEMPERATURES = "breno/ir/temperatures";


// MQTT Subscribers ========================================================================

	const char * const SUB_TEST = "breno/commands";

//Functions

	void setup(PubSubClient& mqttClient, const char * mqttServer, int mqttPort);
	void reconnect(PubSubClient& mqttClient);
	void PUBtest(PubSubClient& mqttClient, const char* topic);
	void PUBTemperatures(PubSubClient& mqttClient, const char* topic, String & payload);
	void SUBcallback(char* topic, byte* payload, unsigned int length);
}


namespace SPIDMA
{

    static const uint32_t BUFFER_SIZE = 128;
    void set_buffer();
    void setup();

}
















#endif


