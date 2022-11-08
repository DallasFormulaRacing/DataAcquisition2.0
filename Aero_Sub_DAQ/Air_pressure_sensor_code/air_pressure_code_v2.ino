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
//initialize global variables
const int kSwitchPin = 7, kChipSelect = 10;
const float kTwo24 = 16777216.00;
uint8_t command[8], StatusByte, cmdbyte;
uint32_t sensor_output_bytes[7];
int count;
float time;
String file_name = "aerodata.csv";
File dataFile;
bool switchUsed;

float temperatureTransferFunction(uint32_t sensor_output_bytes[]) {
	uint32_t Tmp = 0;
	float fTemp;
	// --- convert Temperature to degrees C:
	Tmp = (sensor_output_bytes[4] << 8) + sensor_output_bytes[5];
	fTemp = (((float)Tmp*125.0)/kTwo24) - 40.0;
	Serial.print("Temperature: ");
	Serial.println(fTemp);
	return abs(fTemp);
}

float pressureTransferFunction(uint32_t sensor_output_bytes[]) {
	uint32_t Prs = 0;
	float fPress, transf;
	
	// --- convert Pressure to %Full Scale Span ( +/- 100%)
	Prs = (sensor_output_bytes[1] << 16) + (sensor_output_bytes[2] <<8) + sensor_output_bytes[3];
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
	while(Wire.available() < 7) {
	   delay(20);
	}
	
	StatusByte = Wire.read();    // receive a byte as character
	
	sensor_output_bytes[0] = Wire.read();     
	sensor_output_bytes[1] = Wire.read();    
	sensor_output_bytes[2] = Wire.read();    
	sensor_output_bytes[3] = Wire.read();    
	sensor_output_bytes[4] = Wire.read();    
	sensor_output_bytes[5] = Wire.read();    
	sensor_output_bytes[6] = Wire.read();    
}

void setup() {
	pinMode(kSwitchPin, INPUT_PULLUP);
	time = 0;
	delay(100);
	Serial.begin(9600); 
	while (!Serial); // wait for serial port to connect. Needed for native USB port only

	Wire.begin();
	Serial.print("Initializing SD card...");
	
	// see if the card is present and can be initialized:
	if (!SD.begin(kChipSelect)) {
		Serial.println("Card failed, or not present");
		// don't do anything more:
		while (1);
		}
	Serial.println("card initialized.");

	if(dataFile)
		dataFile.close();
	
	dataFile = SD.open(file_name, FILE_WRITE);
	dataFile.print("Time(ms),Pressure(),Temperature()\n");
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
	if(switchUsed)
		setup();
	
	ReadAirPressureSensor();
	
	if(dataFile) {
		dataFile.print(millis());
		dataFile.print(",");
		dataFile.print(pressureTransferFunction(sensor_output_bytes));
		dataFile.print(",");
		dataFile.print(temperatureTransferFunction(sensor_output_bytes));
		dataFile.print("\n");
	}
	else
		Serial.println(file_name + " is not open!");
	
	delay(100);

}
