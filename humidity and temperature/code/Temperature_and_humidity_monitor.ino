#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// WiFi details
const char* ssid = "AshwiniP_Ashu";
const char* password = "Ashu.p_1564";

// ThingSpeak details
unsigned long channelID = 3353543;
const char* writeAPIKey = "EVTHHWNXG6D3Y580";

WiFiClient client;

// DHT sensor setup
#define DHTPIN D4        // Pin connected to DHT11
#define DHTTYPE DHT11    // Sensor type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if reading failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values in Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Error sending data");
  }

  delay(15000); // ThingSpeak needs 15 sec delay
}
