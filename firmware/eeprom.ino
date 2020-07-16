/*
   EEPROM Write

  Guardado de datos de SSID y Pasword en memoria

*/

#include <EEPROM.h>

int addr = 100;
int memoria = 4096;                     //Tamaño en bytes

char redWifi[31] = "<SSID>";      //Warning por el largo de la cadena de texto
char pass[63] = "<PASSWORD>";       //Warning por el largo de la cadena de texto

char dbLink[109] = "<DATABASE LINK>"; //99 caracteres

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

  //Guarda link  
  for (int i = 0; i <= 109; i++)
  {

    EEPROM.write(addr, dbLink[i]);
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

  char redWifi[31] = " ";
  char pass[63] = " ";
  char dbLink[109] = " ";

  int addr = 100;

  Serial.print("RED: ");
  Serial.print(sizeof(redWifi));
  Serial.println(redWifi);
  Serial.print("PASS: ");
  Serial.print(sizeof(pass));
  Serial.println(pass);
  Serial.print("LINK: ");
  Serial.print(sizeof(dbLink));
  Serial.println(dbLink);

  Serial.println("Fin de ejecución");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2500);
  digitalWrite(LED_BUILTIN, LOW);
  
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
  
  
  delay(2500);
  
}
