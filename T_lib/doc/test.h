#ifndef __TEST_H
#define __TEST_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx.h"                  // Device header
#include "T_i2c_m.h"
#include "T_spi_m.h"
extern t_i2c_gpio_t T_gpio[1];
extern t_spi_gpio_t T_spi_gpio[1];
void hal_gpio_incfg(GPIO_TypeDef *gpio_port,uint16_t gpio_pin);
void hal_gpio_outcfg(GPIO_TypeDef *gpio_port,uint16_t gpio_pin);
void hal_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
uint8_t hal_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void i2c_dealy(uint32_t us);
	

#ifdef __cplusplus
}
#endif

#endif /* __TEST_H */