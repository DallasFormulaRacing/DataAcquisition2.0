/*
* DataAcquisition2.0 — 2023 Vehicle
* (c) 2022 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/


// Standard Libraries
#include <stdio.h>
#include <inttypes.h>

#include <memory>

// ST HAL Dependencies
#include "gpio.h"

#include "usart.h"
extern UART_HandleTypeDef huart3;

#include "adc.h"
extern ADC_HandleTypeDef hadc1;

#include "can.h"
extern CAN_HandleTypeDef hcan1;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
CAN_FilterTypeDef CAN;

#include "i2c.h"
extern I2C_HandleTypeDef hi2c1;

// DFR Custom Dependencies
#include "app.hpp"
#include "../../Core/Inc/retarget.h"
#include "Sensor/LinearPotentiometer/ilinear_potentiometer.hpp"
#include "Sensor/LinearPotentiometer/sls1322.hpp"
#include "Sensor/Accelerometer/lsm303dlhc.hpp"
#include "Sensor/GyroScope/igyroscope.hpp"
#include "Sensor/GyroScope/l3gd20h.hpp"


// CAN variables
uint8_t TxData[8] = {0};
uint8_t RxData[8];
uint32_t TxMailbox;
uint32_t CanID;
float DecodeDataFrame[8] = {0};
bool CANMessageReceivedFlag = false;

/*
	this is the CAN ID list for the PE3 8400 which can also be viewed from the link below
	https://pe-ltd.com/assets/AN400_CAN_Protocol_C.pdf
	the link also tells you what information is inside each of the CAN ID
	NOTE: the Id list is in the same order as the application note from top to bottom

*/
constexpr uint32_t CanIDList[16] = {0x0CFFF048, 0x0CFFF148, 0x0CFFF248, 0x0CFFF348,
						  	  	  	0x0CFFF448, 0x0CFFF548, 0x0CFFF648, 0x0CFFF748,
									0x0CFFF848, 0x0CFFF948, 0x0CFFFA48, 0x0CFFFB48,
									0x0CFFFC48, 0x0CFFFD48, 0x0CFFFE48, 0x0CFFD048 };

// Gyroscope variables
short *GyroData; // first element is X-axis, the second element is the Y-axis, and the third element is the z-axis.


// CallBack functions


					//Can CallBack
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{

    if (HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
    	CANMessageReceivedFlag = true;
    	CanID = RxHeader.ExtId;


    }
}

// function prototypes

void canFilterSetup(int FilterBankNum, uint32_t FilterExtID );
void canParseDataFrame();
void canDecodeDataFrame(float DecodeDataFrame[],int DataSet );

