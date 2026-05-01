/*
 * IoT Environmental Monitoring System
 * Author: Mothi Charan Naik Desavath
 * Hardware: ESP32 + DHT22 + MQ135 + BMP280
 * Features: WiFi, MQTT, Web Dashboard
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "mothi/envmonitor";

// Pins
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define MQ135_PIN 34

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
WebServer server(80);

float temperature, humidity, pressure, airQuality;

void setup() {
    Serial.begin(115200);
    dht.begin();
    Wire.begin();
    bmp.begin(0x76);

    connectWiFi();
    mqttClient.setServer(mqtt_server, mqtt_port);
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("IoT Monitor Ready");
}

void loop() {
    if (!mqttClient.connected()) reconnectMQTT();
    mqttClient.loop();
    server.handleClient();

    static unsigned long lastRead = 0;
    if (millis() - lastRead > 5000) {
        readSensors();
        publishData();
        lastRead = millis();
    }
}

void readSensors() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    pressure = bmp.readPressure() / 100.0F;
    airQuality = analogRead(MQ135_PIN);
    Serial.printf("T:%.1f H:%.1f P:%.1f AQ:%.0f\n", temperature, humidity, pressure, airQuality);
}

void publishData() {
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["pressure"] = pressure;
    doc["air_quality"] = airQuality;
    char buf[200];
    serializeJson(doc, buf);
    mqttClient.publish(mqtt_topic, buf);
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32Monitor")) Serial.println("MQTT connected");
        else delay(5000);
    }
}

void handleRoot() {
    server.send(200, "text/html", "<h1>IoT Monitor</h1><p>Visit /data for JSON</p>");
}

void handleData() {
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["pressure"] = pressure;
    doc["air_quality"] = airQuality;
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}