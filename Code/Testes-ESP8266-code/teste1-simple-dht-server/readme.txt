https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/


monitor serial printa o IP
acessa web, ele aparece

no codigo, alterar SSID e Senha em:
16	const char* ssid = "REPLACE_WITH_YOUR_SSID";
17	const char* password = "REPLACE_WITH_YOUR_PASSWORD";

no codigo, alterar pin usado no DHT em:
19	#define DHTPIN 5     // Digital pin connected to the DHT sensor

Alterar atualização dos dados em:
40	const long interval = 500;  


