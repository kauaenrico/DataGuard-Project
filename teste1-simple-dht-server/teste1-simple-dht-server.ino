#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <stdio.h>
#include <Arduino_SNMP.h>
#include <WiFiUdp.h>


//Inicia o SMMPAgent
WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public");

//Referências para o SNMP
int humidity;
int temperature;


//WIFI
  const char* ssid = "2121_W5";
  const char* password = "gjx2121fbo";
  // Set your Static IP address
  IPAddress local_IP(192, 168, 15, 184);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 15, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8); // optional
  IPAddress secondaryDNS(1, 1, 1, 1); // optional

//sensor temp umid
  #define DHTPIN 5
  #define DHTTYPE    DHT22
  DHT dht(DHTPIN, DHTTYPE);
  unsigned long previousMillis = 0;    // will store last time DHT was updated
  const long interval = 500;  // Updates DHT readings



//iniciando var em zero- no loop() ele é atualizado
float temp = 0.0, hum = 0.0;

// Create AsyncWebServer object on port 80
  AsyncWebServer server(80);







const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266+DHT22</h2>
  <p>
    <span>&#10052;</span>
    <span>Temperatura</span> 
    <span id="temperature">%TEMPERATURA%</span>
    <sup>&deg;C</sup>
  </p>
  <p>
    <span>&#127777;</span>  
    <span>Umidade</span>
    <span id="humidity">%UMIDADE%</span>
    <sup>%</sup>
  </p>
</body>

<script>


// Obtém os valores de temperatura e umidade dos elementos HTML e atualiza as variáveis globais
function updateValues() {
  var temperature = document.getElementById("temperature").textContent;
  var humidity = document.getElementById("humidity").textContent;
  window.globalTemperature = temperature;
  window.globalHumidity = humidity;
  //PRINTAR NO CONSOLE O VALOR  
  //console.log(humidity);
  //console.log(temperature);
}

// Atualiza os valores 
setInterval(updateValues, 500);


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";
//FIM DO HTML AQUI


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURA"){
    return String(temp);
  }
  else if(var == "UMIDADE"){
    return String(hum);
  }
  return String();
}



void setupSNMP()
{
  //Inicializa o snmp
  snmp.setUDP(&udp);
  snmp.begin();
  //Adiciona o OID para umidade (apenas leitura)
  snmp.addReadOnlyIntegerHandler(".1.3.6.1.4.1.12345.0", humidity, false);
  //Adiciona o OID para temperatura (apenas leitura)
  snmp.addReadOnlyIntegerHandler(".1.3.6.1.4.1.12345.1", temperature, false);

}



void verifySNMP()
{
  //Deve ser sempre chamado durante o loop principal
  snmp.loop();

  //Se aconteceu alteração de um dos valores
  if(snmp.setOccurred)
  {
    //Reseta a flag de alteração
    snmp.resetSetOccurred();
  }
}


//////////////////////////////////////////

void setup(){
  setupSNMP();
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
     Serial.println("STA Failed to configure");
  }
  
  WiFi.begin(ssid, password);
  Serial.println("#");
  Serial.println("#");
  Serial.println("#");
  Serial.print("Conexão com WiFi ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando...");
  }

  // Print ESP8266 Local IP Address
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
  Serial.println("#");
  Serial.println("#");
  Serial.println("#");



  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temp).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(hum).c_str());
  });

  // Start server
  server.begin();
}

/////////
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      temp = newT;
      Serial.print("Temperatura: ");
      Serial.print(temp);
      Serial.println("ºC");
    }
    
    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      hum = newH;
      Serial.print("Umidade: ");
      Serial.print(hum);
      Serial.println("%");
    }
    Serial.println("----------");
  }
  verifySNMP();
}
