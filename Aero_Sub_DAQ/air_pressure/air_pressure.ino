/*
* Air Pressure Sensor
* Authors:        William Everson         Embedded Firmware Team
*                 Allison Pham
*                 Srinidhi Malladi
* Modified By:    Cristian Cruz
*                 
* Email:          willreverson@gmail.com
* 
* (c) 2022 Dallas Formula Racing - Formula SAE
* GPL-3.0 License
*/

#include <Wire.h>
#include <SD.h>

static constexpr uint8_t SWITCH_PIN = 7; // To be re-implemented
static constexpr uint8_t CHIP_SELECT_PIN = 10;

static constexpr uint8_t kSensorAddr = 0x29;
static constexpr float k2PowerOf24 = 16777216.0f;

static uint32_t sensor_output_bytes[7];
static uint8_t status_byte = 0;

static String file_name;
static File data_file;

void InitSDCard() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(CHIP_SELECT_PIN)) {
    Serial.println("Card failed, or not present");
    while (1) {/*Do nothing*/};
  }
  Serial.println("card initialized.");

  file_name = "aerodata.csv";
  data_file = SD.open(file_name, FILE_WRITE);
  
  data_file.print("Time(ms),Pressure,Temperature\n");
  data_file.close();
}

void setup() {
  Serial.begin(9600); 
  while (!Serial) { /*wait for serial port to connect. Needed for native USB port only*/}
  Wire.begin();
  InitSDCard();
}

void loop() {
  readAirPressureSensor();   

  data_file = SD.open(file_name, FILE_WRITE);

  if(data_file) {
    float air_pressure = pressureTransferFunction(sensor_output_bytes);
    float air_temperature = temperatureTransferFunction(sensor_output_bytes);

    data_file.print(millis());
    data_file.print(",");
    data_file.print(air_pressure);
    data_file.print(",");
    data_file.print(air_temperature);
    data_file.print("\n");
    data_file.flush();

    Serial.print("Pressure: ");
    Serial.print(air_pressure);
    Serial.print("\t\tTemperature: ");
    Serial.println(air_temperature);
  }
  else
    Serial.println("error opening " + file_name);    
    data_file.close();
}

void readAirPressureSensor() {
  Wire.beginTransmission(kSensorAddr);
  Wire.write(0xAA); // ToDo: Assign to a const variable
  Wire.endTransmission();

  Wire.requestFrom (41, 7);
  while(Wire.available() < 7) {
    delay(1);
  }

  status_byte = Wire.read();    // receive a byte as character
  for (int i = 0; i < 7; i++) {
    sensor_output_bytes[i] = Wire.read();
  }
  
}

float pressureTransferFunction(uint32_t sensor_output_bytes[]) {
  // Pressure Output Transfer Function (Pg 3 of Datasheet)
  // Convert Pressure to %Full Scale Span ( +/- 100%)
  uint32_t nominal_air_pressure = (sensor_output_bytes[1] << 16) + (sensor_output_bytes[2] <<8) + sensor_output_bytes[3];
  float transfer_function_numerator = (float)nominal_air_pressure - (0.5 * k2PowerOf24);
  float real_air_pressure = (transfer_function_numerator / k2PowerOf24) * 1.25;
  real_air_pressure *= 0.1;
  return abs(real_air_pressure);
}

float temperatureTransferFunction(uint32_t sensor_output_bytes[]){
  // Temperature Output Transfer Function (Pg 3 of Datasheet)
  // Convert Temperature to degrees C:
  uint32_t nominal_temperature = (sensor_output_bytes[4] << 8) + sensor_output_bytes[5];
  float real_temperature = ((nominal_temperature * 125.0) / k2PowerOf24) - 40.0;
  return abs(real_temperature);
}
