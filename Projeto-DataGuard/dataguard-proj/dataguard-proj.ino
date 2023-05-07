// Author: Kauã Enrico Pasti Altran
// Date: 24/03/2023
// Project: DataGuard
// UNISAL - Engenharia de Computação/3º Semestre

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <stdio.h>
#include <Arduino_SNMP.h>
#include <WiFiUdp.h>
#include <SNMP_Agent.h>

//#include <LittleFS.h>
//#include <ArduinoJson.h> // Saved data will be stored in JSON
//#define FORMAT_LITTLEFS_IF_FAILED true // Be careful, this will wipe all the data stored. So you may want to set this to false once used once.




//WIFI
  const char* ssid = "2121_W5";
  const char* password = "gjx2121fbo";
  
  // Set your Static IP address
//  IPAddress local_IP(192, 168, 15, 47); //(192, 168, 137, 214)
//  IPAddress gateway(192, 168, 15, 1);
//  IPAddress subnet(255, 255, 255, 0);
//  IPAddress primaryDNS(8, 8, 8, 8); // optional
//  IPAddress secondaryDNS(1, 1, 1, 1); // optional

// SNMP
WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public");  // Starts an SMMPAgent instance with the community string 'public'

//VARs SNMP
int changingNumber;
int changingNumber2;
int settableNumber;
int uptimeValue=0;

 

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
<!DOCTYPE HTML>
<html>

<head>
    <title>DataGuard</title>
</head>

<body>
    <p id="temperature">%TEMPERATURA%</p>
    <p id="humidity">%UMIDADE%</p>
</body>

<script>
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("temperature").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 500);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("humidity").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
    }, 500);
</script>

</html>)rawliteral";
//FIM DO HTML AQUI
///////////////////////////////

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












//////////////////////////////////////////

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  dht.begin();
  
  // Connect to Wi-Fi
  // Configures static IP address
  //if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  //}
  
  WiFi.begin(ssid, password);
  Serial.println("#");
  Serial.println("#");
  Serial.println("#");
  Serial.print("Conexão com WiFi ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Conectando...");
  }

  // Print ESP8266 Local IP Address
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
  Serial.println("#");  Serial.println("#");  Serial.println("#");

  //SNMP
  // give snmp a pointer to the UDP object
  snmp.setUDP(&udp);
  snmp.begin();

  // add 'callback' for an OID - pointer to an integer
  snmp.addIntegerHandler(".1.3.6.1.4.1.5.0", &changingNumber);
  snmp.addIntegerHandler(".1.3.6.1.4.1.5.2", &changingNumber2);  
  // you can accept SET commands with a pointer to an integer (or string)
  snmp.addIntegerHandler(".1.3.6.1.4.1.5.1", &settableNumber);


  


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


//SNMP
    snmp.loop(); // must be called as often as possible
    if(snmp.setOccurred){
        Serial.printf("Number has been set to value: %i", settableNumber);
        snmp.resetSetOccurred();
    }
    //changingNumber++;

   changingNumber=temp;
   changingNumber2=hum;

   if(1==1){ //UPTIME
   uptimeValue=uptimeValue+1;
   settableNumber=uptimeValue;
   delay(990);
   //DEBUG
   Serial.print("uptime: "); Serial.println(uptimeValue);
   }
   
   if(Serial.available()){
    changingNumber2=Serial.parseInt();
    Serial.println(changingNumber2);
    delay(1000);
   }


 // if(1==1){ //UPTIME
 //   settableNumber=settableNumber+1;
 //   delay(990);
    //DEBUG  Serial.print("uptime: "); Serial.println(settableNumber);
  }
