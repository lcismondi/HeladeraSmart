const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int tension = 0;
int Ptension = 512;
int Mtension = 512;
float VtensionP = 0;
float VtensionM = 0;
float rms = 0;
unsigned int NtensionP = 0;
unsigned int NtensionM = 0;
unsigned long comuDB = 0;


void setup() {
  // initialize serial communication at 115200
  Serial.begin(115200);

}

void loop() {

  if (millis() - comuDB >= 10000)
  {
    comuDB = millis();

    Mtension = 512;
    NtensionM = 0;
    Ptension = 512;
    NtensionP = 0;

    Serial.println("*********Resetea**********");

  }
  else
  {

    // read the analog in value
    tension = analogRead(analogInPin);

    //Serial.println((String)"Medición: " + tension);

    //if (tension < 512) //Semiciclo negativo
    {
      if (tension < Mtension)
      {
        Mtension = tension;
        NtensionM++;
        //Serial.println((String)"Mtension: " + Mtension + "," + Ptension);
      }
    }
    //else // tension >= 512  semiciclo positivo
    {
      if (tension > Ptension)
      {
        Ptension = tension;
        NtensionP++;
        //Serial.println((String)"Ptension: " + Ptension + "," + Mtension);
      }
    }

    /*
      VtensionP = (float) 2.1763 * Ptension - 1141.5;
      VtensionM = (float) 2.1763 * Mtension - 1141.5;

      Serial.println((String)VtensionP + " " + VtensionM);

      rms = round(VtensionP - VtensionM) / (2 * sqrt(2));

      Serial.println((String)"RMS: " + rms);
    */
    VtensionP = map(Ptension, 377, 672, -321, 321);
    VtensionM = map(Mtension, 377, 672, -321, 321);

    //Serial.println((String)VtensionP + " " + VtensionM);

    rms = (VtensionP - VtensionM) / (2 * sqrt(2));

    Serial.println((String)"RMS2: " + rms);



    //delay(973);
  }

  // map it to the range of the PWM out
  //outputValue = map(sensorValue, 377, 672, -321, 377);
}
