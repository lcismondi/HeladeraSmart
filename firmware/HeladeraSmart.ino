/*
    HeladeraSmart
    https://github.com/lcismondi/HeladeraSmart

    Luciano Cismondi
    http://www.LucianoCismondi.com.ar

*/

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "HeladeraSmart.h"
#include <WiFiClientSecureBearSSL.h>

//*******************************************************************************
//Variables globales ************************************************************
//*******************************************************************************

int addr = 100;
int memoria = 4096;                           //Tamaño en bytes
unsigned long comuDB = 0;

//Información guardada en memoria estática
char redWifi[31];
char pass[63];
char dbLink[109];

//ID de comunicación a los campos de la base de datos
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
};
identificador id;

ESP8266WiFiMulti WiFiMulti;

//*******************************************************************************
//Inicialización ****************************************************************
//*******************************************************************************

void setup() {

  //Inicialización
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(memoria);

  //Lee datos de conexión de la memoria
  //SSID 32 < 32 caracteres
  //PASS 8 > y < 64 caracteres
  for (int i = 0; i <= 31; i++)
  {
    redWifi[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }
  //Contraseña red wifi
  for (int i = 0; i <= 63; i++)
  {
    pass[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }
  //Conexión base de datos
  for (int i = 0; i <= 109; i++)
  {
    dbLink[i] = EEPROM.read(addr);
    //Se puede limitar buscando 255 como fin de cadena de texto
    addr++;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(redWifi, pass);
  //WiFi.begin("nombre-red", "contraseña-red");

  //Conecta WiFi
  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }
  Serial.println();


  Serial.println("");
  Serial.println("");

  //El IDE de Arduino no resetea el ESP8266 al abrir el monitor serie
  //por lo tanto puede que no se muestren las directivas del setup()
}

void loop() {

  //Keep alive
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH


  //*******************************************************************************
  //Comunicación BD **********************************************************
  //*******************************************************************************
  
  if ((WiFiMulti.run() == WL_CONNECTED) && millis() - comuDB >= 60000 )
  {

    comuDB = millis();
    
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

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
  else
  {
   Serial.println( (60000 - (millis() - comuDB))/1000 );
    
  }
  //delay(10000);

  //*******************************************************************************
  //Configuración de red **********************************************************
  //*******************************************************************************
  if (Serial.available() > 0)
  {

    //String caca = Serial.readString();

    if (Serial.readString().toInt() == 1)
    {

      Serial.println("");
      Serial.println("CONFIGURACIÓN:");
      Serial.println("");
      //Datos de memoria
      Serial.println((String)"RED: " + redWifi);
      Serial.println((String)"PASS: " + pass);
      Serial.println((String)"DB link: " + dbLink);
      //Configuración wifi
      Serial.println("Estado: " + EstadoWiFi[WiFi.status()]);
      Serial.println("SSID: " + WiFi.SSID());
      Serial.println("PSK: " + WiFi.psk());
      Serial.println((String)"RSSI:" + WiFi.RSSI() + "dBm");
      Serial.println("IP: " + WiFi.localIP());
      Serial.println("Autoconnect: " + ONOFF[WiFi.getAutoConnect()]);
      Serial.println("MAC: " + WiFi.macAddress());
      Serial.println("Subred: " + WiFi.subnetMask());
      Serial.println("Gateway: " + WiFi.gatewayIP());
      Serial.println("DNS 1: " + WiFi.dnsIP(0));
      Serial.println("DNS 2: " + WiFi.dnsIP(1));
      Serial.println("Hostname: " + WiFi.hostname());
      Serial.println("BSSID: " + WiFi.BSSIDstr());

      Serial.println("");
      Serial.println("");
    }

  }


}
