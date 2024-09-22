#include "test.h"
#include "T_i2c_m.h"
#include "T_spi_m.h"

void hal_gpio_outcfg(GPIO_TypeDef *gpio_port,uint16_t gpio_pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  GPIO_InitStruct.Pin = gpio_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
}

void hal_gpio_incfg(GPIO_TypeDef *gpio_port,uint16_t gpio_pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  GPIO_InitStruct.Pin = gpio_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(gpio_port, &GPIO_InitStruct);
	
}

void i2c_dealy(uint32_t us)
{
	uint8_t i;
	for (i = 0; i < us; i++);
}

void hal_write(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t PinState)
{
HAL_GPIO_WritePin(GPIOx,GPIO_Pin,PinState);
}


uint8_t hal_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  return HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);
  
}

t_i2c_gpio_t T_gpio[1]={{.pin_scl=GPIO_PIN_8,.pin_sda=GPIO_PIN_9,.scl_port=GPIOB,.sda_port=GPIOB}};


t_spi_gpio_t T_spi_gpio[1]={
	{
		.pin_mosi=GPIO_PIN_5,
	  .pin_miso=GPIO_PIN_4,
	  .pin_clk=GPIO_PIN_3,
	  .pin_cs=GPIO_PIN_6,
	  .port_mosi=GPIOB,
		.port_miso=GPIOB,
		.port_clk=GPIOB,
		.port_cs=GPIOG,
  }
};