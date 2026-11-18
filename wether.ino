#include <ESP8266WiFi.h>
#include <KiwisIoT.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

const char* ssid  = "YOUR_WIFI";
const char* pass  = "YOUR_PASSWORD";
const char* topic = "YOUR_TOPIC";

KiwisIoT kiwisiot(ssid, pass, topic);

#define DHTPIN D4
#define DHTTYPE DHT11
#define LED_PIN D5

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;

float temperature = 0;
float humidity = 0;
float pressure = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  if (!bmp.begin(0x76))
  {
    Serial.println("BMP280 not found!");
    while (1);
  }

  kiwisiot.begin();
}

void loop()
{
  kiwisiot.run();
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  pressure = bmp.readPressure() / 100.0; // hPa

  if (temperature > 35)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.println("----------------------");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure : ");
  Serial.print(pressure);
  Serial.println(" hPa");
  kiwisiot.send("1", String(temperature));
  kiwisiot.send("2", String(humidity));
  kiwisiot.send("3", String(pressure));

  delay(2000);
}