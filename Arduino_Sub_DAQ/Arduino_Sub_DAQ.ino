/*
* Linear Potentiometers
* Author:        Bazel Jazzar   Suspension Lead
* Modified By:   Manuel De Jesus Contreras            Embedded Firmware Team
* Email:         Manuel.DeJesusContreras@UTDallas.edu
* 
* (c) 2022 Dallas Formula Racing - Formula SAE
* GPL-3.0 License
*/


#include <SPI.h>
#include <SD.h>
File my_file;

bool has_not_close = true;
String file_name;
int file_name_num = 1;

//A2, A3 are front, A0, A1 are rear
int analog_pins[] = {A0, A1, A2, A3};

int analog_values[] = {-1, -1, -1, -1};
int reset_pin = 6;

void setup() {

  Serial.begin(9600);
  has_not_close = true;
  
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
    file_name = "linPotData_" + String(file_name_num) + ".csv";
  }

  pinMode(reset_pin, INPUT_PULLUP);

  while (!digitalRead(reset_pin)){
     Serial.println("Waiting");
  }

  
  my_file = SD.open(file_name, FILE_WRITE);
  if (!my_file) {
    Serial.print("error opening " + file_name);
    while(1){
      ;
    }
  }
  
  
  Serial.println("Success opening: " + file_name);
  my_file.println("Time Millis,rear1,rear2,front1,front2");
}

void loop() {
  if (digitalRead(reset_pin) && has_not_close){
    my_file.print(millis());
    my_file.print(",");
    for (int x = 0; x < sizeof(analog_values)/sizeof(int); x++){
        analog_values[x] = analogRead(analog_pins[x]);
        my_file.print(analog_values[x]);
        my_file.print(",");
    }
    my_file.println();
    my_file.flush();
  } else {
    has_not_close = false;
    Serial.println("reset");
    my_file.close();
    Serial.println("Closed: " + file_name);
    setup();
  }

}
