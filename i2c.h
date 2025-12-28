/*
 * i2c.h
 *
 *  Created on: Dec 20, 2025
 *      Author: Sanchit
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include <stdint.h>
#include <stm32f4xx.h>


void i2c_init (void);
void i2c_start (void);
void i2c_write (uint8_t data);
void i2c_address(uint8_t address);
void i2c_stop (void);
void i2c_writeMulti(uint8_t *data, uint8_t size);
void i2c_read(uint8_t address, uint8_t *buffer, uint8_t size);

#endif /* INC_I2C_H_ */
