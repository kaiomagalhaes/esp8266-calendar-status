

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
const int pinoLEDR = D7;
const int pinoLEDY = D6;
const int pinoLEDG = D5;

const char* ssid = "vaka_viking_2g";
const char* password = "batataquente";

String serverName = "http://kaio-calendar.herokuapp.com/kaio/isinmeeting";

unsigned long timerDelay = 5000;

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(pinoLEDR, OUTPUT);
  pinMode(pinoLEDY, OUTPUT);
  pinMode(pinoLEDG, OUTPUT);

  //  pinMode(pinoLEDG, OUTPUT);
  //  pinMode(pinoLEDB, OUTPUT);

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  StaticJsonDocument<200> doc;

  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?temperature=24.37";

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, payload);

        // Test if parsing succeeds.
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        const bool inEvent = doc["inEvent"];
        const float timeToNextEvent = doc["timeToNextEvent"];

        Serial.println("in event");
        Serial.println(inEvent);
        Serial.println("timeToNextEvent: ");
        Serial.println(timeToNextEvent);

        if (inEvent) {
          Serial.println("it should be red bright");
          digitalWrite(pinoLEDR, HIGH);  // acende o LED
          digitalWrite(pinoLEDY, LOW);   // acende o LED
          digitalWrite(pinoLEDG, LOW);   // acende o LED
        } else if (timeToNextEvent > 0 && timeToNextEvent <= 5) {
          Serial.println("it should be yellow bright");
          digitalWrite(pinoLEDY, HIGH);  // acende o LED
          digitalWrite(pinoLEDG, LOW);   // acende o LED
          digitalWrite(pinoLEDR, LOW);
        } else {
          Serial.println("it should be green bright");
          digitalWrite(pinoLEDG, HIGH);
          digitalWrite(pinoLEDY, LOW);
          digitalWrite(pinoLEDR, LOW);
        }
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
