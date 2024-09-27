/**
 * @file   t_smi_physw_lib.h
 * @brief  SMI interface
 * @note   @support  RTL_8211
 */
#ifndef _T_SMI_PHYSW_LIB_H_
#define _T_SMI_PHYSW_LIB_H_

#include "t_platform.h"

/*smi delaytime*/
#define T_PHY_SW_DELAY 20
#define ack_timer 5
typedef enum t_smi_error_code_t
{
  T_SMI_FAILED = -1,
  T_SMI_OK = 0,       /* 0x00000000, OK                                                                   */
  T_SMI_INPUT,        /* 0x00000001, invalid input parameter                                              */
  T_SMI_NULL_POINTER, /* 0x00000002, input parameter is null pointer                                      */
} t_smi_error_code_t;

typedef void (*t_phy_sw_smi_cfg_func)(void *port, uint16_t pin);
typedef void (*t_phy_sw_smi_delay_func)(uint32_t us);
typedef void (*t_phy_sw_smi_write_func)(void *port, uint16_t pin, uint8_t data);
typedef uint8_t (*t_phy_sw_smi_read_func)(void *port, uint16_t pin);

typedef struct
{
  void *mdc_port;
  void *mdio_port;
  uint16_t mdc_pin;
  uint16_t mdio_pin;
} t_phy_sw_smi_port_t;

typedef struct t_phy_sw_smi_lib
{
  void *arg;
  t_phy_sw_smi_cfg_func pin_cfgIn_func;
  t_phy_sw_smi_cfg_func pin_cfgOut_func;
  t_phy_sw_smi_delay_func delay_func;
  t_phy_sw_smi_write_func write_func;
  t_phy_sw_smi_read_func read_func;
  t_phy_sw_smi_port_t *hwport;
} t_phy_sw_smi_t;

t_phy_sw_smi_t *t_phy_sw_smi_create(void *arg,
                        t_phy_sw_smi_cfg_func pin_cfgIn_func,
                        t_phy_sw_smi_cfg_func pin_cfgOut_func,
                        t_phy_sw_smi_delay_func delay_func,
                        t_phy_sw_smi_write_func write_func,
                        t_phy_sw_smi_read_func read_func,
                        t_phy_sw_smi_port_t *hwport);

void t_phy_sw_smi_destroy(t_phy_sw_smi_t *me);

uint32_t t_phy_sw_smi_read(t_phy_sw_smi_t *me,uint32_t phyad, uint32_t regad, uint32_t * data);

uint32_t t_phy_sw_smi_write(t_phy_sw_smi_t *me,uint32_t phyad, uint32_t regad, uint32_t data);

/***********rtl8211fs  smi  interface*******************/
uint8_t rtl82xx_smiRead(t_phy_sw_smi_t *me,uint32_t phyaddr, uint32_t pageaddr,uint32_t regaddr, uint32_t * data);

uint8_t rtl82xx_smiWrite(t_phy_sw_smi_t *me,uint32_t phyaddr, uint32_t pageaddr,uint32_t regaddr, uint32_t * data);


#endif
// End of file


