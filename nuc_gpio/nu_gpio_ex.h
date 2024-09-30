/**
  ******************************************************************************
  * @file    nu_gpio_ex.h
  * @brief   This file contains all the function prototypes for
  *          the nu_gpio_ex.c file 
  * @description  This file is the header file of nu_gpio_ex.c .
  * + extend the  NUVOTON mcu GPIO functions.
  ******************************************************************************
  *
*/

#ifndef _NU_GPIO_EX_H_
#define _NU_GPIO_EX_H_
//
include "NUC029xAN.h"//include the NUC029xAN.h file
#include "t_platform.h"
typedef enum{

	NT_GPIO_RESET=0,
	NT_GPIO_SET=1,
}NT_GPIO_Value;

uint32_t nt_gpio_read(GPIO_T* port, uint32_t pin) ;

void nt_gpio_write(GPIO_T* port, uint32_t pin, NT_GPIO_Value value) ;

#endif
