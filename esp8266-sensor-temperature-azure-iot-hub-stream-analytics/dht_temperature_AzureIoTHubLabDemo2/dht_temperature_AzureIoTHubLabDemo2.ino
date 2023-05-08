/*
 * LAB: NodeMCU ESP8266 - Azure IoT Hub, Stream Analytics e Blob Storage
 * 
Objetivo: Conectar a placa NodeMCU v3 Esp8266 para integrar ao Azure IOT Hub e aos serviços Stream Analytics e Azure Blob Storage.
 
Componentes utilizados:
- Placa NodeMCU Esp8266
- Sensor temperatura DHT11
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>
#include <ArduinoJson.h>
#include "iothubtransportmqtt.h"
#include "dht_temperature_AzureIoTHub_config.h"
#include "DHT.h"

#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

static bool messagePending = false;
static bool messageSending = true;

static char *connectionString = IOT_CONFIG_CONNECTIONSTRING;
static char *ssid = IOT_CONFIG_WIFI_SSID;
static char *pass = IOT_CONFIG_WIFI_PWD;

static int interval = INTERVAL;

void initTime()
{
    time_t epochTime;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true)
    {
        epochTime = time(NULL);

        if (epochTime == 0)
        {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        }
        else
        {
            Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
            break;
        }
    }
}

void initWiFi()
{
    Serial.println("Conectando-se à rede WiFi...");
    Serial.println();  

    WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connectado com sucesso!");  
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht.begin();

  initTime();
  
  //conecta wi-fi
  initWiFi();

  static IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;

  iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, protocol);
  if(iotHubClientHandle == NULL)
  {
    Serial.println("Falha em IoTHubClient_CreateFromConnectionString.");
    while(1);
  }else{
    Serial.println("IoTHubClient_CreateFromConnectionString Criado"); 
  }
  
}

static int messageCount = 1;
void loop() {
  // put your main code here, to run repeatedly:

  delay(2000);

  /*// get humidity reading
  float h = dht.readHumidity();
  // get temperature reading in Celsius
  float t = dht.readTemperature();
  // get temperature reading in Fahrenheit
  //float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println();*/
  
  if(!messagePending && messageSending){
    char messagePayload[MESSAGE_MAX_LEN];
    readMessage(messageCount, messagePayload);
    SendMessage(iotHubClientHandle, messagePayload);
    //Serial.println("sendMessage Azure IOT");
    messageCount++;
    delay(interval);
  }
  IoTHubClient_LL_DoWork(iotHubClientHandle);
  delay(10);
}
