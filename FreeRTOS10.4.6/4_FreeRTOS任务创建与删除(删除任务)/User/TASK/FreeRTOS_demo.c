#include "FreeRTOS_demo.h"
#include "key.h"

/*************************************************************
*                       AIP����
*  xTaskCreate()                  ��̬��ʽ��������
*  xTaskCreateStatic()            ��̬��ʽ��������
*  xTaskCreateRestricted()        ��̬��ʽ����ʹ�� MPU ���Ƶ�����
*  xTaskCreateRestrictedStatic()  ��̬��ʽ����ʹ�� MPU ���Ƶ�����
*  vTaskDelete()                  ɾ������
*
*  ע��xTaskCreateRestricted()��xTaskCreateRestrictedStatic()�Ƚ�
*      ���ã�ֻ����Ŀ��Ҫ��������ҪMPU�����Ż�ʹ�ã������ҾͲ�������
*      �����������API�����˽⣬����Ҫ��ʱ�������˽⡣
***************************************************************/


/*************************************************************************
*                  vTaskDelete()ɾ������API����
*
*      �˺�������ɾ���ѱ����������񣬱�ɾ�������񽫱��Ӿ���̬�����б�����̬
*  �����б�����̬�����б���¼��б����Ƴ���Ҫע����ǣ���������Ḻ���ͷű�
*  ɾ����������ϵͳ������ڴ棬�������û�������ɾ��ǰ������ڴ棬����Ҫ���û�
*  ������ɾ��ǰ��ǰ�ͷţ����򽫵����ڴ�й¶����ʹ�ô˺�������Ҫ��FreeRTOSConfig.h
*  �ļ��н���INCLUDE_vTaskDelete����Ϊ 1��
*
*  ����ԭ�ͣ�
*         void vTaskDelete(TaskHandle_t xTaskToDelete);
*
*  ����������
*         xTaskToDelete       ��ɾ�������������
*
*  ��������ֵ�� ��
***************************************************************************/

/********************************************************************
*  ע�⣺
*       1����vTaskDelete()����Ĳ�����NULL�����ʾɾ����ǰ������
*       2����Ҫ�ظ�ʹ��vTaskDelete()ɾ���Ѿ�ɾ�������񣬷���ᵼ��
*          ����������
*********************************************************************/

/**
* ���񴴽���������
* ����: �������ȼ� ��ջ��С ������ ��������
*/
#define START_PRIO      1                     /* �������ȼ� */
#define START_STK_SIZE  64                    /* �����ջ��С */
static TaskHandle_t StartTask_Handler=NULL;   /* ��������������� */
static void StartTaskCreate(void* parameter); /* ����������������� */

/**
* TASK1��������
* ����: �������ȼ� ��ջ��С ������ ��������
*/
#define TASK1_PRIO      1              /* �������ȼ� */
#define TASK1_STK_SIZE  64             /* �����ջ��С */
static TaskHandle_t Task1_Handle=NULL; /* ����1�������� */
static void Task1(void* parameter);    /* ����1���������� */
/**
* TASK2��������
* ����: �������ȼ� ��ջ��С ������ ��������
*/
#define TASK2_PRIO      2              /* �������ȼ� */
#define TASK2_STK_SIZE  64             /* �����ջ��С */
static TaskHandle_t Task2_Handle=NULL; /* ����2�������� */
static void Task2(void* parameter);    /* ����2���������� */
/**
* TASK3��������
* ����: �������ȼ� ��ջ��С ������ ��������
*/
#define TASK3_PRIO      3              /* �������ȼ� */
#define TASK3_STK_SIZE  64             /* �����ջ��С */
static TaskHandle_t Task3_Handle=NULL; /* ����3�������� */
static void Task3(void* parameter);    /* ����3���������� */


/**********************************************************
*@funcName ��App_demo
*@brief    ��ѧϰ������demo
*@param    ��void(��)
*@retval   ��void(��)
*@fn       ��
************************************************************/
void App_demo(void)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ pdPASS���ɹ� */
    
    /* ��̬����������������� */
    xReturn = xTaskCreate(
                        (TaskFunction_t	)StartTaskCreate,	  /* ������ */
                        (const char* 	)"StartTaskCreate",	  /* �������� */
                        (uint32_t 		)START_STK_SIZE,	  /* �����ջ��С */
                        (void* 		  	)NULL,				  /* ���ݸ��������Ĳ��� */
                        (UBaseType_t 	)START_PRIO, 		  /* �������ȼ� */
                        (TaskHandle_t*  )&StartTask_Handler); /* ������ */
                        
    if(xReturn == pdPASS) printf("StartTaskCreate���񴴽��ɹ�!\r\n");
    else                  printf("StartTaskCreate���񴴽�ʧ��!\r\n");
    
    vTaskStartScheduler();  /* ������������� */
}

