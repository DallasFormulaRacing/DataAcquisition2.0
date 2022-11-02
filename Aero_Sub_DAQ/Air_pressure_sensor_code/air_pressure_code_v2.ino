/*
* Air Pressure Sensor
* Authors:        Allison Pham         Embedded Firmware Team
*                 Srinidhi Malladi
*                 William Everson
* 
* Email:          willreverson@gmail.com
* 
* (c) 2022 Dallas Formula Racing - Formula SAE
* GPL-3.0 License
*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define I2C_ADDRESS 0x29

const int kSwitchPin = 7, kChipSelect = 10;
const float kTwo24 = 16777216.00;
uint8_t cmd[8], StatusByte, cmdbyte;
uint32_t outb[12];
int file_name_num = 1, count;
float time;
String file_name;
File dataFile;
bool switchUsed;

float temperatureTransferFunction(uint32_t outb[]) {
  uint32_t Tmp = 0;
  float fTemp;
  // --- convert Temperature to degrees C:
  Tmp = (outb[4] << 8) + outb[5];
  fTemp = (((float)Tmp*125.0)/kTwo24) - 40.0;
  Serial.print("Temperature: ");
  Serial.println(fTemp);
  return abs(fTemp);
}

float pressureTransferFunction(uint32_t outb[]) {
  uint32_t Prs = 0;
  float fPress, transf;

  // --- convert Pressure to %Full Scale Span ( +/- 100%)
  Prs = (outb[1] << 16) + (outb[2] <<8) + outb[3];
  transf = (float)Prs - (0.5*kTwo24);
  fPress = (transf/kTwo24)*1.25;
  fPress *= 0.1;
  Serial.print("Pressure: ");
  Serial.println(fPress);
  return abs(fPress);
}

void ReadAirPressureSensor() {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0xAA);
  Wire.endTransmission();
  delay(100);

  Wire.requestFrom(I2C_ADDRESS,7);
  char stat = Wire.read();
  char PressureA = Wire.read();
  char PressureB = Wire.read();
  char PressureC = Wire.read();
  char TemperatureA = Wire.read();
  char TemperatureB = Wire.read();
  char TemperatureC = Wire.read();
   
  delay(100);         

  Wire.requestFrom (41, 7);
  while(Wire.available() < 7)
  delay(20);
      
    StatusByte = Wire.read();    // receive a byte as character
  
    outb[0] = Wire.read();     
    outb[1] = Wire.read();    
    outb[2] = Wire.read();    
    outb[3] = Wire.read();    
    outb[4] = Wire.read();    
    outb[5] = Wire.read();    
    outb[6] = Wire.read();    
}

void setup() {
  pinMode(kSwitchPin, INPUT_PULLUP);
  time = 0;
  delay(100);
  Serial.begin(9600); 
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Wire.begin();
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(kChipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  
  file_name = "aerodata.csv";
  while(SD.exists(file_name)){
   Serial.println("File already exists: " + file_name);
   ++file_name_num;    
   file_name = "aerodata" + (String)file_name_num + ".csv";
  }
  Serial.print("New file name: ");
  Serial.println(file_name);
  
  if(dataFile)
    dataFile.close();
  
  dataFile = SD.open(file_name, FILE_WRITE);
  dataFile.print("Time(s),Pressure,Temperature\n");
}

void loop() {
count = 0;
while(digitalRead(kSwitchPin) == HIGH){
  if (count == 0){
      dataFile.close();
      count++;
      switchUsed = true;
  }
  Serial.println("Switch is off");
  delay(250);
}
  
Serial.println("Switch is on");
if(switchUsed){
  setup();
  switchUsed = false;
}
  
  delay(1000);
  ReadAirPressureSensor();

  if(dataFile) {
    dataFile.print(time);
    dataFile.print(",");
    dataFile.print(pressureTransferFunction(outb));
    dataFile.print(",");
    dataFile.print(temperatureTransferFunction(outb));
    dataFile.print("\n");
  }
  else
    Serial.println(file_name + " is not open!");

  delay(100);
  time += 1.1;
}
