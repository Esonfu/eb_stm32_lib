/**
 *@fileNme:t_spi_m.h
 *@brief 模拟spi
 *
 *
 *
 */
#ifndef _T_SPI_M_H
#define _T_SPI_M_H
#include "t_platform.h"
typedef enum{
T_SPI_PIN_RESET=0,
T_SPI_PIN_SET=1
}spi_pin_value_t;

typedef void (*Tspi_pincfg_func)(void* port,uint16_t pin);
typedef void (*Tspi_delay_func)(uint32_t us);
typedef void (*Tspi_write_func)(void* port, uint16_t pin,uint8_t pin_val);
typedef uint8_t (*Tspi_read_func)(void* port, uint16_t pin);


typedef struct {
    uint16_t pin_mosi;
    uint16_t pin_miso;
	  uint16_t pin_clk;
	  uint16_t pin_cs;
	  void* port_mosi;
	  void* port_miso;
	  void* port_clk;
	  void* port_cs;
}t_spi_gpio_t;

typedef struct {
    Tspi_pincfg_func hwcfgOut_func;//硬件初始化
	  Tspi_pincfg_func hwcfgIn_func;
    Tspi_delay_func  delay_func;//延时
	  Tspi_write_func  write_func; 
	  Tspi_read_func   read_func;
    void* arg;
    t_spi_gpio_t *hw_gpio;
} t_spi_t;

t_spi_t* t_mspi_create(void* arg,  
											Tspi_pincfg_func cfgout_func,
											Tspi_pincfg_func cfgin_func,
											Tspi_delay_func delay_func,
											Tspi_write_func w_func,
                      Tspi_read_func  r_func,
											t_spi_gpio_t *hw_gpio);
void    t_mspi_destroy(t_spi_t* me);			
void    t_mspi_flash_write(t_spi_t* me,uint8_t data);	
uint8_t t_mspi_flash_read(t_spi_t* me);

uint32_t t_mspi_id_ReadNorFlash(t_spi_t* me);
#endif
