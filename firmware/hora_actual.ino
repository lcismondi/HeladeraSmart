#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>        //Importada manualmente de https://github.com/PaulStoffregen/Time

// datos para conectarnos a nuestra red wifi
const char *ssid     = "<SSID>";
const char *password = "<Password>";

//iniciamos el cliente udp para su uso con el server NTP
WiFiUDP ntpUDP;

// cuando creamos el cliente NTP podemos especificar el servidor al // que nos vamos a conectar en este caso
// 0.south-america.pool.ntp.org SudAmerica
// también podemos especificar el offset en segundos para que nos
// muestre la hora según nuestra zona horaria en este caso
// restamos -10800 segundos ya que estoy en argentina.
// y por ultimo especificamos el intervalo de actualización en
// mili segundos en este caso 6000

NTPClient timeClient(ntpUDP, "0.south-america.pool.ntp.org", -10800, 6000);

void setup() {
  Serial.begin(115200); // activamos la conexión serial

  WiFi.begin(ssid, password); // nos conectamos al wifi

  // Esperamos hasta que se establezca la conexión wifi
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  //sometimes the NTP Client retrieves 1970. To ensure that doesn’t happen we need to force the update.
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  Serial.println();
  //setTime(0,0,00,1,1,1970);    //Inicio de la cuenta TimerLib 1 Enero 1970
}

void loop() {
  //timeClient.update(); //sincronizamos con el server NTP

  /*
  Serial.print("Get Epoch Time: ");
  Serial.println(timeClient.getEpochTime());
  Serial.print("Day: ");
  Serial.println(timeClient.getDay());
  Serial.print("Formatted Time: ");
  Serial.println(timeClient.getFormattedTime());
  */

  //Serial.print("Now: ");
  //time_t t = now();//Declaramos la variable time_t t;
  time_t t = timeClient.getEpochTime();
  //Serial.println(t);


  Serial.print("Son las: ");
  Serial.println(   String(hour(t))   + ":"
                  + String(minute(t)) + ":" 
                  + String(second(t)) + " "
                  + String(day(t))    + "/" 
                  + String(month(t))  + "/"
                  + String(year(t)));
  
  /*
  Serial.print("Hora: ");
  Serial.println(hour(t));          // returns the hour for the given time t
  Serial.print("Minuto: ");
  Serial.println(minute(t));        // returns the minute for the given time t
  Serial.print("Segundos: ");
  Serial.println(second(t));        // returns the second for the given time t
  Serial.print("Dia: ");
  Serial.println(day(t));           // the day for the given time t
  Serial.print("Día de la semana: ");
  Serial.println(weekday(t));       // day of the week for the given time t
  Serial.print("Mes: ");
  Serial.println(month(t));         // the month for the given time t
  Serial.print("Año: ");
  Serial.println(year(t));          // the year for the given time t
  */


  delay(1000);
}