void cppMain() {

	short *GyroData; // first element is X-axis, the second element is the Y-axis, and the third element is the z-axis.
	
	// initializing all filters
	canFilterSetup(1,CanIDList[0]);
	canFilterSetup(2,CanIDList[1]);
	canFilterSetup(3,CanIDList[2]);
	canFilterSetup(4,CanIDList[3]);
	canFilterSetup(5,CanIDList[4]);
	canFilterSetup(6,CanIDList[5]);
	canFilterSetup(7,CanIDList[6]);
	canFilterSetup(8,CanIDList[7]);
	canFilterSetup(9,CanIDList[8]);
	canFilterSetup(10,CanIDList[9]);
	canFilterSetup(11,CanIDList[10]);
	canFilterSetup(12,CanIDList[11]);
	canFilterSetup(13,CanIDList[12]);
	canFilterSetup(14,CanIDList[13]);
	canFilterSetup(15,CanIDList[14]);
	canFilterSetup(16,CanIDList[15]);

	HAL_CAN_Start(&hcan1); // starts the CAN bus
	HAL_CAN_ActivateNotification( &hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // activate RX message interrupts


	// Enable `printf()` using USART
	RetargetInit(&huart3);

	std::unique_ptr<sensor::ILinearPotentiometer> lin_pot(nullptr);
	lin_pot = std::make_unique<sensor::SLS1322>(hadc1);
	std::unique_ptr<sensor::IAccelerometer> accelerometer(nullptr);
	accelerometer = std::make_unique<sensor::LSM303DLHC>(hi2c1);

	float* AccelerometerData = 0;
	accelerometer ->init();

	std::unique_ptr<sensor::IGyroscope> gyroscope(nullptr);
	gyroscope = std::make_unique<sensor::L3GD20H>(hi2c1);


	float displacement_inches = 0.0f;

	for(;;) {
		// HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
		// HAL_GPIO_TogglePin(GPIOB, LD3_Pin);

		accelerometer ->ComputeAcceleration();
		AccelerometerData = accelerometer -> GetAcceleration();

		printf("the x-axis is %lf \t\t " , AccelerometerData[0]);
		printf("the y-axis is %lf \t\t " , AccelerometerData[1]);
		printf("the z-axis is %lf " , AccelerometerData[2]);
		printf("\r");
		printf("\n");

		displacement_inches = lin_pot->DisplacementInches();
		//HAL_Delay(250);
		//printf("\n Percentage: %f", displacement_inches);
		GyroData = gyroscope -> DegreesPerSecond();
		printf("x = %hd\t",GyroData[0]);
		printf("y = %hd\t",GyroData[1]);
		printf("z = %hd\t",GyroData[2]);
		printf("\n");
		printf("\r");


		//printf("\n Percentage: %f", displacement_inches);
		//HAL_Delay(150);
		if(CANMessageReceivedFlag){
			__disable_irq();
			canParseDataFrame();
			if(CanID == 0x0CFFF148){
				printf("\t\t 0x0CFFF148 \n");
				printf("\r");
				printf("RPM = %.2f ", DecodeDataFrame[0]);
				printf("TPS = %.2f ", DecodeDataFrame[2]);
				printf("Fuel Open Time = %.2f ", DecodeDataFrame[4]);
				printf("Ignition Angle = %.2f \n", DecodeDataFrame[6]);
				printf("\r");
			}
			if(CanID == 0x0CFFF548){
				printf("\t\t 0x0CFFF548 \n");
				printf("\r");
				printf("Battery Volt = %.2f ", DecodeDataFrame[0]);
				printf("Air Temp = %.2f ", DecodeDataFrame[2]);
				printf("Coolant Temp = %.2f ", DecodeDataFrame[4]);
				printf("Temp Type = %.2f \n", DecodeDataFrame[6]);
				printf("\r");
			}
			CANMessageReceivedFlag = false; // reseting the flag
			__enable_irq();
		}
	}
}


void canFilterSetup(int FilterBankNum, uint32_t FilterExtID ){

	// Filter initialization
	CAN.FilterIdHigh = FilterExtID >> 13 ;
	CAN.FilterIdLow = ((FilterExtID & 0x1FFF) <<3) | CAN_ID_EXT | CAN_RTR_DATA ;
	CAN.FilterMaskIdHigh = 0xFFFF;
	CAN.FilterMaskIdLow = 0xFFFF;
	CAN.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN.FilterBank = FilterBankNum;
	CAN.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN.FilterActivation = CAN_FILTER_ENABLE;
	CAN.SlaveStartFilterBank = 17;
	HAL_CAN_ConfigFilter(&hcan1,&CAN);
}

void canParseDataFrame(){

	/*
	 *
	 the table below demonstrates the organization of data in each data type. This is specifically used for the canDecodeDataFrame() in which the data type of the particular
	 id you are decoding is required.

	 	 Data type | DLC | Data Breakdown | Length			data type | DLC | Data breakdown | length			Data type | DLC | Data Breakdown | Length
	 	 -------------------------------------------	    -------------------------------------------			-------------------------------------------
	 	 	 1	   |  8  |     1 - 2      | 2 bytes				 3	  |  8  | 	     1       | 1 byte			     4    |  7  |	  1 - 2 	 | 2 bytes
	  	  	  	   |	 |     3 - 4 	  | 2 bytes				      |     |        2		 | 1 byte					  |     |     3 - 4		 | 2 bytes
	  	  	  	   |	 |     5 - 6 	  | 2 bytes				      |     |        3		 | 1 byte					  |     |     5 - 6		 | 2 bytes
	  	  	  	   |	 |     7 - 8 	  | 2 bytes				      |     |        4		 | 1 byte					  |     |     	7	 	 | 1 bytes
	  	 --------------------------------------------			      |     |        5		 | 1 byte			--------------------------------------------
	  	 	 2	   |  6  |     1 - 2      | 2 bytes				      |     |        6		 | 1 byte				 5    |  8  | 	  1 - 2      | 2 bytes
	  	  	  	   |	 |     3 - 4 	  | 2 bytes				      |     |        7		 | 1 byte					  |     |     3 - 4      | 2 bytes
	  	  	  	   |	 |     5 - 6 	  | 2 bytes				      |     |        8		 | 1 byte					  |		|       5        | 1 byte
	  	 --------------------------------------------		------------------------------------------					  |		|		6		 | 1 byte
																														  |		|       7 		 | 1 byte
																														  |		|		8		 | 1 byte
	*/
	switch (CanID){
		case 0x0CFFF048:
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[2] *= .1;
			DecodeDataFrame[4] *= .1;
			DecodeDataFrame[6] *= .1;

		break;
		case 0x0CFFF148:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 4);
			DecodeDataFrame[0] *=.01;
			DecodeDataFrame[2] *=.01;
			DecodeDataFrame[4] *=.01;
		break;
		case 0x0CFFF248:
			//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.001;
			DecodeDataFrame[2] *= 0.001;
			DecodeDataFrame[4] *= 0.001;
			DecodeDataFrame[6] *= 0.001;
		break;
		case 0x0CFFF348:
			//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.001;
			DecodeDataFrame[2] *= 0.001;
			DecodeDataFrame[4] *= 0.001;
			DecodeDataFrame[6] *= 0.001;

		break;
		case 0x0CFFF448:
			//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.2;
			DecodeDataFrame[2] *= 0.2;
			DecodeDataFrame[4] *= 0.2;
			DecodeDataFrame[6] *= 0.2;
		break;
		case 0x0CFFF548:
			//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 4);
			DecodeDataFrame[0] *= .01;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
		break;
		case 0x0CFFF648:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 5);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
		break;
		case 0x0CFFF748:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[6] *= .1;
		break;
		case 0x0CFFF848:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 2);
			DecodeDataFrame[0] *= 0.01;
			DecodeDataFrame[2] *= 0.01;
			DecodeDataFrame[4] *= 0.01;
		break;
		case 0x0CFFF948:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 3);
			for(int n = 0 ; n <= 7 ; n++){
				DecodeDataFrame[n] *=.5;
			}
		break;
		case 0x0CFFFA48:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 2);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
		break;
		case 0x0CFFFB48:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
			DecodeDataFrame[6] *= 0.1;
		break;
		case 0x0CFFFC48:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
			DecodeDataFrame[6] *= 0.1;
		break;
		case 0x0CFFFD48:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
			DecodeDataFrame[6] *= 0.1;
		break;
		case 0x0CFFFE48:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			// bytes 3 and 4 don't seem to be used
		break;
		case 0x0CFFD048:
		//enter decode function
			canDecodeDataFrame(DecodeDataFrame, 1);
			DecodeDataFrame[0] *= 0.1;
			DecodeDataFrame[2] *= 0.1;
			DecodeDataFrame[4] *= 0.1;
			DecodeDataFrame[6] *= 0.1;
		break;
	}

}

