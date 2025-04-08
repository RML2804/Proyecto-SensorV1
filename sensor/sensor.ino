#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <DHT.h>

#define DHTPIN 4    // Pin donde conectas el DHT22 (en este caso D2, GPIO4)
DHT dht(DHTPIN, DHT22);  // Cambia a DHT22

// Credenciales WiFi
#define WIFI_SSID "Totalplay-2.4G-0010"    
#define WIFI_PASSWORD "cjAwPXUfYW4M79t3"

// Credenciales Firebase
#define FIREBASE_HOST "sensor-e7847-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "V3rYchrt409GwLGkn8UzhH7JfPIN9maVagyzPxk9" 

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("¡Conectado a WiFi!");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT!");
    return;
  }

  Serial.print("Humedad: "); Serial.print(h); Serial.println(" %");
  Serial.print("Temperatura: "); Serial.print(t); Serial.println(" *C");

  if (Firebase.setFloat(firebaseData, "DTH22/Humedad", h)) {
    Serial.println("Humedad enviada con éxito.");
  } else {
    Serial.println("Error al enviar humedad: " + firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, "DTH22/Temperatura", t)) {
    Serial.println("Temperatura enviada con éxito.");
  } else {
    Serial.println("Error al enviar temperatura: " + firebaseData.errorReason());
  }

  delay(5000); // Espera 5 segundos antes del siguiente envío
}