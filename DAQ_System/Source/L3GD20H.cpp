/**
 * Copyright (c) 2011 Pololu Corporation.  For more information, see
 * 
 * http://www.pololu.com/
 * http://forum.pololu.com/
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
 
 #include "mbed.h"
 #include "L3GD20H.h"

#define GYR_ADDRESS 0xD6

L3GD20H::L3GD20H(PinName sda, PinName scl)
:_L3GD20H_(sda, scl){
    char reg_v;
    _L3GD20H_.frequency(100000);
    
  // Normal power mode, all axes enabled
    reg_v = 0;    
    reg_v |= 0x0F;       
    write_reg(GYR_ADDRESS,L3GD20_CTRL_REG1,reg_v);
    
    printf("yeet");

}



bool L3GD20H::read(short g[3]) {
    char gyr[6];
 
    if (recv(GYR_ADDRESS, L3GD20_OUT_X_L, gyr, 6)) {
    //scale is 1 dps/digit
        g[0] = (gyr[1] << 8 | gyr[0]);  
        g[1] = (gyr[3] << 8 | gyr[2]);
        g[2] = (gyr[5] << 8 | gyr[4]);

 
        return true;
    }
 
    return false;
}

bool L3GD20H::writeRegister(int addr_i2c,int addr_reg, char v){
    return write_reg(addr_i2c, addr_reg, v);
}

I2C& L3GD20H::get_L3GD20H_(){
    return _L3GD20H_;
}

bool L3GD20H::write_reg(int addr_i2c,int addr_reg, char v){
    char data[2] = {static_cast<char>(addr_reg), v}; 
    return L3GD20H::_L3GD20H_.write(addr_i2c, data, 2) == 0;
}

bool L3GD20H::read_reg(int addr_i2c,int addr_reg, char *v){
    char data = addr_reg; 
    bool result = false;
    
    if ((_L3GD20H_.write(addr_i2c, &data, 1) == 0) && (_L3GD20H_.read(addr_i2c, &data, 1) == 0)){
        *v = data;
        result = true;
    }
    return result;
}


bool L3GD20H::recv(char sad, char sub, char *buf, int length){
    if (length > 1) sub |= 0x80;
 
    return _L3GD20H_.write(sad, &sub, 1, true) == 0 && _L3GD20H_.read(sad, buf, length) == 0;
}