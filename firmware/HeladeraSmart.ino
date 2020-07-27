/*
    HeladeraSmart
    https://github.com/lcismondi/HeladeraSmart

    Luciano Cismondi
    http://www.LucianoCismondi.com.ar

*/

#include "HeladeraSmart.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>                          //Importada manualmente de https://github.com/PaulStoffregen/Time


//*******************************************************************************
//Pinout ************************************************************************
//*******************************************************************************

const int oneWireBus = 4;//D2
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

//*******************************************************************************
//Variables globales ************************************************************
//*******************************************************************************

unsigned int addr = 100;
const int memoria = 4096;                           //Tamaño en bytes
unsigned int Ntemperatura = 0;
int tension = 0;
int Ptension = 512;
int Mtension = 512;
float VtensionP = 0;
float VtensionM = 0;
float temperatura = 0;
String timeNTP = "";
String timeHW = "";
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
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;                               //iniciamos el cliente udp para su uso con el server NTP

// cuando creamos el cliente NTP podemos especificar el servidor al que nos vamos a
// conectar en este caso, 0.south-america.pool.ntp.org SudAmerica. También podemos
// especificar el offset en segundos para que nos muestre la hora según nuestra zona
// horaria, en este caso, restamos -10800 segundos ya que estoy en argentina. Y por
// ultimo especificamos el intervalo de actualización en mili segundos, acá 6000.

NTPClient timeClient(ntpUDP, "0.south-america.pool.ntp.org", -10800, 6000);

//*******************************************************************************
//Inicialización ****************************************************************
//*******************************************************************************

void setup() {

  //Inicialización
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(analogInPin, INPUT);
  Serial.begin(115200);
  EEPROM.begin(memoria);
  sensors.begin();                            //Temperatura DS18B20

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

  //Conecta WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(redWifi, pass);
  //WiFi.begin("nombre-red", "contraseña-red");
  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  //Conexión con el servidor NTP
  timeClient.begin();
  //sometimes the NTP Client retrieves 1970. To ensure that doesn’t happen we need to
  // force the update.
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  Serial.println("");
  Serial.println("");

  //El IDE de Arduino no resetea el ESP8266 al abrir el monitor serie
  //por lo tanto puede que no se muestren las directivas del setup()
}

void loop() {

  //Keep alive
  //digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  //delay(1000);                      // Wait for a second
  //digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH


  //*******************************************************************************
  //Fecha *************************************************************************
  //*******************************************************************************

  //timeClient.update();            //sincroniza con el server NTP
  time_t t = timeClient.getEpochTime();
  time_t h = millis();

  
    timeNTP = String(month(t))  + "/"
            + String(day(t))    + "/"
            + String(year(t))   + "+"
            + String(hour(t))   + ":"
            + String(minute(t)) + ":"
            + String(second(t));
    //Serial.println("Son las: " + timeNTP);
    timeHW  = String(month(h))  + "/"
            + String(day(h))    + "/"
            + String(year(h))   + "+"
            + String(hour(h))   + ":"
            + String(minute(h)) + ":"
            + String(second(h));
    //Serial.println("Son las: " + timeHW);
  


  //*******************************************************************************
  //Temperatura *******************************************************************
  //*******************************************************************************

    //Frena el refresh, es lento
    sensors.requestTemperatures();
    temperatura = + sensors.getTempCByIndex(0);
    Ntemperatura++;
    

  //*******************************************************************************
  //Tensión alterna 220VAC ********************************************************
  //*******************************************************************************

  tension = analogRead(analogInPin);

  if (tension < Mtension)
  {
    Mtension = tension;
  }
  if (tension > Ptension)
  {
    Ptension = tension;
  }



  //*******************************************************************************
  //Comunicación BD ***************************************************************
  //*******************************************************************************

  if ((WiFiMulti.run() == WL_CONNECTED) && millis() - comuDB >= 60000 )
  {
    //Si la comunicación es cada 60 segundos (1 minuto)
    //Día   1140 registros
    //Mes  43299 registros
    //Año 518400 registros

    comuDB = millis();

    //Conversor de tensión
    VtensionP = map(Ptension, 377, 672, -321, 321);
    VtensionM = map(Mtension, 377, 672, -321, 321);

    //Inicia transferencia de información
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setInsecure();

    HTTPClient https;

    String direccion =  String(dbLink)  +
                        "?usp=pp_url"   +
                        id.NTP  + timeNTP     +
                        id.FHW  + timeHW      +
                        id.VAC  + (VtensionP - VtensionM) / (2 * sqrt(2))   +
                        id.IAC  + "5"   +
                        id.DTY  + "0.4" +
                        id.TEM  + (temperatura / Ntemperatura) +
                        id.APE  + "2"   +
                        id.TPT  + "1"   +
                        "&submit=Submit";
    Serial.println("Link: " + direccion);

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

          //Comunicación exitosa reinicia parámetros
          temperatura = 0;
          Ntemperatura = 0;
          Mtension = 512;
          Ptension = 512;
          
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

    //Serial.println((String)"Tiempo restante: " + (60000 - (millis() - comuDB)) / 1000 );
    //Serial.println((String)"Temperatura: " + temperatura);
    VtensionP = map(Ptension, 377, 672, -321, 321);
    VtensionM = map(Mtension, 377, 672, -321, 321);
    Serial.println((String)"RMS:" + (VtensionP - VtensionM) / (2 * sqrt(2)));
    


  }
  //delay(1000);

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
