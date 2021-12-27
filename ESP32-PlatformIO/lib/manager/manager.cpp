#include "manager.h"


//Defines 
WiFiClient MQTT::espClient; // Cria o objeto espClient
PubSubClient MQTT::mqttClient(espClient); // Instancia o Cliente MQTT passando o objeto espClient
ESP32DMASPI::Slave slave;

uint16_t* spi_slave_tx_buf;
uint16_t* spi_slave_rx_buf;
// Functions Implementations ===============================================================

// WIFI Functions =========================================================================
void WIFI::ConnectToWiFi()
{
 
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI::SSID, WIFI::PASSWORD);
	Serial.print("Connectando a "); Serial.println(WIFI::SSID);
	
	uint8_t i = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		delay(500);
	
		if ((++i % 16) == 0)
		{
		Serial.println(F(" TENTANDO CONECTAR AINDA"));
		}
	}
	
	Serial.print(F("Connectado. Meu IP é: "));
	Serial.println(WiFi.localIP());
}

// RTOS Functions =========================================================================
void RTOS::RTOS_init(void){
    xTaskCreatePinnedToCore(
        RTOS::keepWiFiAlive,
        "keep WiFi Alive",
        5000,
        NULL,
        1,
        NULL,
        0
    );
    xTaskCreatePinnedToCore(
        RTOS::MQTTsend,
        "keep Sleep",
        5000,
        NULL,
        1,
        NULL,
        0
    );
}

void RTOS::keepWiFiAlive(void * parameters){
  for(;;){
    if(WiFi.status() == WL_CONNECTED){
        Serial.print("Wifi ainda conectado, IP: ");
        Serial.println(WiFi.localIP());
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        continue;
    }
    else{
        Serial.println("conectando no Wifi...");
        WIFI::ConnectToWiFi();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
    }

  }
}

void RTOS::MQTTsend(void * parameters)
{
    String payload;
    for(;;)
    {
        if (slave.remained() == 0) slave.queue(spi_slave_rx_buf, spi_slave_tx_buf, SPIDMA::BUFFER_SIZE);
        while (slave.available()) {
            // show received data
            for (size_t i = 0; i < 64; ++i)
            {
                //printf("%d ", spi_slave_rx_buf[i]/256);
                payload.concat(String(spi_slave_rx_buf[i]/256) + "-");

            }
            MQTT::PUBTemperatures(MQTT::mqttClient, MQTT::PUB_IR_TEMPERATURES, payload);
            payload.clear();

            //printf("\n");

            slave.pop();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}


// MQTT Functions =========================================================================
void MQTT::setup(PubSubClient& mqttClient, const char * mqttServer, int mqttPort)
{	
	
	mqttClient.setBufferSize(MQTT::MAX_PACKET_SIZE);
	
	mqttClient.setServer(MQTT::BROKER_ADDRESS, MQTT::mqttPort);
	auto my_lambda = [&](char* topic, byte* payload, unsigned int length) 
    {  //callback
		
		MQTT::SUBcallback(topic, payload, length);
  	};
	static auto static_lambda = my_lambda;
	void(*ptr)(char*, byte*, unsigned int) = [](char* topic, byte* payload, unsigned int length) 
    { 
		return static_lambda(topic, payload, length); 
	}; 

  	mqttClient.setCallback(std::function<void(char*, byte*, unsigned int)>(ptr));
	if (!mqttClient.connected()) MQTT::reconnect(mqttClient);
//	mqttClient.subscribe("");

	mqttClient.loop();

}


void MQTT::reconnect(PubSubClient& mqttClient) 
{
	Serial.println("Conectando ao MQTT Broker...");
  	while (!mqttClient.connected()) 
      {
    	Serial.println("Reconectando ao MQTT Broker..");
      	String clientId = "ESP32Client-";
      	clientId += WiFi.macAddress().c_str();
      
		if (mqttClient.connect(clientId.c_str())) 
        {
    		Serial.println("Connected as " + clientId);
        	// inscreva-se no tópico
        	mqttClient.subscribe("/breno/commands");
      	}      
  	}
}


void MQTT::PUBTemperatures(PubSubClient& mqttClient, const char* topic, String & payload)
{
	
	
	while(!mqttClient.connected()){
    	MQTT::reconnect(mqttClient);
  	}

  	mqttClient.loop();

  	while(!mqttClient.connected()){		
    	MQTT::reconnect(mqttClient);
  	}

  	mqttClient.loop();

    mqttClient.publish(topic, payload.c_str());

}

void MQTT::SUBcallback(char* topic, byte* payload, unsigned int length)
{
//	DynamicJsonDocument doc(1024);
	String message = "";
	Serial.print("Callback - ");
	Serial.print("Message: ");
	for (int i = 0; i < length; i++) 
    {
		//Serial.print((char)payload[i]);
		message.concat((char)payload[i]);
  	}
	Serial.print("message:");
	Serial.println(message);
	String input = message;
//	deserializeJson(doc, input);
//	JsonObject obj = doc.as<JsonObject>();

//	bool status = obj[String("status")];
//	Serial.printf("status: %d\n", status);

	if(!strcmp(topic, MQTT::SUB_TEST))
    {
		//do something
	}

}

// SPIDMA Functions =========================================================================

void SPIDMA::set_buffer()
{
    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
        spi_slave_tx_buf[i] = (0xFF - i) & 0xFF;
    }
    memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
}

void SPIDMA::setup()
{
    spi_slave_tx_buf = slave.allocDMABuffer(BUFFER_SIZE);
    spi_slave_rx_buf = slave.allocDMABuffer(BUFFER_SIZE);

    set_buffer();

    delay(5000);

    slave.setDataMode(SPI_MODE0);
    slave.setMaxTransferSize(BUFFER_SIZE);
    slave.setDMAChannel(2);  // 1 or 2 only
    slave.setQueueSize(1);   // transaction queue size
    slave.begin(VSPI,18, 19, 23, 5);  // default SPI is HSPI

}