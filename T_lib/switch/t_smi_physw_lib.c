#include "t_smi_physw_lib.h"

t_phy_sw_smi_t *t_phy_sw_smi_create(void *arg,
                                    t_phy_sw_smi_cfg_func pin_cfgIn_func,
                                    t_phy_sw_smi_cfg_func pin_cfgOut_func,
                                    t_phy_sw_smi_delay_func delay_func,
                                    t_phy_sw_smi_write_func write_func,
                                    t_phy_sw_smi_read_func read_func,
                                    t_phy_sw_smi_port_t *hwport)
{
    t_phy_sw_smi_t *me = (t_phy_sw_smi_t *)malloc(sizeof(t_phy_sw_smi_t));
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

void t_phy_sw_smi_destroy(t_phy_sw_smi_t *me) { free(me); }

void t_smiZbit(t_phy_sw_smi_t *me)
{

    if (me == NULL || me->hwport == NULL)
    {
        return;
    }
    /* change GPIO pin to Output only */
    me->pin_cfgOut_func(me->hwport->mdc_port, me->hwport->mdc_pin);
    me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 0);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    me->delay_func(T_PHY_SW_DELAY);
}

void t_smiReadBit(t_phy_sw_smi_t *me, uint32_t *pdata)
{
    uint32_t u;
    if (me == NULL || me->hwport == NULL || me->delay_func == NULL ||
        me->write_func == NULL || me->read_func == NULL)
    {
        return;
    }
    me->pin_cfgIn_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    me->delay_func(T_PHY_SW_DELAY);
    me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
    u = me->read_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    *pdata = u;
}

void t_smiWriteBit(t_phy_sw_smi_t *me, uint32_t data)
{
    if (me == NULL || me->hwport == NULL || me->delay_func == NULL ||
        me->write_func == NULL || me->read_func == NULL)
    {
        return;
    }

    me->pin_cfgOut_func(me->hwport->mdio_port, me->hwport->mdio_pin);
    if (data)
    { /*write 1*/
        me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 1);
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
        me->delay_func(T_PHY_SW_DELAY);
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    }
    else
    {
        /*write 0*/
        me->write_func(me->hwport->mdio_port, me->hwport->mdio_pin, 0);
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 0);
        me->delay_func(T_PHY_SW_DELAY);
        me->write_func(me->hwport->mdc_port, me->hwport->mdc_pin, 1);
    }
}

uint32_t t_phy_sw_smi_read(t_phy_sw_smi_t *me, uint32_t phyad, uint32_t regad, uint32_t *data)
{
    int i;
    uint32_t readBit;
    if (me == NULL || me->hwport == NULL || me->delay_func == NULL ||
        me->write_func == NULL || me->read_func == NULL)
    {
        return T_SMI_FAILED;
    }
    if ((phyad > 31) || (regad > 31) || (data == NULL))
        return T_SMI_FAILED;
    /*it lock the resource to ensure that SMI opertion is atomic,
     *the API is based on RTL865X, it is used to disable CPU interrupt,
     *if porting to other platform, please rewrite it to realize the same function
     */
    /* 32 continuous 1 as preamble*/
    for (i = 0; i < 32; i++)
        t_smiWriteBit(me, 1);

    /* ST: Start of Frame, <01>*/
    t_smiWriteBit(me, 0);
    t_smiWriteBit(me, 1);

    /* OP: Operation code, read is <10>*/
    t_smiWriteBit(me, 1);
    t_smiWriteBit(me, 0);

    /* PHY Address */
    for (i = 4; i >= 0; i--)
        t_smiWriteBit(me, (phyad >> i) & 0x1);

    /* Register Address */
    for (i = 4; i >= 0; i--)
        t_smiWriteBit(me, (regad >> i) & 0x1);

    /* TA: Turnaround <z0>, zbit has no clock in order to steal a clock to
     *  sample data at clock falling edge
     */
    t_smiZbit(me);
    t_smiReadBit(me, &readBit);

    /* Data */
    *data = 0;
    for (i = 15; i >= 0; i--)
    {
        t_smiReadBit(me, &readBit);
        *data = (*data << 1) | readBit;
    }
    t_smiWriteBit(me, 1);
    t_smiZbit(me);
    return T_SMI_OK;
}

uint32_t t_phy_sw_smi_write(t_phy_sw_smi_t *me, uint32_t phyad, uint32_t regad, uint32_t data)
{
    int i;
    if (me == NULL || me->hwport == NULL || me->delay_func == NULL ||
        me->write_func == NULL || me->read_func == NULL)
    {
        return T_SMI_FAILED;
    }
    if ((phyad > 31) || (regad > 31) || (data > 0xFFFF))
        return T_SMI_FAILED;
    /*it lock the resource to ensure that SMI opertion is atomic,
     *the API is based on RTL865X, it is used to disable CPU interrupt,
     *if porting to other platform, please rewrite it to realize the same function
     */
    /* 32 continuous 1 as preamble*/
    for (i = 0; i < 32; i++)
        t_smiWriteBit(me, 1);

    /* ST: Start of Frame, <01>*/
    t_smiWriteBit(me, 0);
    t_smiWriteBit(me, 1);

    /* OP: Operation code, write is <01>*/
    t_smiWriteBit(me, 0);
    t_smiWriteBit(me, 1);

    /* PHY Address */
    for (i = 4; i >= 0; i--)
        t_smiWriteBit(me, (phyad >> i) & 0x1);

    /* Register Address */
    for (i = 4; i >= 0; i--)
        t_smiWriteBit(me, (regad >> i) & 0x1);

    /* TA: Turnaround <10>*/
    t_smiWriteBit(me, 1);
    t_smiWriteBit(me, 0);

    /* Data */
    for (i = 15; i >= 0; i--)
        t_smiWriteBit(me, (data >> i) & 0x1);
    t_smiZbit(me);
    return T_SMI_OK;
}

uint32_t rtl82xx_smiRead(t_phy_sw_smi_t *me, uint32_t phyaddr, uint32_t pageaddr, uint32_t regaddr)
{
    uint32_t regval = 0;
    t_phy_sw_smi_write(me, phyaddr, 0x1f, pageaddr);

    t_phy_sw_smi_read(me, phyaddr, regaddr, &regval); // 读取指定寄存器

    t_phy_sw_smi_write(me, phyaddr, 0x1f, 0); // change  BMCR

    return regval;
}

uint32_t rtl82xx_smiWrite(t_phy_sw_smi_t *me, uint32_t phyaddr, uint32_t pageaddr, uint32_t regaddr, uint32_t *data)
{
    uint32_t regval = 0;

    t_phy_sw_smi_write(me, phyaddr, 0x1f, pageaddr); // change  pagsr

    t_phy_sw_smi_write(me, phyaddr, regaddr, *data);

    t_phy_sw_smi_read(me, phyaddr, regaddr, &regval);

    t_phy_sw_smi_write(me, phyaddr, 0x1f, 0); // change  BMCR

    return regval;
}
