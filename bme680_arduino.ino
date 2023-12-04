/*
Example for BME680 Sensor with Arduino Uno.
By: RoboticX Team
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <LiquidCrystal_I2C.h>

// Define I2C address and LCD parameters
const int I2C_ADDRESS = 0x27;  // I2C address of the LCD module
const int LCD_COLS = 20;       // Number of LCD columns
const int LCD_ROWS = 4;        // Number of LCD rows

// Create an LCD object
LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD_COLS, LCD_ROWS);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C communication
  Wire.begin();

  while (!Serial);
  Serial.println(F("BME680 async test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }
    // Initialize the LCD and clear the display
  lcd.init();
  lcd.clear();
  
  // Turn ON LCD Backlight
  lcd.backlight();

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  delay(50);
  
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }

  // Read BME-680 Sensor Values
  float temperature = bme.temperature;
  float pressure = bme.pressure / 100.0;
  float humidity = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;
  float approx_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(F(" *C"));

  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(temperature);
  lcd.print(" *C");

  delay(1000);

  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(F(" hPa"));

  lcd.setCursor(0, 1);
  lcd.print("Pressure= ");
  lcd.print(pressure);
  lcd.print(" hPa");

  delay(1000);

  Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println(F(" %"));

  lcd.setCursor(0, 2);
  lcd.print("Humidity = ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(1000);

  Serial.print(F("Gas = "));
  Serial.print(gas);
  Serial.println(F(" KOhms"));

  lcd.setCursor(0, 3);
  lcd.print("Gas = ");
  lcd.print(gas);
  lcd.print(" KOhms");

  delay(1000);

  lcd.clear();

  Serial.print(F("Approx. Altitude = "));
  Serial.print(approx_altitude);
  Serial.println(F(" m"));

  lcd.setCursor(0, 0);
  lcd.print("Approx. Altitude = ");
  lcd.setCursor(0, 1);
  lcd.print(approx_altitude);
  lcd.print(" m");

  Serial.println();
  delay(2000);
  lcd.clear();
}