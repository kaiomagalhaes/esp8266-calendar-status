

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
const int pinoLEDR = D7;
const int pinoLEDY = D6;
const int pinoLEDG = D5;

String serverName = "http://kaio-calendar.herokuapp.com/kaio/isinmeeting";

unsigned long timerDelay = 5000;

unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);

  setupLeds();
  connectToWifi();
  //  pinMode(pinoLEDG, OUTPUT);
  //  pinMode(pinoLEDB, OUTPUT);
}

void loop() {
  StaticJsonDocument<200> doc;

  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      signaWifiPinging();
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName.c_str());

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
          signalInMeeting();
        } else if (timeToNextEvent > 0 && timeToNextEvent <= 5) {
          Serial.println("it should be yellow bright");
          signalMeetingSoon();
        }
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected, connecting again");
      connectToWifi();
    }
    lastTime = millis();
  }
}
