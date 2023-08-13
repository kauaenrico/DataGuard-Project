Projeto iniciado pelo arquivo
\PROJETO-3SEMESTRE-ENG_COMPUTACAO\teste1-simple-dht-server\

ACESSO WEB UTILIZANDO PORTA 8080
ip:8080
exemplo: 192.168.15.20:8080

Changes:

//WIFI
  const char* ssid = "SSID";
  const char* password = "SENHA";

######

Caso queira DHCP, comentar os seguintes itens:
	//Set your Static IP address 
	  IPAddress local_IP(192, 168, 15, 47); //(192, 168, 137, 214)
	  IPAddress gateway(192, 168, 15, 1);
	  IPAddress subnet(255, 255, 255, 0);
	  IPAddress primaryDNS(8, 8, 8, 8); // optional
	  IPAddress secondaryDNS(1, 1, 1, 1); // optional
	
	//Configures static IP address
	  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
 	    Serial.println("STA Failed to configure");
 	  }

######
SNMP OID
	1.3.6.1.4.1.5.0 - Variavel changingNumber - Variavel temp
  	1.3.6.1.4.1.5.2 - Variavel changingNumber2 - Variavel hum 
	1.3.6.1.4.1.5.1 - Variavel settableNumber - Variavel uptimeValue