#include "t_smi_8367_lib.h"

t_smi_lib *t_smi_create(void *arg,
                        t_smi_cfg_func pin_cfgIn_func,
                        t_smi_cfg_func pin_cfgOut_func,
                        t_smi_delay_func delay_func,
                        t_smi_write_func write_func,
                        t_smi_read_func read_func,
                        t_smi_port_t *hwport)
{
    t_smi_lib *me = (t_smi_lib *)malloc(sizeof(t_smi_lib));
    if (me == NULL || pin_cfgIn_func == NULL || pin_cfgOut_func == NULL ||
        delay_func == NULL || write_func == NULL || read_func == NULL ||
        hwport == NULL)
    {
        return NULL;
    }
    me->arg = arg;
    me->pin_cfgIn_func = pin_cfgIn_func;
    me->pin_cfgOut_func = pin_cfgOut_func;
    me->delay_func = delay_func;
    me->write_func = write_func;
    me->read_func = read_func;
    me->hwport = hwport;
    if (me->pin_cfgOut_func)
    {
        me->pin_cfgOut_func(me->hwport->mdc_port, me->hwport->mdc_pin);
        me->pin_cfgOut_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    }
    return me;
}

void t_smi_destroy(t_smi_lib *me) { free(me); }

uint32_t t_smi_start(t_smi_lib *me)
{
    if (me == NULL || me->hwport == NULL)
    {
        return -1;
    }
    /* change GPIO pin to Output only */
    if (me->pin_cfgOut_func)
    {
        me->pin_cfgOut_func(me->hwport->mdc_port, me->hwport->mdc_pin);
        me->pin_cfgOut_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    }
    /* Initial state: SCK: 0, SDA: 1 */
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 1);
    me->delay_func(T_DELAY);
    /* CLK 1: 0 -> 1, 1 -> 0 */
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->delay_func(T_DELAY);
    /* CLK 2: */
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 0);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 1);
}

uint32_t t_smi_writeBit(t_smi_lib *me, uint16_t signal, uint32_t bitLen)
{
    if (me == NULL || me->hwport == NULL)
    {
        return T_ERR_FAILED;
    }
    for (; bitLen > 0; bitLen--)
    {
        me->delay_func(T_DELAY);

        /* prepare data */
        if (signal & (1 << (bitLen - 1)))
        {
            me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 1);
        }
        else
        {
            me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 0);
        }

        me->delay_func(T_DELAY);
        /* clocking */
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
        me->delay_func(T_DELAY);
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    }
}
uint32_t t_smi_readBit(t_smi_lib *me, uint32_t bitLen, uint32_t *rData)
{
    uint32_t u = 0;
    if (me == NULL || me->hwport == NULL || rData == NULL)
    {
        return T_ERR_FAILED;
    }
    /* change GPIO pin to Input only */
    me->pin_cfgIn_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    for (*rData = 0; bitLen > 0; bitLen--)
    {
        // CLK_DURATION(DELAY);
        me->delay_func(T_DELAY);
        /* clocking */

        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);

        me->delay_func(T_DELAY);

        u = me->read_func(me->hwport->mdio_port, me->hwport->mdio_pin);

        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);

        *rData |= (u << (bitLen - 1));
    }

    /* change GPIO pin to Output only */
    me->pin_cfgOut_func(me->hwport->mdio_port, me->hwport->mdio_pin);
}
uint32_t t_smi_stop(t_smi_lib *me)
{
    if (me == NULL || me->hwport == NULL || me->write_func == NULL ||
        me->delay_func == NULL)
    {
        return T_ERR_FAILED;
    }
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 0);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 1);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);

    /* add a click */
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->delay_func(T_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);

    /* change GPIO pin to Input only */
    me->pin_cfgIn_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    me->pin_cfgIn_func(me->hwport->mdc_port, me->hwport->mdc_pin);
}

uint32_t t_smi_read(t_smi_lib *me, uint32_t mAddrs, uint32_t *rData)
{
    uint32_t rawData = 0, ACK;
    uint8_t con;
    uint32_t ret = T_ERR_OK;
    int pData;
    if (me == NULL || me->hwport == NULL || me->write_func == NULL ||
        me->delay_func == NULL)
    {
        return T_ERR_FAILED;
    }
    if (mAddrs > 0xFFFF)
        return T_ERR_INPUT;

    if (rData == NULL)
        return T_ERR_NULL_POINTER;
    t_smi_start(me);
    t_smi_writeBit(me, 0x0b, 4);
    t_smi_writeBit(me, 0x4, 3);
    t_smi_writeBit(me, 0x01, 1);

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for issuing READ command*/
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, (mAddrs & 0xff), 8); /* Set reg_addr[7:0] */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for setting reg_addr[7:0] */
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, (mAddrs >> 8), 8); /* Set reg_addr[15:8] */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK by RTL8369 */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;
    t_smi_readBit(me, 8, &rawData); /* Read DATA [7:0] */
    *rData = rawData & 0xff;

    t_smi_writeBit(me, 0x00, 1); /* ACK by CPU */

    t_smi_readBit(me, 8, &rawData); /* Read DATA [15: 8] */

    t_smi_writeBit(me, 0x01, 1); /* ACK by CPU */
    *rData |= (rawData << 8);

    t_smi_stop(me);
    return ret;
}

uint32_t t_smi_write(t_smi_lib *me, uint32_t mAddrs, uint32_t rData)
{

    uint8_t con;
    uint32_t ACK;
    uint32_t ret = T_ERR_OK;
    if (me == NULL || me->hwport == NULL || me->write_func == NULL ||
        me->delay_func == NULL)
    {
        return T_ERR_FAILED;
    }

    if (mAddrs > 0xFFFF)
        return T_ERR_INPUT;

    if (rData > 0xFFFF)
        return T_ERR_INPUT;

    t_smi_start(me); /* Start SMI */

    t_smi_writeBit(me, 0x0b, 4); /* CTRL code: 4'b1011 for RTL8370*/

    t_smi_writeBit(me, 0x4, 3); /* CTRL code: 3'b100 */

    t_smi_writeBit(me, 0x0, 1); /* 0: issue WRITE command */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for issuing WRITE command*/
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, (mAddrs & 0xff), 8); /* Set reg_addr[7:0] */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for setting reg_addr[7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, (mAddrs >> 8), 8); /* Set reg_addr[15:8] */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for setting reg_addr[15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, rData & 0xff, 8); /* Write Data [7:0] out */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for writting data [7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_writeBit(me, rData >> 8, 8); /* Write Data [15:8] out */

    con = 0;
    do
    {
        con++;
        t_smi_readBit(me, 1, &ACK); /* ACK for writting data [15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0)
        ret = T_ERR_FAILED;

    t_smi_stop(me);

    return ret;
}
