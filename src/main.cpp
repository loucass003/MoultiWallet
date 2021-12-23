#include <Arduino.h>
#include <WiFi.h>
#include <AutoConnect.h>
#include "ArduinoOTA.h"
#include <arduino-timer.h>

#define PINS_LEN 8

uint8_t pins[PINS_LEN] = {26, 25, 4, 5, 13, 12,14, 27};
WebServer portalServer;
AutoConnect portal(portalServer);

AutoConnectConfig apConfig("MOULTIPRISE", "12345678");

auto timer = timer_create_default();

void initOTA() {
    ArduinoOTA.setHostname("MOULTIPRISE");
    // No authentication by default
    //ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}

int relay = HIGH;

bool toggle_relays(void *) {
  relay = relay == HIGH ? LOW : HIGH;
  
  for (int i = 0; i < PINS_LEN; i++){
    digitalWrite(pins[i], relay);
  }

  return true;
}



void setup() {

  apConfig.preserveAPMode = false;
  apConfig.ota = AC_OTA_EXTRA;
  portal.config(apConfig);

  if (portal.begin()) {
      Serial.println("WiFi connected: " + WiFi.localIP().toString());
      initOTA();
      for (int i = 0; i < PINS_LEN; i++){
        pinMode(pins[i], OUTPUT);
      }

      timer.every(100, toggle_relays);
  }

  
}

  `

void loop() {
  portal.handleClient();
  ArduinoOTA.handle();

 

  // if (millis() - delay > 200) {
  //   for (int i = 0; i < PINS_LEN; i++){
  //     digitalWrite(pins[i], LOW);
  //   }
  //   delay = millis();
  // } else {
  //   for (int i = 0; i < PINS_LEN; i++){
  //     digitalWrite(pins[i], HIGH);
  //   }
  // }

  // delay(200);

  // for (int i = 0; i < PINS_LEN; i++){
  //   digitalWrite(pins[i], HIGH);
  // }

  // delay(200);

  timer.tick();
}