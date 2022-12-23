/*************************************************** 
  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>

static constexpr uint8_t kIR1Addr 0x5C;
static constexpr uint8_t kIR2Addr 0x5A;
static constexpr uint8_t kIR3Addr 0x5B;
static constexpr uint8_t kIR4Addr 0x5D;
static Adafruit_MLX90614 temp_sensor;

constexpr int kNumofTempSensors = 4; 

void setup() {
  Serial.begin(9600);
  while (!Serial) {/*Do nothing*/};

  if(!temp_sensor.begin()) {    
    Serial.println("Error connecting to MLX sensors. Check wiring.");
    while(1) {/*Do nothing*/};
  }
 
  //Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}

void loop() {
  temp_sensor.AddrSet(kIR1Addr); 
  Serial.print("IR1: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp1 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp1); Serial.println("*F");

  temp_sensor.AddrSet(kIR2Addr); 
  Serial.print("IR2: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp2 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp2); Serial.println("*F");

  temp_sensor.AddrSet(kIR3Addr); 
  Serial.print("IR3: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp3 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp3); Serial.println("*F");

  temp_sensor.AddrSet(kIR4Addr); 
  Serial.print("IR4: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp4 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp4); Serial.println("*F");

  
  temp_sensor.avgF = ComputeAverageTemperature();
  Serial.print("\n***********Average Temp: "); 
  Serial.print(temp_sensor.avgF); 
  Serial.print("***********\n");
  Serial.println();
}

double ComputeAverageTemperature() {
  float sum = temp_sensor.temp1 + temp_sensor.temp2 + temp_sensor.temp3 + temp_sensor.temp4;
  return sum / kNumofTempSensors;
}
