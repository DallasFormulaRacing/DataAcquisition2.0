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

bool hasntClose = true;
String file_name;
int file_name_num = 1;
int sensorValue1 = -1;
int sensorValue2 = -1;
int sensorValue3 = -1;
int sensorValue4 = -1;

//A2, A3 are front, A0, A1 are rear
int anaPins[] = {A0, A1, A2, A3};

int anaVals[] = {-1, -1, -1, -1};
int rstPin = 6;

void setup() {

  Serial.begin(9600);
  hasntClose = true;
  
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

  pinMode(rstPin, INPUT_PULLUP);

  while (!digitalRead(rstPin)){
  // while(analogRead(A0) > 2){
  //   Serial.println(analogRead(A0));
     Serial.println("Waiting");
  }

  
  myFile = SD.open(file_name, FILE_WRITE);
  if (!myFile) {
    Serial.print("error opening " + file_name);
    while(1){
      ;
    }
  }
  
  
  Serial.println("Success opening: " + file_name);
  myFile.println("Time Millis,rear1,rear2,front1,front2");
  //delay(5000);
}
//int count = 0;
void loop() {
  if (digitalRead(rstPin) && hasntClose){
    myFile.print(millis());
    myFile.print(",");
    for (int x = 0; x < 4; x++){
        anaVals[x] = analogRead(anaPins[x]);
        myFile.print(anaVals[x]);
        myFile.print(",");
        
        //Serial.print(anaVals[x]);
        //Serial.print("\t");
        
    }
    //Serial.println(count);
    //count++;
    myFile.println();
    /*
    sensorValue1 = analogRead(A0);
    sensorValue2 = analogRead(A1);
    sensorValue3 = analogRead(A2);
    sensorValue4 = analogRead(A3);
    */
    myFile.flush();
  
  //  ln(sensorValue1 + "\t" + sensorValue2 + "\t" + sensorValue3 + "/t" + sensorValue4 + ",");
  /*
    
    myFile.print("\t");
    myFile.print(sensorValue1);
    myFile.print("\t");
    myFile.print(sensorValue2);
    myFile.print("\t");
    myFile.print(sensorValue3);
    myFile.print("\t");
    myFile.print(sensorValue4);
    myFile.print("\n");
    */
    
    
    // myFile.print("\t");
    //myFile.print(sensorValue1);
    //myFile.print("\t");
  
    //myFile.print(sensorValue2);
    // myFile.print("\t");
    //myFile.print(sensorValue3);
    // myFile.print("\t");
    //myFile.println(sensorValue4);
    
    // Serial.print(millis());
    // Serial.print(", ");
    //Serial.print(sensorValue1);
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
  } else {
    hasntClose = false;
    Serial.println("reset");
    myFile.close();
    Serial.println("Closed: " + file_name);
    setup();
  }

}
