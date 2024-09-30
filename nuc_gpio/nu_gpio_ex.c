#include "nu_gpio_ex.h"
/**
 * @brief 读取指定GPIO引脚的值
 * @param port GPIO端口，这是一个GPIO_T结构体的指针 P0~4
 * @param pin 要读取的引脚，这是一个32位的整数，每一位对应一个引脚，位为1表示选择该引脚BIT1~7
 * @return 引脚的值，如果引脚的状态为高，返回1，否则返回0
 */
uint32_t nt_gpio_read(GPIO_T* port, uint32_t pin) 
{
    return (port->PIN & pin) ? 1 : 0;
}

/**
 * @brief 写入指定GPIO引脚的值
 * @param port GPIO端口，这是一个GPIO_T结构体的指针  P0~4
 * @param pin 要写入的引脚，这是一个32位的整数，每一位对应一个引脚，位为1表示选择该引脚BIT1~7
 * @param value 要写入的值，如果值为1，引脚的状态将被设置为高，如果值为0，引脚的状态将被设置为低 @PinStatus { NT_GPIO_Value }
 */
void nt_gpio_write(GPIO_T* port, uint32_t pin, NT_GPIO_Value value) 
{
    if (value) {
        port->DOUT |= pin;
    } else {
        port->DOUT &= ~pin;
    }
}