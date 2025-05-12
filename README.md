# Modbus-over-RS485-stm32
interfacing modbus protocol over RS485 using stm32 (master and slave)

:heavy_check_mark: This library supports MODBUS RTU  
:heavy_check_mark: Tested on : stm32f103vct6


# HARDWARE   
Designd PCB in Altium Designer: 

<img src="https://github.com/TamanaAmraji/Modbus/blob/main/Document/master.PNG" width="400" height="400"/> <img src="https://github.com/TamanaAmraji/Modbus/blob/main/Document/Master_2D.PNG " width="400" height="400"/>   

Based on [MAX485 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX1487-MAX491.pdf/) (in *Documents* folder) The connections are as follows:  

| MAX485 pin | Connection |
| --- | --- |
| `RO` | USARTx RX pin of micro|
| `RE` | GPIO pin on micro |
| `DE` | Same GPIO pin as `RE` |
| `DI` | USARTx TX pin of micro |
| `GND` | GND pin on micro|
| `A` | pin A of *USB to RS485* converter module |
| `B` | pin B of *USB to RS485* converter module |
| `VCC` | +5VDC |

## Circuit diagram: 
![image](https://github.com/Cube-Electronic/Modbus-over-RS485-stm32/blob/main/Documents/ESP32-Modbus-MAX485-Circuit-.jpg)
 


# Code 

## MASTER

Available modbus function codes in this library are:  
- [ ] Read discrete inputs       | Function code: 2
- [x] Read coils					   | Function code: 1
- [x] Write single coil				| Function code: 5
- [x] Write multiple coils				| Function code: 15
- [x] Read input registers 			| Function code: 4
- [ ] Read multiple holding registers	| Function code: 3
- [x] Write single holding register	| Function code: 6
- [ ] Write multiple holding registers | Function code: 16

for using master library follow these steps:  
### cubeMX:
:one: 	Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.(Project Manager -> Code Generator)  
:two: 	Enable USART (Asynchronous)   
> [!Important]
> The selected Baud Rate in "Parameter Settings" section is important and is usually set to 9600 for Modbus protocol

3️⃣	Enable USART global interrupt (NVIC settings)  
4️⃣: 	Enable a gpio as output for RS485 control pin  
:five:	select your debug mode in sys tab (system core) and generate code 
### cubeIDE  
:six:	Add these files to your *Inc* folder: 

 &emsp;◻️: *M_modbusConf.h*  
 &emsp;◻️: *M_modbus.h*  
 &emsp;◻️: *modbus_crc.h* from *modbus_crc* folder

:seven: 	Add  Add these files to your *Src* folder: 

&emsp;◻️: *M_modbus.c*  
&emsp;◻️: *modbus_crc.c* from *modbus_crc* folder

also include header to your main: 
  ```css
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "M_modbus.h"
/* USER CODE END Includes */       
```
:eight:	Config "M_modbusConf.h" 

*for example: 
The control pin is B12 and UART channel is chennel 2* 

  ```css
#define TX_EN_GPIO_Port			GPIOB			//GPIO_PIN_x
#define TX_EN_Pin			GPIO_PIN_12		//GPIOx
#define uart_ch				&huart2			//&huartx
```

### Functions      
These are some examples of functions you can use in your code:

  ```css

	  writeSingeCoil(10,0,1);                     //slave ID, coil address, set or reset

	  readMultipleCoils(10, 0, 8);                //slave ID, starting coil address, number of coils

	  writeMultipleCoils(12, 0,15,0b00110011);    //slave ID, start address, coils pattern

	  writeSingleHoldingRegister(9, 12, 0xFAFA);  //slave ID, register address, data (you can input your data as binary(0b), hex(0x) or dec)

	  readInputRegisters(4,7, 2);                 //slave ID, register start address, no of registers

```

## SLAVE

Available modbus function codes in this library are:  
- [x] Read discrete inputs       | Function code: 2
- [x] Read coils					   | Function code: 1
- [x] Write single coil				| Function code: 5
- [x] Write multiple coils				| Function code: 15
- [x] Read input registers 			| Function code: 4
- [x] Read multiple holding registers	| Function code: 3
- [x] Write single holding register	| Function code: 6
- [x] Write multiple holding registers | Function code: 16

for using master library follow these steps:  
### cubeMX:
:one: 	Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.(Project Manager -> Code Generator)  
:two: 	Enable USART (Asynchronous)  
> [!Important]
> The selected Baud Rate in "Parameter Settings" section is important and is usually set to 9600 for Modbus protocol

3️⃣:	Enable USART global interrupt (NVIC settings)  
4️⃣: 	Enable a gpio as output for RS485 control pin 
:five:	select your debug mode in sys tab (system core) and generate code  
### cubeIDE  
:six:	Add these files to your *Inc* folder: 

 &emsp;◻️: *S_modbusConf.h*  
 &emsp;◻️: *S_modbus.h*  
 &emsp;◻️: *modbus_crc.h* from *modbus_crc* folder

:seven: 	Add  Add these files to your *Src* folder: 

&emsp;◻️: *S_modbus.c*  
&emsp;◻️: *modbus_crc.c* from *modbus_crc* folder

also include header to your main: 
  ```css
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "S_modbus.h"
/* USER CODE END Includes */       
```
:eight:	Config "S_modbusConf.h" 

*for example: 
The control pin is B12 and UART channel is chennel 2 and the device ID is 9* 

  ```css
#define TX_EN_GPIO_Port			GPIOB			//GPIO_PIN_x
#define TX_EN_Pin			GPIO_PIN_12		//GPIOx
#define uart_ch				huart2			//&huartx
#define SLAVE_ID 			9				//Device slave ID
```
:nine:	Add these 2 parts to your *main.c*

```css

/* USER CODE BEGIN 0 */
uint8_t RxData[256];
uint8_t TxData[256];
int indx = 0;


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (RxData[0] == SLAVE_ID)
	{
		switch (RxData[1]){
		case 0x03:
			readHoldingRegs();
			break;
		case 0x04:
			readInputRegs();
			break;
		case 0x01:
			readCoils();
			break;
		case 0x02:
			readInputs();
			break;
		case 0x10:
			writeHoldingRegs();
			break;
		case 0x06:
			writeSingleReg();
		break;
		case 0x05:
			writeSingleCoil();
		break;
		case 0x0f:
			writeMultiCoils();
		break;
		default:
			modbusException(ILLEGAL_FUNCTION);
			break;
		}
	}

	indx = Size;
	HAL_UARTEx_ReceiveToIdle_IT(&uart_ch, RxData, 256);
}
/* USER CODE END 0 */

```

```css

  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_IT(&uart_ch, RxData, 256);

  /* USER CODE END 2 */

```


# Simulators

If you want to use simulators you can use two slave and master simulaters in this repository (*Simulators* folder) or you can download it from [modbustools](https://www.modbustools.com/download.html) 

> [!NOTE]
> -Select your *USB to RS485* converter module connected port as your slave or master port in 
    simulator  
> -The baud rate should be the selected one in your [cubeMX settings](#cubemx)    
> -In mode section select RTU mode  
> -In Master simulator your Slave ID should be as what you selected for slave in your [code](#cubeide-1)   
> -The other parts of RTU mode in simulator is as what you selected in [cubeMX settings](#cubemx)   *usually set to none parity, 8 bits data with 1 stop bit*  
> -The slave address is what you selected in your [code](#functions)   


# Refrences
 This project was developed with the help of resources from [controllerstech website](https://controllerstech.com/) 

