#include "mbed.h"
#include "Hx711.h"

#include <cstdint>
#include <ios>
#include <iostream>

#define kFactor 2 // it's typcally 2 
#define dout ARDUINO_UNO_D2
#define sclk ARDUINO_UNO_D3
#define Vs   3.3 // input voltage is anywhere between 2.7 - 5.5

const double stepSize = Vs / 16777216.0; // calculates the voltage step so you can convert the adc value back to an analog value
const double poissonRatio = 0.3; // material based so this might change depending on the material we are measuring

/*
    note: 
    the data outputted by the hx711 is a mapped value (ADC) that maps to 0 - 16777216 (2^24)
    inorder to get the voltage difference between the two points you need to multiple the raw value
    by the voltage step size. 
*/

/* 
    strain equations:
    equations are based on full-bridge configurations and equations were obtained from : https://www.hbkworld.com/en/knowledge/resource-center/articles/2023/strain-measurement-basics/strain-gauge-fundamentals/wheatstone-bridge-circuit#!ref_www.hbm.com
    converting strain to mili or micro may be a better idea to get more usefull data. However, when converting to micro or milli strain, you will find that this will accentuate the 
        key:
            - v = poission ratio
            - Vo = voltage difference at the middle of the wheatstone bridge
            - Vs = voltage supply (this is probably gonna be 5v)
            - k = K-factor or gauge factor

    strain = (1/4)*(4/kFactor)*(voltageDelta/5) // torsion strain eq = (1/4) * (4/k) * (Vo/Vs)
    strain = (1/(2(1+poissonRatio))) * (4/kFactor) * (voltageDelta/5) // compression/tension of a bar strain eq = (1/(2(1 + v))) * (4/k) * (Vo/Vs)
    strain = (1/4) * (4/kFactor) * (voltageDelta/5) // bending Beam strain eq = (1/4) * (4/k) * (Vo/Vs)
    strain = (1/(2(1-poissonRatio))) * (4/kFactor) * (voltageDelta/5); // compression/tension(with poisson ratio) of a bar strain eq  = (1/(2(1 - v))) * (4/k) * (Vo/Vs)
    strain = (.25) *(4/kFactor) * (voltageDelta/5); // bending Beam strain eq = (1/4) * (4/k) * (Vo/Vs)
*/

int main()
{
    // holds data from hx711
    int32_t rawValue = 0;

    //variables for average
    double sum = 0;
    double offset = 0;

    // varaibles to hold converted raw data 
    double voltageDelta = 0;
    double strain = 0; // needs to be developed

    Hx711 strainGauge(sclk,dout,128); // default gain is 128 


    // callibration takes a little so you gotta let the strain gauge alone for a few second  
    for(int i = 0; i < 20; i++){
        rawValue = strainGauge.readRaw();
        sum += rawValue; 
    }
    offset = sum / 20; // note this is the offset of the raw value which is a 24 bit adc mapped value
    printf("%lf",offset);

    while (true){
        rawValue = strainGauge.readRaw(); 
        voltageDelta = (rawValue - offset) * stepSize;
        printf("%lf \n", voltageDelta);
        wait_us(100000);
    
    }
}