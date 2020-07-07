/*
    HeladeraSmart
    https://github.com/lcismondi/HeladeraSmart

    Luciano Cismondi
    http://www.LucianoCismondi.com.ar

*/

#include <ESP8266WiFi.h>
#include "HeladeraSmart.h"


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  WiFi.begin("nombre-red", "contraseña-red");
  //SSID 32 < 32 caracteres
  //PASS 8 > y < 64 caracteres

  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }
  Serial.println();

  //Configuración wifi
  Serial.print("Estado: ");
  Serial.println(EstadoWiFi[WiFi.status()]);
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("PSK: ");
  Serial.println(WiFi.psk());
  Serial.print("RSSI:");
  Serial.print(WiFi.RSSI());
  Serial.println("dBm");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Autoconnect: ");
  Serial.println(ONOFF[WiFi.getAutoConnect()]);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subred: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  Serial.print("Hostname: ");
  Serial.println(WiFi.hostname());
  Serial.print("BSSID: ");
  Serial.println(WiFi.BSSIDstr());
  Serial.print("");
  Serial.print("");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  Serial.print("RSSI:");
  Serial.print(WiFi.RSSI());
  Serial.println("dBm");
  
}
