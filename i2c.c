/*
 * i2c.c
 *
 *  Created on: Dec 20, 2025
 *      Author: Sanchit
 */
#include <stdint.h>
#include <stm32f4xx.h>
#include <i2c.h>

void i2c_init (void){
	/* enable i2c and peripheral clock */

	RCC->APB1ENR |= (1<<21);
	RCC->AHB1ENR |= (1<<1);

	/* set mode alternate function */
	GPIOB->MODER &= ~(3 << 12);
	GPIOB->MODER &= ~(3 << 14);
	GPIOB->MODER |= (2<<12);
	GPIOB->MODER |= (2<<14);

	/* output type */
	GPIOB->OTYPER |= (1<<6);
	GPIOB->OTYPER |= (1<<7);

	/* output speed */
	GPIOB->OSPEEDR |= (3<<12);
	GPIOB->OSPEEDR |= (3<<14);

	/* enable pull up */

	GPIOB->PUPDR |= (1<<12);
	GPIOB->PUPDR |= (1<<14);

	/* set alternate function */
	GPIOB->AFR[0] |= (4<<24);
	GPIOB->AFR[0] |= (4<<28);

	/* reset i2c1 */

	I2C1->CR1 |= (1<<15);
	I2C1->CR1 &= ~(1<<15);

	/* enable peripheral input clock in cr2 */

	I2C1->CR2 = (50<<0);

	/* configure clock control */

	I2C1->CCR |= (285<<0);

	/* configure trise */

	I2C1->TRISE |= (51<<0);

	/* finally enable i2c register */

	I2C1->CR1 |= (1<<0);
	I2C1->CR1 |= (1<<10);

}

void i2c_start (void){

	/* enable start condition */

	I2C1->CR1 |= (1<<8);
	while(!(I2C1->SR1 & (1<<0)));


}

void i2c_write (uint8_t data){
	while(!(I2C1->SR1 & (1<<7))); /* wait for txe bit to set */
	I2C1->DR = data;
	while(!(I2C1->SR1 & (1<<2))); /* wait for BTF to set */

}

void i2c_address(uint8_t address){
	I2C1->DR = address;
	while (!(I2C1->SR1 & (1<<1)));
	uint8_t temp = I2C1->SR1;
	temp = I2C1->SR2;
	(void)temp;
}

void i2c_stop (void){
	I2C1->CR1 |= (1<<9);
}

void i2c_writeMulti(uint8_t *data, uint8_t size){
	while(!(I2C1->SR1 & (1<<7))); /* wait for txe bit to set */
	while(size){
		while(!(I2C1->SR1 & (1<<7))); /* wait for txe bit to set */
		I2C1->DR = (volatile uint32_t) *data++;
		size--;
		while(!(I2C1->SR1 & (1<<2))); /* wait for BTF to set */
	}
}

void i2c_read(uint8_t address, uint8_t *buffer, uint8_t size){

	uint8_t remaining = size;

	if(size==1){
		I2C1->DR = address;
		while (!(I2C1->SR1 & (1<<1)));
		uint8_t temp = I2C1->SR1;
				temp = I2C1->SR2;
				(void)temp;
		I2C1->CR1 |= (1<<9);
		while(!(I2C1->SR1 & (1<<6)));
		buffer[size-remaining] = I2C1->DR;
	}
	else {



	I2C1->DR = address;
	while (!(I2C1->SR1 & (1<<1)));
	uint8_t temp = I2C1->SR1;
	temp = I2C1->SR2;
	(void)temp;

	while(remaining > 2){

		while (!(I2C1->SR1 & (1<<6)));
		buffer[size-remaining] = I2C1->DR;
		I2C1->CR1 |= (1<<10);
		remaining--;
	}
	while (!(I2C1->SR1 & (1<<6)));
	buffer[size-remaining] = I2C1->DR;
	I2C1->CR1 &= ~(1<<10);
	I2C1->CR1 |= (1<<9);
	remaining--;
	while(!(I2C1->SR1 & (1<<6)));
	buffer[size-remaining] = I2C1->DR;
	}
}



