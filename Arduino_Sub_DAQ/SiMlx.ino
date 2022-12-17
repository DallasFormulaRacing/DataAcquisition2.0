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
#define IR1_ADDR 0x5C
#define IR2_ADDR 0x5A
#define IR3_ADDR 0x5B
#define IR4_ADDR 0x5D
Adafruit_MLX90614 temp_sensor;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Adafruit MLX90614 test");  

  if(!temp_sensor.begin()) {
    
    Serial.println("Error connecting to MLX sensors. Check wiring.");
    while (1);
    } 
 
  //Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
  
}


void loop() {
  
  temp_sensor.AddrSet(IR1_ADDR); 
  Serial.print("IR1: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp1=temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp1); Serial.println("*F");

  temp_sensor.AddrSet(IR2_ADDR); 
  Serial.print("IR2: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp2=temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp2); Serial.println("*F");

  temp_sensor.AddrSet(IR3_ADDR); 
  Serial.print("IR3: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp3 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp3); Serial.println("*F");

  temp_sensor.AddrSet(IR4_ADDR); 
  Serial.print("IR4: ");
  Serial.print("Ambient = "); Serial.print(temp_sensor.readAmbientTempF()); 
  temp_sensor.temp4 = temp_sensor.readObjectTempF();
  Serial.print("*F\tObject = "); Serial.print(temp_sensor.temp4); Serial.println("*F");

  constexpr int kNumofTempSensors = 4; 
  double compute_average_temp (temp_sensor.avgF=((temp_sensor.temp1+temp_sensor.temp2+temp_sensor.temp3+temp_sensor.temp4)/kNumofTempSensors));
  Serial.print("\n***********Average Temp: "); 
  Serial.print(compute_average_temp); 
  Serial.print("***********\n");
  Serial.println();
}
