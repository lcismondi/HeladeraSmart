const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

float tension = 0;
float adADC = 0;
float rms = 0;
unsigned int Ntension = 0;
unsigned int tiempoM = 0;
unsigned int muestras = 300;
float nyquist = 0;
unsigned int frecuencia = 50;
unsigned long comuDB = 0;
unsigned long muestreoV = 0;


void setup() {
  // initialize serial communication at 115200
  Serial.begin(115200);

}

void loop() {


  if (millis() - comuDB >= 1000)
  {
    comuDB = millis();
    
    rms = sqrt(tension/ Ntension);

    

    Serial.println("*********Resetea**********");

  tension = 0;
  Ntension = 0;

  }
  else
  {

    //muestreoV = millis();

    //y = 2.1794x - 1148.6
    //y = 2,1999x - 1158,4
    //y = 2.2010x - 1158.1
    //y = 2,1967x - 1156,7

    adADC = 2.1967 * analogRead(analogInPin) - 1156.7;
    tension = adADC * adADC + tension;
    //Serial.println((String)"Medición: " + tension);
    Ntension++;
    

    //tiempoM = millis() - muestreoV;
    //Serial.println((String)"Tiempo de medición:" + tiempoM);

  }


}
