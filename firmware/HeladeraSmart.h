
const String EstadoWiFi[] = {"Idle", "Error SSID", "Scan completo", "Conectado", "Contraseña incorrecta", "Conección perdida", "Desconectado"};
const String ONOFF [] = {"OFF", "ON"};

//Pinout ESP8266
/*
  D0   3
  D1   1
  D2  16
  D3   5
  D4   4
  D5  14
  D6  12
  D7  13
  D8   0
  D9   2
  D10 15
  D11 13
  D12 12
  D13 14
  D14  4
  D15  5
*/

//Configuración ADC1115
/* Set the voltage range of the ADC to adjust the gain
   Please note that you must not apply more than VDD + 0.3V to the input pins!
   ADS1115_RANGE_6144  ->  +/- 6144 mV
   ADS1115_RANGE_4096  ->  +/- 4096 mV
   ADS1115_RANGE_2048  ->  +/- 2048 mV (default)
   ADS1115_RANGE_1024  ->  +/- 1024 mV
   ADS1115_RANGE_0512  ->  +/- 512 mV
   ADS1115_RANGE_0256  ->  +/- 256 mV
*/
/* Set the inputs to be compared
  ADS1115_COMP_0_1    ->  compares 0 with 1 (default)
  ADS1115_COMP_0_3    ->  compares 0 with 3
  ADS1115_COMP_1_3    ->  compares 1 with 3
  ADS1115_COMP_2_3    ->  compares 2 with 3
  ADS1115_COMP_0_GND  ->  compares 0 with GND
  ADS1115_COMP_1_GND  ->  compares 1 with GND
  ADS1115_COMP_2_GND  ->  compares 2 with GND
  ADS1115_COMP_3_GND  ->  compares 3 with GND
*/

/* Set number of conversions after which the alert pin will be active
   - or you can disable the alert
    ADS1115_ASSERT_AFTER_1  -> after 1 conversion
    ADS1115_ASSERT_AFTER_2  -> after 2 conversions
    ADS1115_ASSERT_AFTER_4  -> after 4 conversions
    ADS1115_DISABLE_ALERT   -> disable comparator / alert pin (default)
*/
/* Set the conversion rate in SPS (samples per second)
   Options should be self-explaining:
    ADS1115_8_SPS
    ADS1115_16_SPS
    ADS1115_32_SPS
    ADS1115_64_SPS
    ADS1115_128_SPS (default)
    ADS1115_250_SPS
    ADS1115_475_SPS
    ADS1115_860_SPS
*/
/* Set continuous or single shot mode:
    ADS1115_CONTINUOUS  ->  continuous mode
    ADS1115_SINGLE     ->  single shot mode (default)
*/
/* Enable or disable latch. If latch is enabled the alarm pin will be active until the
   conversion register is read (getResult functions). If disabled the alarm pin will be
   deactivated with next value within limits.

    ADS1115_LATCH_DISABLED (default)
    ADS1115_LATCH_ENABLED
*/
/* Sets the alert pin polarity if active:

  Enable or disable latch. If latch is enabled the alarm pin will be active until the
  conversion register is read (getResult functions). If disabled the alarm pin will be
  deactivated with next value within limits.

  ADS1115_ACT_LOW  ->  active low (default)
  ADS1115_ACT_HIGH ->  active high
*/
