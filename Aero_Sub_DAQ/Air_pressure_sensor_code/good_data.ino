#include <Wire.h>
#define P1 0x29

void setup() 
{

  delay(100);
  Serial.begin(9600); 
  Wire.begin();
  
}

const float two_24 = 16777216.00;
uint8_t cmd[8];
uint8_t StatusByte, cmdbyte;
uint32_t Prs,Tmp, outb[12];
float  fTemp, fPress, transf;
//int32_t Tmp;
//long double Prs; maybe??
bool pwrON;
//bool Command_Only(uint8_t cmdin ); Does nothing?
/*--------------------------------------------------------*/
void loop()
{

    if (Serial.available() > 0)// Check serial port
    { 
        cmdbyte = Serial.read();
        Serial.print("cmdbyte: ");
        Serial.println(cmdbyte,DEC);
    }
    else 
    {
    cmdbyte = 0; 
    }
    
      digitalWrite(4, HIGH);
      pwrON = true;
 Serial.print("pwrON: ");
 Serial.println(pwrON,DEC);
  delay(1000);
  
  Wire.beginTransmission(P1);
  Wire.write(0xAA);
  Wire.endTransmission();
  delay(100);

  Wire.requestFrom(P1,7);
  char stat = Wire.read();
  char PressureA = Wire.read();
  char PressureB = Wire.read();
  char PressureC = Wire.read();
  char TemperatureA = Wire.read();
  char TemperatureB = Wire.read();
  char TemperatureC = Wire.read();
  
  Serial.print("Stat: ");
  Serial.println(stat,DEC);
  Serial.print("presA: ");
  Serial.println(PressureA,DEC);
  Serial.print("presB: ");
  Serial.println(PressureB,DEC);
  Serial.print("presC: ");
  Serial.println(PressureC,DEC);
  Serial.print("tempA: ");
  Serial.println((uint8_t)TemperatureA,DEC);
  Serial.print("tempB: ");
  Serial.println((uint8_t)TemperatureB,DEC);
  Serial.print("tempC: ");
  Serial.println((uint8_t)TemperatureC,DEC);
 
   
       delay(100);         

    Wire.requestFrom (41, 7);
    while(Wire.available() < 7)
      delay(20);
      
    StatusByte = Wire.read();    // receive a byte as character
  
    outb[0] = Wire.read();     // receive a byte as character
    Serial.print("outb stat: ");
    Serial.println(outb[0], DEC);
    outb[1] = Wire.read();    // receive a byte as character
    Serial.print("outb1 PresA: ");
    Serial.println(outb[1], DEC);
    outb[2] = Wire.read();    // receive a byte as character
    Serial.print("outb2 PresB: ");
    Serial.println(outb[2], DEC);
    outb[3] = Wire.read();    // receive a byte as character
    Serial.print("outb3 PresC: ");
    Serial.println(outb[3], DEC);
    outb[4] = Wire.read();    // receive a byte as character
    Serial.print("outb4 TempA: ");
    Serial.println(outb[4], DEC);
    outb[5] = Wire.read();    // receive a byte as character
    Serial.print("outb5 TempB: ");
    Serial.println(outb[5], DEC);
    outb[6] = Wire.read();    // receive a byte as character
    Serial.print("outb6 TempC: ");
    Serial.println(outb[6], DEC);
   

    Tmp = 0;
 
    // --- convert Temperature to degrees C:
    Tmp = (outb[4] << 8) + outb[5];
    fTemp = (((float)Tmp*125.0)/two_24) - 40.0;
    Serial.print("fTemp: ");
    Serial.println(abs(fTemp));

    Prs = 0;
    // --- convert Pressure to %Full Scale Span ( +/- 100%)
    Prs = (outb[1] << 16) + (outb[2] <<8) + outb[3]; //outb is still a char
    //Prs = outb[1] + outb[2] + outb[3];
    transf = (float)Prs - (0.5*two_24);
    fPress = transf/two_24;
    fPress *= 0.5*1.25;
    
  //Serial.print("Prs: ");
  //Serial.println(Prs); //weird values output from -30k to 6k
    Serial.print("fPress: ");
    Serial.println(abs(fPress));

    delay(100);
    
 
  
}
