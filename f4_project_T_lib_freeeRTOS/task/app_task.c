#include "app_task.h"
#include "stdio.h"
#include "usart.h"
/*ȫ�ֱ���*/
QueueHandle_t serial_xQueue;

/*app*/
TaskHandle_t AppTaskCreate_Handle = NULL; /* ���������� */   
void AppTaskCreate(void);/* ���ڴ������� */

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

/*���񴴽�*/
int app_create(void)
{
	
	serial_xQueue = xQueueCreate(2, sizeof(serial_Packet));
	
	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 

												
	if (serial_xQueue == NULL) {
		printf("EER serial_xQueue");
    }												
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1; 
	while(1);   /* ��������ִ�е����� */
												
}
	
/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	taskENTER_CRITICAL();           //�����ٽ���

  xReturn=xTaskCreate((TaskFunction_t )LED_Task, /* ������ں��� */
              (const char*    )"LED_Task",/* �������� */
              (uint16_t       )256,   /* ����ջ��С */
              (void*          )NULL,	/* ������ں������� */
              (UBaseType_t    )2,	    /* ��������ȼ� */
              (TaskHandle_t*  )&LED_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("���� LED_Task ����ɹ�!\r\n");            
  else
   printf("���� LED_Task erro!\r\n");
	
	xTaskCreate(Serial_Task,"Serial",512,NULL,3,&Serial_Task_Handle);
	xTaskCreate(Serial_bordcast_Task,"Serial2",512,NULL,3,&Serial_bordcast_Handle);
	xTaskCreate(Serial_bordcast1_Task,"Serial3",512,NULL,3,&Serial_bordcast1_Handle);
	vTaskDelete(AppTaskCreate_Handle);
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/*��������*/
void LED_Task(void* parameter)
{
  while(1)
  {
	printf("���� LED_Task is runing!\r\n");
	vTaskDelay(1000);
	}


}
void Serial_Task(void* parameter){

  serial_Packet data;


  while(1)
  {
   if (xQueueReceive(serial_xQueue, &data, portMAX_DELAY)) 
		 {
            // ������յ�������
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
            // ������յ�������
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
            // ������յ�������
			 printf("Serial_bordcast1_Task is runing!\r\n");
			 printf("Serial_bordcast1_Task:%s,serial_len:%d ",data.ser_data,data.ser_len);
     }
	}


}
