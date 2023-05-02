#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Arduino_SNMP.h>

//Please modify your wifi creaditial here
const char* ssid = "2121_W5";
const char* password = "gjx2121fbo";

WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public");  // Starts an SMMPAgent instance with the community string 'public'

int changingNumber = 0;
int changingNumber2; //serial read
int settableNumber = 0; //uptime


void setup(){
  
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // give snmp a pointer to the UDP object
    snmp.setUDP(&udp);
    snmp.begin();
    
    // add 'callback' for an OID - pointer to an integer
    snmp.addIntegerHandler(".1.3.6.1.4.1.5.0", &changingNumber);
    
    snmp.addIntegerHandler(".1.3.6.1.4.1.5.2", &changingNumber2);


    
    // you can accept SET commands with a pointer to an integer (or string)
    snmp.addIntegerHandler(".1.3.6.1.4.1.5.1", &settableNumber);
    
    
    
}

void loop(){
    snmp.loop(); // must be called as often as possible
    if(snmp.setOccurred){
        Serial.printf("Number has been set to value: %i", settableNumber);
        snmp.resetSetOccurred();
    }
    //changingNumber++;

  
   if(Serial.available()){
    changingNumber2=Serial.parseInt();
    Serial.println(changingNumber2);
    delay(1000);
   }


  if(1==1){
    settableNumber=settableNumber+1;
    delay(990);
    //DEBUG PURPOSES Serial.print("uptime: "); Serial.println(settableNumber);
   
  }
}
