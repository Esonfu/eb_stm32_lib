#include "t_spi_m.h"


void  t_mspi_destroy(t_spi_t* me)
{
 T_FREE(me);
}

t_spi_t* t_mspi_create(void* arg,  
											Tspi_pincfg_func cfgout_func,
											Tspi_pincfg_func cfgin_func,
											Tspi_delay_func delay_func,
											Tspi_write_func w_func,
                      Tspi_read_func  r_func,
											t_spi_gpio_t *hw_gpio)
{
  t_spi_t *me=(t_spi_t*)T_MALLOC(sizeof(t_spi_t));
	if(me>0){
		me->arg=arg;
		me->arg=arg;
		me->delay_func=delay_func;
		me->hwcfgIn_func=cfgin_func;
		me->hwcfgOut_func=cfgout_func;
		me->write_func=w_func;
		me->read_func=r_func;
		me->hw_gpio=hw_gpio;
	}
	me->write_func(hw_gpio->port_clk,hw_gpio->pin_clk,T_SPI_PIN_RESET);
	/* ����MOSI����Ϊ���ģʽ */
	me->hwcfgOut_func(hw_gpio->port_mosi,hw_gpio->pin_mosi);
	/* ����MISO����Ϊ����ģʽ */
	me->hwcfgIn_func(hw_gpio->port_miso,hw_gpio->pin_miso);
	/* ����CLK����Ϊ���ģʽ */
	me->hwcfgOut_func(hw_gpio->port_clk,hw_gpio->pin_clk);
	/* ����CS����Ϊ���ģʽ */
	me->hwcfgOut_func(hw_gpio->port_cs,hw_gpio->pin_cs);
	return me;
}

void  t_mspi_flash_write(t_spi_t* me,uint8_t data)
{
	 uint8_t i;
	 if(me == NULL || me->write_func == NULL||me->hwcfgOut_func == NULL||NULL==me->hw_gpio){
        return;
    }
	  for(i=0;i<8;i++){
		
      me->write_func(me->hw_gpio->port_clk,me->hw_gpio->pin_clk,T_SPI_PIN_RESET);
      if (data & 0x80){
         me->write_func(me->hw_gpio->port_mosi,me->hw_gpio->pin_mosi,T_SPI_PIN_SET);
			}
      else{
         me->write_func(me->hw_gpio->port_mosi,me->hw_gpio->pin_mosi,T_SPI_PIN_RESET);
			}
        data <<= 1;
			 me->write_func(me->hw_gpio->port_clk,me->hw_gpio->pin_clk,T_SPI_PIN_RESET);
		}
		return;
}

uint8_t t_mspi_flash_read(t_spi_t* me)
{   
	 uint8_t i, data = 0;
	 if(me == NULL || me->write_func == NULL||me->hwcfgOut_func == NULL||NULL==me->hw_gpio){
        return -1;
    } 
	  //me->hwcfgIn_func(me->hw_gpio->port_miso,me->hw_gpio->pin_miso);
    for ( i = 0; i < 8; i++)
    {
        me->write_func(me->hw_gpio->port_clk,me->hw_gpio->pin_clk,T_SPI_PIN_RESET);
        data <<= 1;
        if (me->read_func(me->hw_gpio->port_miso,me->hw_gpio->pin_miso) == T_SPI_PIN_SET)
				{
            data |= 0x01;
				}
        me->write_func(me->hw_gpio->port_clk,me->hw_gpio->pin_clk,T_SPI_PIN_SET);
    }
    return data;
}

void t_mspi_SPI_Select(t_spi_t* me)
{
	 if(me == NULL || me->write_func == NULL||me->hwcfgOut_func == NULL||NULL==me->hw_gpio){
        return ;
    } 
    me->write_func(me->hw_gpio->port_cs,me->hw_gpio->pin_cs,T_SPI_PIN_RESET);
}

void t_mspi_SPI_Deselect(t_spi_t* me)
{
	 if(me == NULL || me->write_func == NULL||me->hwcfgOut_func == NULL||NULL==me->hw_gpio){
        return ;
    } 
    me->write_func(me->hw_gpio->port_cs,me->hw_gpio->pin_cs,T_SPI_PIN_SET);
}

uint32_t t_mspi_id_ReadNorFlash(t_spi_t* me)
{
    uint8_t temp, temp0, temp1, temp2;
		if(me == NULL || me->write_func == NULL||me->hwcfgOut_func == NULL||NULL==me->hw_gpio){
        return -1 ;
    } 
    // ѡ��Flash�豸
    t_mspi_SPI_Select(me);

    // ���Ͷ�ȡ�������������������Flash�豸
    t_mspi_flash_write(me,0x9F);
    temp = t_mspi_flash_read(me);
    temp0 = t_mspi_flash_read(me);
    temp1 = t_mspi_flash_read(me);
    temp2 = t_mspi_flash_read(me);

    // ȡ��ѡ��Flash�豸
    t_mspi_SPI_Deselect(me);

    return temp2 << 24 | temp1 << 16 | temp0 << 8 | temp;
}
