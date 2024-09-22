eson_t  hal_f4_project

This project named rules eg: project_name_xxx(peripherals)_xxx(RTSOS)

exp;F4_PROJECT_uart_dma_Freertos  IS MEING:这是一个armf4带freertos的工程.

engineering record：

1_移植USART——DMA.
2_移植Freertos操作系统.

//串口数据包接收关系

//接收框图

串口dma中断接收—>xQueue->串口接收线程
                             |
							 V
                      线程处理后用于使用-->转发给其他线程使用		



增加T_LIB（各种协议的模拟）
							 