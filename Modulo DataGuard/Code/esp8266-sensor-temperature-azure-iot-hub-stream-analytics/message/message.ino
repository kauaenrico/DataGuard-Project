
#include <ArduinoJson.h>

/*#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>*/

void readMessage(int messageId, char *payload){

  // obtem leitura da umidade
  float h = dht.readHumidity();
  
  // obtem leitura da tempetatura em Celsius
  float t = dht.readTemperature();
  
  // obtem leitura da tempetatura em Fahrenheit
  //float f = dht.readTemperature(true);
  
  // checa se houver alguma falha.
  if (isnan(h) || isnan(t))
  {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  //Para utilizar StaticJsonBuffer nesse exemplo foi necessário atualizar a biblioteca ArduinoJason.h para versão 5.0
  StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["deviceId"] = DEVICE_ID;
  root["messageId"] = messageId;

  // NAN não é valido para json, será setado NULL
  if (std::isnan(t))
  {
      root["temperatureC"] = NULL;
  }
  else
  {
      root["temperatureC"] = t;  
  }

  if (std::isnan(h))
  {
      root["humidity"] = NULL;
  }
  else
  {
      root["humidity"] = h;
  }
   
  root.printTo(payload, MESSAGE_MAX_LEN);
}
