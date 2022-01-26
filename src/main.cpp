#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DHTesp.h>
#include <ThingsBoard.h>
#include "wifi_id.h"
#include "thingsboard_id.h" // THINGSBOARD_ACCESS_TOKEN
#define PIN_DHT    D3
#define THINGSBOARD_SERVER "demo.thingsboard.io"
WiFiClient espClient;
ThingsBoard tb(espClient);
DHTesp dht;

void WifiConnect();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.setup(PIN_DHT, DHTesp::DHT11);

  WifiConnect();
  if (tb.connect(THINGSBOARD_SERVER, THINGSBOARD_ACCESS_TOKEN))
    Serial.println("Connected to thingsboard");
  else
    Serial.println("Error connected to thingsboard");
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  if (millis() % 3000 ==0 && tb.connected())
  {
    digitalWrite(LED_BUILTIN, 0);
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    if (dht.getStatus()==DHTesp::ERROR_NONE)
    {
      Serial.printf("Temperature: %.2f C, Humidity: %.2f %%\n", 
                    temperature, humidity);
      tb.sendTelemetryFloat("temperature", temperature);
      tb.sendTelemetryFloat("humidity", humidity);
    }
    digitalWrite(LED_BUILTIN, 1);
  }
  tb.loop();
}

void WifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }  
  Serial.print("System connected with IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
}
