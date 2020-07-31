const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int tension = 0;
int Ptension[] = {527, 527, 527, 527, 527};
int Mtension[] = {527, 527, 527, 527, 527};
float VtensionP = 0;
float VtensionM = 0;
float rms = 0;
//unsigned int NtensionP = 0;
//unsigned int NtensionM = 0;
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

    //y = 2,1794x - 1148,6

    Ptension[0] = (Ptension[0] + Ptension[1] + Ptension[2] + Ptension[3] + Ptension[4]) / 5;
    Mtension[0] = (Mtension[0] + Mtension[1] + Mtension[2] + Mtension[3] + Mtension[4]) / 5;

    //y = 2.1794x - 1148.6 
    //y = 2.2010x - 1158.1
    VtensionP = (float) 2.201 * Ptension[0] - 1158.1;
    VtensionM = (float) 2.201 * Mtension[0] - 1158.1;

    rms = round(VtensionP - VtensionM) / (2 * sqrt(2));

    Serial.println((String) Ptension[0] + " " + Mtension[0]);
    Serial.println((String)"RMS: " + rms);

    //int Ptension[] = {527, 527, 527, 527, 527};
    //int Mtension[] = {527, 527, 527, 527, 527};

    Serial.println("*********Resetea**********");

  }
  else
  {

    Ptension[0] = 527;
    Ptension[1] = 527;
    Ptension[2] = 527;
    Ptension[3] = 527;
    Ptension[4] = 527;
    Mtension[0] = 527;
    Mtension[1] = 527;
    Mtension[2] = 527;
    Mtension[3] = 527;
    Mtension[4] = 527;

    muestreoV = millis();

    for (int i = 0; i < muestras; i++)
    {
      tension = analogRead(analogInPin);
      //Serial.println((String)"Medición: " + tension);

      if (tension < 527) //Semiciclo negativo
      {
        for (int i = 0; i < 5; i++)
        {
          if (tension < Mtension[i])
          {
            Mtension[i] = tension;
            //Serial.println((String)"Mtension: " + Mtension + "," + Ptension);
            break;
          }
        }
      }
      else // tension >= 527  semiciclo positivo
      {
        for (int i = 0; i < 5; i++)
        {
          if (tension > Ptension[i])
          {
            Ptension[i] = tension;
            //Serial.println((String)"Mtension: " + Mtension + "," + Ptension);
            break;
          }
        }
      }
    }

    tiempoM = millis() - muestreoV;
    //Serial.println((String)"Tiempo de medición:" + tiempoM);
    //Serial.println((String) Ptension[0] + " " + Ptension[1] + " " + Ptension[2] + " " + Ptension[3] + " " + Ptension[4]);
    //Serial.println((String) Mtension[0] + " " + Mtension[1] + " " + Mtension[2] + " " + Mtension[3] + " " + Mtension[4]);


  }


}


//nyquist = muestras*(tiempoM*1000)*100/(frecuencia*2);

//nyquist = ((float)tiempoM*1000 / muestras) / (frecuencia * 2) * 100;

//muestras = muestras*((200-nyquist)/100);



//Serial.println((String)"Calidad: " + nyquist);

//Serial.println((String)"Muestras: " + muestras);




/*
  VtensionP = (float) 2.1763 * Ptension - 1141.5;
  VtensionM = (float) 2.1763 * Mtension - 1141.5;

  Serial.println((String)VtensionP + " " + VtensionM);

  rms = round(VtensionP - VtensionM) / (2 * sqrt(2));

  Serial.println((String)"RMS: " + rms);
*/
/*
  VtensionP = map(Ptension, 377, 672, -321, 321);
  VtensionM = map(Mtension, 377, 672, -321, 321);

  //Serial.println((String)VtensionP + " " + VtensionM);

  rms = (VtensionP - VtensionM) / (2 * sqrt(2));

  Serial.println((String)"RMS2: " + rms);
*/


//delay(973);
//}

// map it to the range of the PWM out
//outputValue = map(sensorValue, 377, 672, -321, 377);
//}
