#include <WiFi.h>
#include <PubSubClient.h>

// Configurações Wi-Fi
const char* ssid = "wokwi-iot-gateway";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic = "sensor/estado";

WiFiClient espClient;
PubSubClient client(espClient);

// Configurações de hardware
const int DHT_PIN = 15;
const int led = 5;
const int pir = 19;

void setup() {
  // Configuração de pinos
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
  Serial.begin(115200);
  
  // Conexão Wi-Fi
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  if (WiFi.status() != WL_CONNECTED) {
    client.connect("sensor/estado");
    Serial.println(" Conectado ao Wifi");
    delay(500);

  // Conexão MQTT
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("sensor/estado")) { // Nome do cliente MQTT
      Serial.println("Conectado ao MQTT");
      client.subscribe(topic);
    } else {
      Serial.print("Falha na conexão com o MQTT, erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  }
}

void loop() {
  // Leitura do sensor PIR
  const int motionDetected = digitalRead(pir);
  Serial.println(motionDetected);
  delay(100);

  // Lógica do sensor de movimento
  if (motionDetected == 1) {
    digitalWrite(led, HIGH);
    client.publish(topic, "Movimento Detectado"); // Envia mensagem ao tópico MQTT
    delay(1000);
  } else {
    digitalWrite(led, LOW);
    client.publish(topic, "Movimento não detectado"); // Envia mensagem ao tópico MQTT
    delay(1000);
  }
  client.loop(); // Mantém a conexão MQTT ativa
}
