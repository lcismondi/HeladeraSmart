/*
    HeladeraSmart
    https://github.com/lcismondi/HeladeraSmart

    Luciano Cismondi
    http://www.LucianoCismondi.com.ar

*/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <WiFiClientSecureBearSSL.h>

//https://buger.dread.cz/simple-esp8266-https-client-without-verification-of-certificate-fingerprint.html

//Variables globales************************************************************

int addr = 100;                         //La librería utiliza la parte baja de la memoria para escribir "algo".
int memoria = 4096;                     //Tamaño en bytes

char redWifi[31];
char pass[63];
char dbLink[109];

struct identificador
{
  const String NTP = "&entry.1694967850=";    //Fecha y hora del servidor NTP
  const String FHW = "&entry.1925489208=";    //Fecha y hora del hardware
  const String VAC = "&entry.1583316227=";    //Tensión alterna eficaz
  const String IAC = "&entry.1738067796=";    //Corriente alterna eficaz
  const String DTY = "&entry.326159585=";     //Duty cicle
  const String TEM = "&entry.2016185492=";    //Temperatura
  const String APE = "&entry.923977541=";     //Apertura de puerta
  const String TPT = "&entry.497615105=";     //Tiempo de apertura de puerta
  const String dummy1 = "a";
  const String dummy2 = "b";
};
identificador id;

ESP8266WiFiMulti WiFiMulti;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(memoria);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  //Lee datos de conexión de la memoria
  //SSID 32 < 32 caracteres
  //PASS 8 > y < 64 caracteres
  for (int i = 0; i <= 31; i++)
  {
    redWifi[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }

  Serial.print("RED: ");
  Serial.println(redWifi);

  for (int i = 0; i <= 63; i++)
  {
    pass[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }

  Serial.print("PASS: ");
  Serial.println(pass);

  for (int i = 0; i <= 109; i++)
  {
    dbLink[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }

  Serial.print("LINK: ");
  Serial.println(dbLink);


  //Limpia el buffer
  /*
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  */

  WiFi.mode(WIFI_STA);
  //WiFiMulti.addAP("SIC 2.4GHz", "LUJOIG240128");
  WiFiMulti.addAP(redWifi, pass);
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    //client->setFingerprint(fingerprint);
    client->setInsecure();

    HTTPClient https;

    String direccion =  String(dbLink)  +
                        "?usp=pp_url"   +
                        id.NTP  + "8"   +
                        id.FHW  + "7"   +
                        id.VAC  + "6"   +
                        id.IAC  + "5"   +
                        id.DTY  + "0.4" +
                        id.TEM  + "3"   +
                        id.APE  + "2"   +
                        id.TPT  + "1"   +
                        "&submit=Submit";

    Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, direccion)) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 10s before next round...");
  Serial.print("RED: ");
  Serial.println(redWifi);
  Serial.print("PASS: ");
  Serial.println(pass);
  Serial.print("LINK: ");
  Serial.println(dbLink);
  delay(10000);
}
