#include "nu_gpio_ex.h"
/**
 * @brief ��ȡָ��GPIO���ŵ�ֵ
 * @param port GPIO�˿ڣ�����һ��GPIO_T�ṹ���ָ�� P0~4
 * @param pin Ҫ��ȡ�����ţ�����һ��32λ��������ÿһλ��Ӧһ�����ţ�λΪ1��ʾѡ�������BIT1~7
 * @return ���ŵ�ֵ��������ŵ�״̬Ϊ�ߣ�����1�����򷵻�0
 */
uint32_t nt_gpio_read(GPIO_T* port, uint32_t pin) 
{
    return (port->PIN & pin) ? 1 : 0;
}

/**
 * @brief д��ָ��GPIO���ŵ�ֵ
 * @param port GPIO�˿ڣ�����һ��GPIO_T�ṹ���ָ��  P0~4
 * @param pin Ҫд������ţ�����һ��32λ��������ÿһλ��Ӧһ�����ţ�λΪ1��ʾѡ�������BIT1~7
 * @param value Ҫд���ֵ�����ֵΪ1�����ŵ�״̬��������Ϊ�ߣ����ֵΪ0�����ŵ�״̬��������Ϊ�� @PinStatus { NT_GPIO_Value }
 */
void nt_gpio_write(GPIO_T* port, uint32_t pin, NT_GPIO_Value value) 
{
    if (value) {
        port->DOUT |= pin;
    } else {
        port->DOUT &= ~pin;
    }
}