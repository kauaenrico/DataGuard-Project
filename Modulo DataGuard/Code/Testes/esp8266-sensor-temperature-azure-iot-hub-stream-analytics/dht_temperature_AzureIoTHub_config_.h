
#define DEVICE_ID "NodeMCU" //Device ID cadastrado no Azure IoT Hub

#define IOT_CONFIG_WIFI_SSID "" //nome da rede wifi

#define IOT_CONFIG_WIFI_PWD "" //senha da rede wifi

#define IOT_CONFIG_CONNECTIONSTRING "HostName=******;DeviceId=******;SharedAccessKey=******" //Primary Connection String do device configurado no Azure IoT Hub

#define INTERVAL 3000 //define tempo em milissegundos para envio da mensagem ao IoT Hub

#define PIN_NODEMCU 5 //configura numero do pino da placa NodeMCU onde o fio de comunicação do sensor de tempeartura foi conectado.
// No nosso Lab conectamos no pino de identificação da placa "D1" no qual sua representação na documentação da pinagem da placa é GPIO5 que é o número 5 que configuramos. 
// Qualquer dúvida sobre esse assunto pesquise: "pinagem da placa NodeMCU".

#define MESSAGE_MAX_LEN 256
