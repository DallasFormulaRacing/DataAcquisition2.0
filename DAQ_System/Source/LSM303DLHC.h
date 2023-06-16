#ifndef __LSM303DLHC_H
#define __LSM303DLHC_H
 
#include "mbed.h"
 
// register addresses
 
#define CTRL_REG1_A       0x20
#define CTRL_REG2_A       0x21
#define CTRL_REG3_A       0x22
#define CTRL_REG4_A       0x23
#define CTRL_REG5_A       0x24
#define CTRL_REG6_A       0x25 // DLHC only
#define REFERENCE_A       0x26
#define STATUS_REG_A      0x27
 
#define OUT_X_L_A         0x28
#define OUT_X_H_A         0x29
#define OUT_Y_L_A         0x2A
#define OUT_Y_H_A         0x2B
#define OUT_Z_L_A         0x2C
#define OUT_Z_H_A         0x2D
 
#define INT1_CFG_A        0x30
#define INT1_SRC_A        0x31
#define INT1_THS_A        0x32
#define INT1_DURATION_A   0x33
#define INT2_CFG_A        0x34
#define INT2_SRC_A        0x35
#define INT2_THS_A        0x36
#define INT2_DURATION_A   0x37
 
#define CRA_REG_M         0x00
#define CRB_REG_M         0x01
#define MR_REG_M          0x02
 
#define OUT_X_H_M         0x03
#define OUT_X_L_M         0x04
#define OUT_Y_H_M         0x07
#define OUT_Y_L_M         0x08
#define OUT_Z_H_M         0x05
#define OUT_Z_L_M         0x06
 
#define SR_REG_M          0x09
#define IRA_REG_M         0x0A
#define IRB_REG_M         0x0B
#define IRC_REG_M         0x0C
   
/** Tilt-compensated compass interface Library for the STMicro LSM303DLm 3-axis magnetometer, 3-axis acceleromter
 * @author Michael Shimniok http://www.bot-thoughts.com/
 *
 * This is an early revision; I've not yet implemented heading calculation and the interface differs from my
 * earlier LSM303DLH; I hope to make this library drop in compatible at some point in the future.
 * setScale() and setOffset() have no effect at this time.
 *
 * @code
 * #include "mbed.h"
 * #include "LSM303DLM.h"
 *
 * LSM303DLM compass(p28, p27);
 * ...
 * int a[3], m[3];
 * ...
 * compass.readAcc(a);
 * compass.readMag(m);
 *
 * @endcode
 */
class LSM303DLHC {
 
    public:
       
        LSM303DLHC(PinName sda, PinName scl);
 
        /** sets the x, y, and z offset corrections for hard iron calibration
         * 
         * Calibration details here:
         *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
         *
         * If you gather raw magnetometer data and find, for example, x is offset
         * by hard iron by -20 then pass +20 to this member function to correct
         * for hard iron.
         *
         * @param x is the offset correction for the x axis
         * @param y is the offset correction for the y axis
         * @param z is the offset correction for the z axis
         */
        void init();
        void setOffset(float x, float y, float z);
        
        /** sets the scale factor for the x, y, and z axes
         *
         * Calibratio details here:
         *  http://mbed.org/users/shimniok/notebook/quick-and-dirty-3d-compass-calibration/
         *
         * Sensitivity of the three axes is never perfectly identical and this
         * function can help to correct differences in sensitivity.  You're
         * supplying a multipler such that x, y and z will be normalized to the
         * same max/min values
         */
        void SetScale(float x, float y, float z);
 
        /** read the calibrated accelerometer and magnetometer values
         *
         * @param a is the accelerometer 3d vector, written by the function
         * @param m is the magnetometer 3d vector, written by the function
         */
        void Read(int a[3], int m[3]);
        
        /** read the calibrated accelerometer values
         *
         * @param a is the accelerometer 3d vector, written by the function
         */
        void ReadRawAcceleration(int a[3]);
 
        /** read the calibrated magnetometer values
         *
         * @param m is the magnetometer 3d vector, written by the function
         */
        void ReadRawMagnetometer(int m[3]);
        
        void computeAcc(int in[3], float out[3], float gravity);

        double calibrate();
     
    private:
        I2C device_;
        char data_[6];
        int offset_[3];
        int scale_[3];
};
 
#endif
