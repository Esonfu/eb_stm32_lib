/**
 * @file
 * This file includes the necessary header file for the "smi_interface to RTL8367" module.
 * @note RTL_83XX  API's  inerterface is Actually i2c interface
 */
#ifndef _T_SMI_8367_LIB_H_
#define _T_SMI_8367_LIB_H_

#include "stdio.h"
#include "stdint.h"

/**
 *
 *@file readme  ： Smi interface is actually i2c interface.
 *
 */

/*smi delaytime*/
#define T_DELAY 1
#define ack_timer 5
typedef enum t_error_code_e
{
  T_ERR_FAILED = -1,
  T_ERR_OK = 0,       /* 0x00000000, OK                                                                   */
  T_ERR_INPUT,        /* 0x00000001, invalid input parameter                                              */
  T_ERR_NULL_POINTER, /* 0x00000002, input parameter is null pointer                                      */
} t_error_code_t;

typedef void (*t_smi_cfg_func)(void *port, uint16_t pin);
typedef void (*t_smi_delay_func)(uint32_t us);
typedef void (*t_smi_write_func)(void *port, uint16_t pin, uint8_t data);
typedef uint8_t (*t_smi_read_func)(void *port, uint16_t pin);

typedef struct
{
  void *mdc_port;
  void *mdio_port;
  uint16_t mdc_pin;
  uint16_t mdio_pin;
} t_smi_port_t;

typedef struct _t_smi_lib
{
  void *arg;
  t_smi_cfg_func pin_cfgIn_func;
  t_smi_cfg_func pin_cfgOut_func;
  t_smi_delay_func delay_func;
  t_smi_write_func write_func;
  t_smi_read_func read_func;
  t_smi_port_t *hwport;
} t_smi_lib;

t_smi_lib *t_smi_create(void *arg,
                        t_smi_cfg_func pin_cfgIn_func,
                        t_smi_cfg_func pin_cfgOut_func,
                        t_smi_delay_func delay_func,
                        t_smi_write_func write_func,
                        t_smi_read_func read_func,
                        t_smi_port_t *hwport);

void t_smi_destroy(t_smi_lib *me);

uint32_t t_smi_read(t_smi_lib *me, uint32_t mAddrs, uint32_t *rData);

uint32_t t_smi_write(t_smi_lib *me, uint32_t mAddrs, uint32_t rData);

#endif
// End of file
