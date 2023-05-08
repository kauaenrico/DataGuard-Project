# LAB 2: NodeMCU ESP8266 com Sensor de temperatura e umidade DHT11 - Azure IoT Hub, Stream Analytics e Blob Storage

**Objetivo deste Laboratório:**

Conectar a placa NodeMCU v3 Esp8266 para efetuar leitura do sensor de temperatura e umidade DHT11 e integrar ao Azure IOT Hub e aos serviços Stream Analytics e Azure Blob Storage.

**Componentes utilizados:**
+ Device: Placa NodeMCU ESP8266
+ Sensor de temperatura DHT11

## Cenário

![image](https://user-images.githubusercontent.com/42357180/160649317-bd5b9dd5-f8dc-4881-b169-b752374759bd.png)

**Importante:**

Para executar este lab é aconselhável que tenha boa familiaridade com a Azure para as configurações abaixo.

**Configurações Azure:**

1) criar recurso IoT Hub
2) criar novo device
3) criar recurso Stream Analytics
4) criar recurso Blob Storage
5) criar um novo container
6) configurar o Stream analytics (entre IoT Hub e blob storage - container)

## Esquema de ligação do sensor DHT11

![Esquema-NodeMCU-Sensor-temperatura-umidade-DHT11](https://user-images.githubusercontent.com/42357180/161806798-4f3b6a5d-d0e5-41b2-9e44-2768d5592678.png)

+ pino 1 - saída do sinal (obtém informações do sensor)
+ pino 2 - positivo (VCC)
+ pino 3 - negativo (GND)

## Explicação conexão dos pinos na NodeMCU
![20220405_133529](https://user-images.githubusercontent.com/42357180/161809434-06761e12-5262-4b6a-ad3a-319470355076.jpg)

+ Porta digital "D1" está conectado o pino da saída do sinal do sensor
+ Porta GND está ligado o "terra/negativo"
+ Porta 3V (3.5 volts) está ligado a entrada do sinal "positivo" conhecido também como VCC (opera entre 3.5 á 5 volts)

## Plataforma desenvolvimento - Arduino IDE
  + instalar as bibliotecas para comunicação com Azure ioT Hub:
    + AzureIoTHub
    + AzureIoTProtocol_MQTT
    + AzureIoTUtility

  ![image](https://user-images.githubusercontent.com/42357180/160653305-c49cb487-9546-41ec-9b90-fd56b5cb1a18.png)
  
  ![image](https://user-images.githubusercontent.com/42357180/160653744-185c765e-4a9d-423e-bf7b-05c407bde374.png)
  
  Garantir que as bibliotecas acima estejam instaladas.
  
  Antes de executar demais etapas no Arduino é necessário efetuar algumas modificações no ambiente de desenvolvimento (essa etapa evita que tenha erros futuros na execução do projeto): 
  https://github.com/renatotvs/esp8266-azure-iot-hub-stream-analytics/blob/main/leia-me.txt
  
  + **Arquivos:**
  
    + **dht_temperature_AzureIoTHubLabDemo2.ino** -  arquivo principal que efetua a conexão com wifi da rede configurada e executa o envio da mensagem ao IoT Hub.
    + **dht_temperature_AzureIoTHub_config.h** - contém os parâmetros que precisam ser configurados:
      + connection string do "Device" do IoT Hub criado
      + dados de acesso a rede wi-fi
      + pino de comunicação com a placa nodeMCU
      + etc.
    + **iotHubClient.ino** - contém método que dispara a mensagem para Azure IoT Hub.
    + **message.ino** - contém método que efetua leitura do componente configurado no pino da placa que nesse caso estamos usando um potenciômetro como exemplo, poderia ser usado algum outro dispositivo: sensor de temperatura, umidade, etc.
 
## Execução do projeto Arduino com a placa NodeMCU

+ Selecionar a placa e porta para execução:

![image](https://user-images.githubusercontent.com/42357180/161621665-b6b331e4-19ca-43fe-b29d-b581910e0ae0.png)

![image](https://user-images.githubusercontent.com/42357180/161625136-bc8c8da4-156b-438f-936e-4502331a4804.png)


**Certifique-se que o cabo usb esteja conectado:**

![7_nodemcu_connection_with_pc](https://user-images.githubusercontent.com/42357180/161812642-60120370-f9b3-463f-a0f9-e858eaba970e.png)

+ Fazer upload do código para a placa:

![image](https://user-images.githubusercontent.com/42357180/161621162-45d9ade3-512f-4d48-be1c-c4f1680fd001.png)

+ Acompanhamento do processo do upload do código antes de conectar a rede wifi:

![image](https://user-images.githubusercontent.com/42357180/161619292-05cc18f1-c5d9-42ef-a815-e2aa74117f76.png)

+ Visualizar a execução pelo "Serial Monitor" e o momento que a placa é conectado a rede wi-fi:

![image](https://user-images.githubusercontent.com/42357180/161620081-dbbf839b-1983-404a-b7ca-820aab6e48bb.png)

![image](https://user-images.githubusercontent.com/42357180/161623867-e38825aa-97a1-44cb-8af6-736554ef1396.png)

A imagem acima mostra quando o wi-fi foi estabelecido com sucesso e o processo enviou mensagem ao IoT Hub.

## Acompanhamento da integração com IoT Hub

**IoT Hub**

![image](https://user-images.githubusercontent.com/42357180/160660183-64d0b0ab-516d-43a7-9b8f-40228696e629.png)

**Stream Analytics**

No painel abaixo é possível acompanhar em tempo real os eventos processados e integradas ao blob storage.
![image](https://user-images.githubusercontent.com/42357180/160661942-2bf770f0-9d06-4bb5-963c-777f8be36767.png)

**Container Blob Storage**

No container é armazenado um arquivo .json de forma incremental. Sempre que o houver uma nova integração com o IoT Hub, será processado pelo Stream Analytics o envio dos dados ao container do blob storage.
![image](https://user-images.githubusercontent.com/42357180/160661344-7fee93fd-41cd-495b-af26-8c2d49d3a7eb.png)

**Visualização do arquivo json**
![image](https://user-images.githubusercontent.com/42357180/161624415-8867872a-a0f9-4491-aaca-cf18ead62106.png)

**Dados coletados do sensor:**

+ **temperatureC** - temperatura em Celsius.
+ **humidity** - umidade.

*A cada 3 segundos é coletado um novo valor de temperatura e umidade.*

**Conseiderações**

Fique a vontade para mudar o parãmetro de configuração do tempo de 3 segundos ou mesmo incluir novos componentes de medição.
Esse laboratório é apenas uma ideia de como integrar a placa NodeMCU com Azure IoT Hub.
É possível aplicar a diversos cenários da internet das coisas (iot), coleta de sensores diversos, telemetria, etc...
