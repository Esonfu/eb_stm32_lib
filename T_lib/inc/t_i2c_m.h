/**
 *@fileNme:t_i2c_m.h
 *@brief 模拟iic
 *
 *
 *
 */
#ifndef _T_I2C_M_H
#define _T_I2C_M_H
#include "t_platform.h"
#define i2c_delay_time 40  //IIC延时调整

typedef enum{
T_I2C_PIN_RESET=0,
T_I2C_PIN_SET=1
}pin_value_e;
typedef void (*Ti2c_pincfg_func)(void* port,uint16_t pin);
typedef void (*Ti2c_delay_func)(uint32_t us);
typedef void (*Ti2c_write_func)(void* port, uint16_t pin,uint8_t pin_val);
typedef uint8_t (*Ti2c_read_func)(void* port, uint16_t pin);
typedef enum{T_I2C_Block1=0, T_I2C_Block2=2}T_I2C_Block;

typedef struct {
    uint16_t pin_scl;
    uint16_t pin_sda;
	  void* scl_port;
	  void* sda_port;
}t_i2c_gpio_t;

typedef struct {
    Ti2c_pincfg_func hwcfgOut_func;//硬件初始化
	  Ti2c_pincfg_func hwcfgIn_func;
    Ti2c_delay_func delay_func;//延时
	  Ti2c_write_func write_func; 
	  Ti2c_read_func  read_func;
    void* arg;
    t_i2c_gpio_t *hw_gpio;
} t_i2c_t;

t_i2c_t* t_i2c_create(void* arg,  
											Ti2c_pincfg_func cfgout_func,
											Ti2c_pincfg_func cfgin_func,
											Ti2c_delay_func delay_func,
											Ti2c_write_func i2c_w_func,
                      Ti2c_read_func  i2c_r_func,
											t_i2c_gpio_t *hw_gpio);

void    t_i2c_destroy(t_i2c_t* me);										
void    t_i2c_start(t_i2c_t* me);
void    t_i2c_stop(t_i2c_t* me);
int     t_i2c_wait_ack(t_i2c_t* me);
void    t_i2c_send_byte(t_i2c_t* me, uint8_t byte);
uint8_t t_i2c_read_byte(t_i2c_t* me);
																			
int t_i2c_write_gp8312(t_i2c_t* me, uint16_t val);

uint8_t t_i2c_CheckDevice(t_i2c_t *me,uint8_t _Address);


#endif
