/**
*
*@brief ��ͬc��
*
*/


#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "stm32f4xx.h"                  // Device header

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"


#define T_MALLOC(size) malloc(size)
#define T_FREE         free


#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_H */