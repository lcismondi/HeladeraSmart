/*
   EEPROM Write

  Guardado de datos de SSID y Pasword en memoria

*/

#include <EEPROM.h>

int addr = 0;
int memoria = 4096;                     //Tamaño en bytes

char redWifi[31] = "<SSID name>";      //Warning por el largo de la cadena de texto
char pass[63] = "<Password>";       //Warning por el largo de la cadena de texto

//SSID < 32 caracteres
//PASS > 8 y < 64 caracteres

void setup() {
  Serial.begin(115200);
  EEPROM.begin(memoria);

  Serial.println();
  Serial.print("Memoria inicializada en: ");
  Serial.print(memoria);
  Serial.println("bytes");
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();



  //Guarda SSID
  for (int i = 0; i <= 31; i++)
  {

    EEPROM.write(addr, redWifi[i]);
    addr++;

    //Completar con 255 al terminar la palabra
    //WARNING!!! Controlar overflow de memoria
  }

  //Guarda contraseña  
  for (int i = 0; i <= 63; i++)
  {

    EEPROM.write(addr, pass[i]);
    addr++;

    //WARNING!!! Controlar overflow de memoria
  }


  if (EEPROM.commit())
  {
    Serial.println("EEPROM successfully committed");
  }
  else
  {
    Serial.println("ERROR! EEPROM commit failed");
  }

}

void loop() {

  Serial.println("Fin de ejecución");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2500);
}