// this function will not incorporate resolution into the equation since resolution changes depending on the message id
void canDecodeDataFrame(float DecodeDataFrame[],int DataSet ){
float Num = 0;

/*
  the equation used to decode the data frame is

  Num = HighByte*256+LowByte
	if (Num>32767) then
	Num = Num - 65536

	The equation was taken from the application note for the pe3 performance ECU (AN400 Rev C– Application Note)

*/
//resets the array for each iteration of decoding
for(int n = 0; n <= 7; n++){

	DecodeDataFrame[n] = 0;
}

	switch (DataSet){
		case 1: // 4 unique data values 2 bytes each
			for(int n = 0; n <= 6 ; n += 2){
				Num = RxData[n + 1] * 256 + RxData[n];
				if(Num > 32767){
						Num = Num - 65536;
				}
				DecodeDataFrame[n] = Num; // NOTE: due to how to for loop is setup you will have the data stored into even elements (including 0)
			}
		break;

		case 2: // 3 unique data values 2 bytes each ( 6 DLC instead of 8)
			for(int n = 0; n <= 4 ; n += 2){
				Num = RxData[n + 1] * 256 + RxData[n];
				if(Num > 32767){
						Num = Num - 65536;
				}
				DecodeDataFrame[n] = Num;
			}
		break;

		case 3: // 8 unique data values 1 byte each
			for(int n = 0; n <= 7; n++){
				DecodeDataFrame[n] = RxData[n];
			}
		break;

		case 4: // 4 unique data values 3 are 2 bytes and 1 is 1 byte (DLC is 7 instead of 8)
			for(int n = 0; n <= 4 ; n += 2){
				Num = RxData[n + 1] * 256 + RxData[n];
				if(Num > 32767){
						Num = Num - 65536;
				}
				DecodeDataFrame[n] = Num;
			}
			DecodeDataFrame[6] = RxData[6]; // placed at the 7th frame to be consistent with the odd spacing in the array
		break;

		case 5: // 6 unique data values 2 are 2 bytes and 4 are 1 byte
			// changed the naming scheme for the n value to add emphais what data the for loop is manipulating
			for(int TwoByteData = 0; TwoByteData <= 2 ; TwoByteData += 2){
				Num = RxData[TwoByteData + 1] * 256 + RxData[TwoByteData];
				if(Num > 32767){
						Num = Num - 65536;
				}
				DecodeDataFrame[TwoByteData] = Num;
			}
			for(int OneByteData = 4; OneByteData <=7; OneByteData++){
				DecodeDataFrame[OneByteData] = RxData[OneByteData];
			}
		break;
	}
}
