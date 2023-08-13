#include <ESP8266WiFi.h>

const char *apSSID = "ConfigurarWiFi";
const char *apPassword = "senha123";
boolean apMode = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Desligar o LED embutido inicialmente

  // Inicializar serial para debug
  Serial.begin(115200);

  // Verificar se há configurações de rede salvas
  if (WiFi.SSID() == "") {
    apMode = true;
    startAPMode();
  } else {
    connectToWiFi();
  }
}

void loop() {
  // Você pode adicionar outras partes do seu código aqui
}

void startAPMode() {
  digitalWrite(LED_BUILTIN, LOW); // Ligar o LED para indicar o modo AP

  Serial.println("Modo de Ponto de Acesso ativado.");
  Serial.print("SSID do AP: ");
  Serial.println(apSSID);
  
  // Iniciar o modo AP com o SSID e senha definidos
  WiFi.softAP(apSSID, apPassword);
  
  Serial.println("Conecte-se a este WiFi e acesse 192.168.4.1 para configurar.");
}

void connectToWiFi() {
  digitalWrite(LED_BUILTIN, HIGH); // Desligar o LED para indicar a conexão com a rede
  
  Serial.println("Conectando à rede WiFi...");

  WiFi.begin(); // Iniciar a conexão usando as configurações salvas
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Aguardando conexão WiFi...");
  }
  
  Serial.println("Conectado à rede WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}
