/*
 * M_modbus.h
 *
 *  Created on: Feb 2, 2025
 *      Author: Tamana
 */

#ifndef INC_M_MODBUS_H_
#define INC_M_MODBUS_H_

#include "modbus_crc.h"
#include "stdint.h"
#include "M_modbusConf.h"
#include <stdbool.h>


void sendData (uint8_t *data);
void writeSingeCoil(uint8_t slave_address, uint8_t coil_addr,bool data);
void writeMultipleCoils(uint8_t slave_address, uint8_t coil_addr,uint8_t n_Coils, uint16_t coilsPattern);
void readMultipleCoils(uint8_t slave_address, uint8_t start_addr, uint8_t n_Coils);
void writeSingleHoldingRegister(uint8_t slave_address, uint16_t address, uint32_t data);
void readInputRegisters(uint8_t slave_address, uint16_t startaddress, uint32_t n_registers);
#endif /* INC_M_MODBUS_H_ */
