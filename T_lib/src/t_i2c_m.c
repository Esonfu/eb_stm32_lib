#include "T_i2c_m.h"


void  t_i2c_destroy( t_i2c_t* me){
    T_FREE(me);
}


t_i2c_t* t_i2c_create(void* arg,  
											Ti2c_pincfg_func cfgout_func,
											Ti2c_pincfg_func cfgin_func,
											Ti2c_delay_func delay_func,
											Ti2c_write_func i2c_w_func,
                      Ti2c_read_func  i2c_r_func,
											t_i2c_gpio_t *hw_gpio)
{
  t_i2c_t*me=(t_i2c_t*)T_MALLOC(sizeof(t_i2c_t));
  if(me){
	me->arg=arg;
  me->delay_func=delay_func;
	me->hwcfgIn_func=cfgin_func;
	me->hwcfgOut_func=cfgout_func;
	me->write_func=i2c_w_func;
	me->read_func=i2c_r_func;
	me->hw_gpio=hw_gpio;
	}
	me->hwcfgOut_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl);
	me->hwcfgOut_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda);
  return me;
}


void  t_i2c_start(t_i2c_t* me)
{

    if(me == NULL || me->write_func == NULL || me->delay_func == NULL){
        return;
    }
    me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET);//sd 1
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);//cd 1
    me->delay_func(i2c_delay_time);
    me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_RESET);//sd 0
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET);// cd 0
		me->delay_func(i2c_delay_time);
}

void    t_i2c_stop(t_i2c_t* me)
{
	  if(me == NULL || me->write_func == NULL || me->delay_func == NULL){
        return;
    }
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_RESET);//sd 0
    me->delay_func(i2c_delay_time);
    me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET);// cd 1
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);//sd 1
    me->delay_func(i2c_delay_time);
}





int  t_i2c_wait_ack(t_i2c_t* me)
{
  uint8_t waittime=0;
	uint8_t rack = 0;
  if(me == NULL || me->delay_func == NULL || me->write_func == NULL||me->read_func==NULL){
        return 1;
    }
	
	me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);
  me->delay_func(i2c_delay_time);
	me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET);
  me->delay_func(i2c_delay_time);
	me->hwcfgIn_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda);	
  if (me->read_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda))	/* CPU读取SDA口线状态 */
	{
		rack = 1;
	}
	else
	{
		rack = 0;
	}
  me->hwcfgOut_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda);
	me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET); 
	me->delay_func(i2c_delay_time);
	return rack;
}

/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void t_i2c_ack(t_i2c_t *me)
{
	if(me == NULL || me->delay_func == NULL || me->write_func == NULL||me->read_func==NULL){
        return ;
    }
	
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_RESET);
		me->delay_func(i2c_delay_time);
	  me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET); 
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET); 
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);
  return ;
}


/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void t_i2c_nack(t_i2c_t *me)
{
	
		if(me == NULL || me->delay_func == NULL || me->write_func == NULL||me->read_func==NULL){
        return ;
    }
	
		me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);
		me->delay_func(i2c_delay_time);
	  me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET); 
		me->delay_func(i2c_delay_time);
		me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET); 
		me->delay_func(i2c_delay_time);
	
   return ;
}

void  t_i2c_send_byte(t_i2c_t* me, uint8_t byte)
{
   uint8_t i=0;
   if(me == NULL || me->delay_func == NULL || me->write_func == NULL||me->read_func==NULL){
        return ;
    }
     for(i=0;i<8;i++)
    {
			 me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,(byte&(0x80>>i)));
       me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET); 
       me->delay_func(i2c_delay_time);
			 me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET); 
			 me->delay_func(i2c_delay_time);
    }
}


uint8_t t_i2c_read_byte(t_i2c_t* me)
{
  uint8_t i,dat=0;
      
   if(me == NULL || me->delay_func == NULL || me->write_func == NULL||me->read_func==NULL){
        return 0 ;
    }
	 // me->write_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda,T_I2C_PIN_SET);
    me->hwcfgIn_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda);
		me->delay_func(i2c_delay_time);
    for(i=0;i<8;i++)
    {
			  
        me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_RESET);
        me->delay_func(i2c_delay_time);
        me->write_func(me->hw_gpio->scl_port,me->hw_gpio->pin_scl,T_I2C_PIN_SET);
        me->delay_func(i2c_delay_time);
			  dat<<=1; /* 高位先输出,所以先收到的数据位要左移 */
			  if(me->read_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda)){dat++;}
			  me->delay_func(i2c_delay_time);
    }
    me->hwcfgOut_func(me->hw_gpio->sda_port,me->hw_gpio->pin_sda); 
      
    t_i2c_nack(me);
		
    return dat;	
}

uint8_t t_i2c_24Cxx_read_byte(t_i2c_t* me, uint8_t ReadAddr, T_I2C_Block EE_BlockX){
    uint8_t temp=0; 
    uint8_t Block_Addr_W,Block_Addr_R;
    Block_Addr_W=0xa0+EE_BlockX;
    Block_Addr_R=0xa1+EE_BlockX;
    t_i2c_start(me); 

    t_i2c_send_byte(me, Block_Addr_W); //发送器件地址,写数据

    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    }

    t_i2c_send_byte(me, ReadAddr%256); //发送读取数据地址
    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    } 
    t_i2c_start(me); 
    t_i2c_send_byte(me, Block_Addr_R); //进入接收模式 
    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    }
    temp=t_i2c_read_byte(me); 
    t_i2c_stop(me);//产生一个停止条件 
    return temp;
}


uint8_t t_i2c_24Cxx_write_byte(t_i2c_t* me, uint8_t WriteAddr,uint8_t DataToWrite,T_I2C_Block EE_BlockX)
{
    uint8_t Block_Addr_W;
    Block_Addr_W=0xa0+EE_BlockX;
    
    t_i2c_start(me); 

    t_i2c_send_byte(me, Block_Addr_W); //发送器件地址,写数据 
    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    }
    t_i2c_send_byte(me, WriteAddr%256); //发送写入数据地址
    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    } 
    t_i2c_send_byte(me, DataToWrite); //发送字节 
    if(t_i2c_wait_ack(me)==1)
    {
        return 0;
    } 
    t_i2c_stop(me);//产生一个停止条件 
    me->delay_func(5000); 
    return 1;
}

int t_i2c_write_gp8312(t_i2c_t* me, uint16_t val)
{
    
	  t_i2c_start(me); 
    t_i2c_send_byte(me, 0xb0); 
    if(t_i2c_wait_ack(me)==1)
    {
        return -1;
    }
    t_i2c_send_byte(me, 0x02); 
    if(t_i2c_wait_ack(me)==1)
    {
        return -1;
    } 
    t_i2c_send_byte(me, val&0xff);
    if(t_i2c_wait_ack(me)==1)
    {
        return -1;
    } 
    t_i2c_send_byte(me, val>>8);
    if(t_i2c_wait_ack(me)==1)
    {
        return -1;
    } 
    t_i2c_stop(me);
    
    return 0;
}


uint8_t t_i2c_CheckDevice(t_i2c_t *me,uint8_t _Address)
{
	uint8_t ucAck=1;
	t_i2c_start(me);/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	t_i2c_send_byte(me,_Address | 0x00);
	/* 检测设备的ACK应答 */
  ucAck=t_i2c_wait_ack(me);

	t_i2c_stop(me);			    /* 发送停止信号 */

	return ucAck;
}

