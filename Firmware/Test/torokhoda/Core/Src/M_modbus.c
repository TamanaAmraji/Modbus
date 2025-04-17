/*
 * M_modbus_coil.c
 *
 *  Created on: Feb 1, 2025
 *      Author: PR
 */

#include "M_modbus.h"
#include "usart.h"

uint8_t RxData[10];
uint8_t TxData[11];
int Data[16];

void sendData (uint8_t *data){

HAL_GPIO_WritePin(TX_EN_GPIO_Port, TX_EN_Pin, GPIO_PIN_SET);
HAL_UART_Transmit(uart_ch, data, 11, 1000);
HAL_GPIO_WritePin(TX_EN_GPIO_Port,TX_EN_Pin , GPIO_PIN_RESET);

}

/* modbus function codes:
 * Read discrete inputs				2
 * Read coils						1	t
 * Write single coil				5	t
 * Write multiple coils				15	t
 * Read input registers 			4	t
 * Read multiple holding registers	3
 * Write single holding register	6	t
 * Write multiple holding registers 16
 *
 * examples:

	  writeSingeCoil(10,0,1); 					slave ID, coil address, set or reset

	  readMultipleCoils(10, 0, 8); 				slave ID, starting coil address, number of coils

	  writeMultipleCoils(12, 0,15,0b00110011);	slave ID, start address, coils pattern

	  writeSingleHoldingRegister(9, 12, 0xFAFA); slave ID, register address, data

	//you can input your data as binary(0b), hex(0x) or dec

	  readInputRegisters(4,7, 2); 				slave ID, register start address, no of registers

*/

void writeSingeCoil(uint8_t slave_address, uint8_t coil_addr,bool data ){

	 HAL_UARTEx_ReceiveToIdle_IT(uart_ch, RxData, 10);

	  TxData[0] = slave_address;  // slave address
	  TxData[1] = 0x05;  // Force single coil (Function code)

	  TxData[2] = (coil_addr - (coil_addr%256))/256; 	// coil address high
	  TxData[3] = coil_addr%256;  						// coil address low

	  if (data == 1)  TxData[4] = 0xFF; else  TxData[4] = 0x00;
	  TxData[5] = 0;  // force data low

	  uint16_t crc = crc16(TxData, 6);
	  TxData[6] = crc&0xFF;   // CRC LOW
	  TxData[7] = (crc>>8)&0xFF;  // CRC HIGH
	  sendData(TxData);
}

void writeMultipleCoils(uint8_t slave_address, uint8_t coil_addr,uint8_t n_Coils, uint16_t coilsPattern){

	 HAL_UARTEx_ReceiveToIdle_IT(uart_ch, RxData, 10);

	  TxData[0] = slave_address;  // slave address
	  TxData[1] = 0x0f;  // Function code

	  TxData[2] = (coil_addr - (coil_addr%256))/256; 	// coil address high
	  TxData[3] = coil_addr%256;  						// coil address low

	  TxData[4] = (n_Coils - (n_Coils%256))/256;
	  TxData[5] = n_Coils%256; // up to 255 (for more than 255 coils use TxData[4] too

	  if (n_Coils%8 ==0) TxData[6] = n_Coils/8; else TxData[6] = n_Coils/8 +1;

	  TxData[7] = (coilsPattern - (coilsPattern%256))/256; //use for first 8 coils
	  TxData[8] = coilsPattern; //next 8 coils, ex: 0b10101101 or 0xAD

	  uint16_t crc = crc16(TxData, 6);
	  TxData[9] = crc&0xFF;   // CRC LOW
	  TxData[10] = (crc>>8)&0xFF;  // CRC HIGH
	  sendData(TxData);
}

void readMultipleCoils(uint8_t slave_address, uint8_t start_addr, uint8_t n_Coils){

	 HAL_UARTEx_ReceiveToIdle_IT(uart_ch, RxData, 10);
	 //data: 	 RxData[3] and 	 RxData[2+ n_coils]

	  TxData[0] = slave_address;  // slave address
	  TxData[1] = 0x01;  // Function code

	  TxData[2] = 0;  // coil address high
	  TxData[3] = start_addr;  // coil address low
	  //The starting coil address will be 00000000 00000000 = 0 + 1 = 1 (first coil)

	  TxData[4] = 0x00;
	  TxData[5] = n_Coils;
	  //The no of coils to read will be 00000000 +n_coils  = n_coils/8 bytes

	  uint16_t crc = crc16(TxData, 6);
	  TxData[6] = crc&0xFF;   // CRC LOW
	  TxData[7] = (crc>>8)&0xFF;  // CRC HIGH
	  sendData(TxData);
}

void writeSingleHoldingRegister(uint8_t slave_address, uint16_t address, uint32_t data){

	 HAL_UARTEx_ReceiveToIdle_IT(uart_ch, RxData, 10);

	  TxData[0] = slave_address;	// slave address
	  TxData[1] = 0x06;				// Function code

	  TxData[2] = (address - (address%256))/256;  // coil address high
	  TxData[3] = address%256;  				// coil address low
	  //The starting coil address will be address hex + 40001

	  TxData[4] =(data-(data%256))/256;
	  TxData[5] = (data%256);


	  uint16_t crc = crc16(TxData, 6);
	  TxData[6] = crc&0xFF;   // CRC LOW
	  TxData[7] = (crc>>8)&0xFF;  // CRC HIGH
	  sendData(TxData);

}

void readInputRegisters(uint8_t slave_address, uint16_t startaddress, uint32_t n_registers){

	 HAL_UARTEx_ReceiveToIdle_IT(uart_ch, RxData, 10);
	 //data: RxData[3] ... RxData[2+ n_registers*2]

	  TxData[0] = slave_address;	// slave address
	  TxData[1] = 0x04;				// Function code

	  TxData[2] = (startaddress - (startaddress%256))/256;  // coil address high
	  TxData[3] = startaddress%256;  				// coil address low
	  //The starting coil address will be address hex + 40001

	  TxData[4] =(n_registers-(n_registers%256))/256;	// in Respone we have (registers x 2) data bytes
	  TxData[5] = (n_registers%256);


	  uint16_t crc = crc16(TxData, 6);
	  TxData[6] = crc&0xFF;   // CRC LOW
	  TxData[7] = (crc>>8)&0xFF;  // CRC HIGH
	  sendData(TxData);

}
