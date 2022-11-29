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
static constexpr int kSwitchPin = 7, kChipSelect = 10;
static constexpr float kTwo24 = 16777216.00;
static uint32_t sensor_output_bytes[7];
static String file_name = "aerodata.csv";
File dataFile;
static bool switchUsed;

float temperatureTransferFunction(uint32_t sensor_output_bytes[]) {
	uint32_t temperature = 0;
	float final_temperature;
	// --- convert Temperature to degrees C:
	temperature = (sensor_output_bytes[4] << 8) + sensor_output_bytes[5];
	final_temperature = (((float)temperature*125.0)/kTwo24) - 40.0;
	return abs(final_temperature);
}

float pressureTransferFunction(uint32_t sensor_output_bytes[]) {
	uint32_t pressure = 0;
	float final_pressure, transfer_function;
	
	// --- convert Pressure to %Full Scale Span ( +/- 100%)
	pressure = (sensor_output_bytes[1] << 16) + (sensor_output_bytes[2] <<8) + sensor_output_bytes[3];
	transfer_function = (float)pressure - (0.5*kTwo24);
	final_pressure = (transfer_function/kTwo24)*1.25;
	final_pressure *= 0.1;
	return abs(final_pressure);
}

void ReadAirPressureSensor() {
	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(0xAA);
	Wire.endTransmission();        
	
	Wire.requestFrom (41, 7);
	while(Wire.available() < 7) {
	   delay(20);
	}
	for(int i = 0; i <= 6;i++) {
	   sensor_output_bytes[i] = Wire.read();
	}
}

void setup() {
	pinMode(kSwitchPin, INPUT_PULLUP);
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
	
	dataFile = SD.open(file_name, FILE_WRITE);
	dataFile.print("Time(ms),Pressure(),Temperature()\n");
}

void loop() {
	while(digitalRead(kSwitchPin) == HIGH){
		if (!switchUsed){
			dataFile.close();
			switchUsed = true;
		}
		delay(100);
	}
	if(switchUsed){
		setup();
	}
	
	ReadAirPressureSensor();
	
	if(dataFile) {
		dataFile.print(millis());
		dataFile.print(",");
		dataFile.print(pressureTransferFunction(sensor_output_bytes));
		dataFile.print(",");
		dataFile.print(temperatureTransferFunction(sensor_output_bytes));
		dataFile.print("\n");
	}
}
