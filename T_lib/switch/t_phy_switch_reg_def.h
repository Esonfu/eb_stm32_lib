/**
 * @file
 * This file to discription Switching&phy Register
 * @note
 * +RTL8367
 * +RTL8211
 * +RTL8305
 */
#ifndef _T_PHY_SWITCH_REG_DEF_H_
#define _T_PHY_SWITCH_REG_DEF_H_

/*RTL8367*/

/*RTL8211FS*/

/*RTL8305NB*/ /*fiber app note*/
#define GLOBAL_CONTROL_REG_ADD 0x08
#define GLOBAL_CONTROL_PAGSR 0x1f
#define MAC_PAGR_MODE 0x0000
#define PHY_PAGR_MODE 0x8000
#define FIBER_REG 0x1c
#define PAGR_MODE_EN 0x0001 // EN fiber
#define PAGR_MODE_DIS_EN 0x0000
/******PORT********/
typedef enum
{
    PORT0_PHY_ADDR = 0x00,
    PORT1_PHY_ADDR = 0x02,
    PORT2_PHY_ADDR = 0x05,
    PORT3_PHY_ADDR = 0x06,
    PORT4_PHY_ADDR = 0x07,
} t_rtl_8305_port;

#endif