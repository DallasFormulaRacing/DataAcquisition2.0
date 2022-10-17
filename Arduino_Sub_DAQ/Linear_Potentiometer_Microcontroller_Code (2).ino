/*
 *  Linear Potentiometer Microcontroller Code
 * 
 *  By: Basel Jazzar - Suspension Lead
 *  8/25/22
 *  
 *  Dallas Formula Racing - Formula SAE
 *  
 */


#include <SPI.h>
#include <SD.h>
File myFile;

String file_name;
int file_name_num = 1;
int sensorValue1 = -1;
int sensorValue2 = -1;
int sensorValue3 = -1;
int sensorValue4 = -1;

void setup() {

  Serial.begin(9600);
  
  
  while(!Serial){
    ;
  }
  
  Serial.print("Initializing SD card...");
  while (!SD.begin(10)) {
    Serial.println("Initialization failed");
  }
  Serial.println("Initialization done");

  file_name = "data_" + String(file_name_num) + ".csv";
  
  while(SD.exists(file_name)){
    Serial.print("File already exists: " + file_name);
    file_name_num++;    
    file_name = "data_" + String(file_name_num) + ".csv";
  }
  
  // while(analogRead(A0) > 2){
  //   Serial.println(analogRead(A0));
  //   Serial.println("Waiting");
  // }

  
  myFile = SD.open(file_name, FILE_WRITE);
  if (!myFile) {
    Serial.print("error opening " + file_name);
    while(1){
      ;
    }
  }
  
  Serial.println("Success opening: " + file_name);
  
  //delay(5000);
}

void loop() {
  
  sensorValue1 = analogRead(A1);
  // sensorValue2 = analogRead(A2);
  // sensorValue3 = analogRead(A3);
  // sensorValue4 = analogRead(A5);
  myFile.flush();

  // myFile.print(millis());
  // myFile.print("\t");
  myFile.print(sensorValue1);
  // myFile.print("\t");
  // myFile.print(sensorValue2);
  // myFile.print("\t");
  // myFile.print(sensorValue3);
  // myFile.print("\t");
  // myFile.println(sensorValue4);
  
  // Serial.print(millis());
  // Serial.print(", ");
  Serial.print(sensorValue1);
  // Serial.print(", ");
  // Serial.print(sensorValue2);
  // Serial.print(", ");
  // Serial.print(sensorValue3);
  // Serial.print(", ");
  // Serial.println(sensorValue4);

  // if(analogRead(A0) < 2){
  //   myFile.close();
  //   Serial.println("Closed: " + file_name);
  //   setup();
  // }

}
