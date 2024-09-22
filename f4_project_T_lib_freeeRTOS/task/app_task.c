#include "app_task.h"
#include "stdio.h"
#include "usart.h"
/*全局变量*/
QueueHandle_t serial_xQueue;

/*app*/
TaskHandle_t AppTaskCreate_Handle = NULL; /* 创建任务句柄 */   
void AppTaskCreate(void);/* 用于创建任务 */

/*led*/
void LED_Task(void* parameter);
TaskHandle_t LED_Task_Handle = NULL;


/*Serial*/
void Serial_Task(void* parameter);
TaskHandle_t Serial_Task_Handle= NULL;

/*Serial_bordcast*/
void Serial_bordcast_Task(void* parameter);
TaskHandle_t Serial_bordcast_Handle= NULL;


/*Serial_bordcast*/
void Serial_bordcast1_Task(void* parameter);
TaskHandle_t Serial_bordcast1_Handle= NULL;

/*任务创建*/
int app_create(void)
{
	
	serial_xQueue = xQueueCreate(2, sizeof(serial_Packet));
	
	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 

												
	if (serial_xQueue == NULL) {
		printf("EER serial_xQueue");
    }												
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1; 
	while(1);   /* 正常不会执行到这里 */
												
}
	
/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	taskENTER_CRITICAL();           //进入临界区

  xReturn=xTaskCreate((TaskFunction_t )LED_Task, /* 任务入口函数 */
              (const char*    )"LED_Task",/* 任务名字 */
              (uint16_t       )256,   /* 任务栈大小 */
              (void*          )NULL,	/* 任务入口函数参数 */
              (UBaseType_t    )2,	    /* 任务的优先级 */
              (TaskHandle_t*  )&LED_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建 LED_Task 任务成功!\r\n");            
  else
   printf("创建 LED_Task erro!\r\n");
	
	xTaskCreate(Serial_Task,"Serial",512,NULL,3,&Serial_Task_Handle);
	xTaskCreate(Serial_bordcast_Task,"Serial2",512,NULL,3,&Serial_bordcast_Handle);
	xTaskCreate(Serial_bordcast1_Task,"Serial3",512,NULL,3,&Serial_bordcast1_Handle);
	vTaskDelete(AppTaskCreate_Handle);
  taskEXIT_CRITICAL();            //退出临界区
}



/*具体任务*/
void LED_Task(void* parameter)
{
  while(1)
  {
	printf("创建 LED_Task is runing!\r\n");
	vTaskDelay(1000);
	}


}
void Serial_Task(void* parameter){

  serial_Packet data;


  while(1)
  {
   if (xQueueReceive(serial_xQueue, &data, portMAX_DELAY)) 
		 {
            // 处理接收到的数据
			 printf("Serial_Task is runing!\r\n");
			 printf("serial_port:%s,serial_len:%d ",data.ser_data,data.ser_len);
     }
	}


}

void Serial_bordcast_Task(void* parameter){

  serial_Packet data;


  while(1)
  {
   if (xQueueReceive(serial_xQueue, &data, portMAX_DELAY)) 
		 {
            // 处理接收到的数据
			 printf("Serial_bordcast_Task is runing!\r\n");
			 printf("Serial_bordcast_Task:%s,serial_len:%d ",data.ser_data,data.ser_len);
     }
	}


}

void Serial_bordcast1_Task(void* parameter){

  serial_Packet data;


  while(1)
  {
   if (xQueueReceive(serial_xQueue, &data, portMAX_DELAY)) 
		 {
            // 处理接收到的数据
			 printf("Serial_bordcast1_Task is runing!\r\n");
			 printf("Serial_bordcast1_Task:%s,serial_len:%d ",data.ser_data,data.ser_len);
     }
	}


}