/**********************************************************
*@funcName ��StartTaskCreate
*@brief    �����ڴ������������
*@param    ��void* parameter(δ�õ�)
*@retval   ��void(��)
*@fn       ��
*            ������񴴽�������ר�����������������ģ���
*        �ǻ���������񴴽����������������񴴽���ɺ���
*        �ǻ����ǻ�Ѹ��������١�
************************************************************/
static void StartTaskCreate(void* parameter)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ pdPASS���ɹ� */
    
    taskENTER_CRITICAL(); /* �����ٽ�������������������Ǳ��뱣֤���ٽ��� */
    
    /* ��̬����Task1���� */
    xReturn = xTaskCreate(
                        (TaskFunction_t	)Task1,		        /* ������ */
                        (const char* 	)"Task1",		    /* �������� */
                        (uint32_t 		)TASK1_STK_SIZE,	/* �����ջ��С */
                        (void* 		  	)NULL,				/* ���ݸ��������Ĳ��� */
                        (UBaseType_t 	)TASK1_PRIO, 		/* �������ȼ� */
                        (TaskHandle_t*  )&Task1_Handle);    /* ������ */

	if(xReturn == pdPASS) printf("Task1���񴴽��ɹ�!\r\n");
	else                  printf("Task1���񴴽�ʧ��!\r\n");
    
    /* ��̬����Task2���� */
    xReturn = xTaskCreate(
                        (TaskFunction_t	)Task2,		        /* ������ */
                        (const char* 	)"Task2",		    /* �������� */
                        (uint32_t 		)TASK2_STK_SIZE,	/* �����ջ��С */
                        (void* 		  	)NULL,				/* ���ݸ��������Ĳ��� */
                        (UBaseType_t 	)TASK2_PRIO, 		/* �������ȼ� */
                        (TaskHandle_t*  )&Task2_Handle);    /* ������ */

	if(xReturn == pdPASS) printf("Task2���񴴽��ɹ�!\r\n");
	else                  printf("Task2���񴴽�ʧ��!\r\n");
    
    /* ��̬����Task3���� */
    xReturn = xTaskCreate(
                        (TaskFunction_t	)Task3,		        /* ������ */
                        (const char* 	)"Task3",		    /* �������� */
                        (uint32_t 		)TASK3_STK_SIZE,	/* �����ջ��С */
                        (void* 		  	)NULL,				/* ���ݸ��������Ĳ��� */
                        (UBaseType_t 	)TASK3_PRIO, 		/* �������ȼ� */
                        (TaskHandle_t*  )&Task3_Handle);    /* ������ */

	if(xReturn == pdPASS) printf("Task3���񴴽��ɹ�!\r\n");
	else                  printf("Task3���񴴽�ʧ��!\r\n");
    
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

/*********************************************************************
*@funcName ��Task1
*@brief    ������1
*@param    ��void* parameter(δ�õ�)
*@retval   ��void(��)
*@fn       ���ö�̬��������ķ�ʽ����������1
**********************************************************************/
static void Task1(void* parameter)
{
    while (1)
    {
        printf("��̬��������1\r\n");
        vTaskDelay(900);   /* ��ʱ500��tick */
    }
}

/*********************************************************************
*@funcName ��Task2
*@brief    ������2
*@param    ��void* parameter(δ�õ�)
*@retval   ��void(��)
*@fn       ���ö�̬��������ķ�ʽ����������2
**********************************************************************/
static void Task2(void* parameter)
{
    while (1)
    {
        printf("��̬��������2\r\n");
        vTaskDelay(900);   /* ��ʱ500��tick */
    }
}

/*********************************************************************
*@funcName ��Task3
*@brief    ������3
*@param    ��void* parameter(δ�õ�)
*@retval   ��void(��)
*@fn       ���ö�̬��������ķ�ʽ����������3
**********************************************************************/
static void Task3(void* parameter)
{
    uint8_t key = 0;
    
    while (1)
    {
        key = KEY_Scan();
        switch(key)
        {
            case 1:  /* KEY0���� */
                if(Task1_Handle != NULL)vTaskDelete(Task1_Handle);
                Task1_Handle = NULL;
                printf("ɾ������1\r\n");
                break;
            case 2:  /* KEY1���� */
                if(Task2_Handle != NULL)vTaskDelete(Task2_Handle);
                Task2_Handle = NULL;
                printf("ɾ������2\r\n");
                break;
            default: break;
        }
        vTaskDelay(10);   /* ��ʱ500��tick */
    }
}

