#include "FreeRTOS_demo.h"

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
*                   xTaskCreate()��̬��������API����
*
*      �˺�������ʹ�ö�̬�ķ�ʽ�������������������ƿ��Լ������ջ�ռ�
*  ������ڴ棬���� FreeRTOS �� FreeRTOS ����Ķ��з��䣬��ʹ�ô˺�������
*  Ҫ��FreeRTOSConfig.h�ļ��н���configSUPPORT_DYNAMIC_ALLOCATION����Ϊ 1��
*  �˺�����������������̽������̬��������������������С� 
*
*  ����ԭ�ͣ�
*         BaseType_t xTaskCreate(
*                TaskFunction_t                pxTaskCode,
*                const char *                  const pcName,
*                const configSTACK_DEPTH_TYPE  usStackDepth,
*                void * const                  pvParameters,
*                UBaseType_t                   uxPriority,
*                TaskHandle_t * const          pxCreatedTask);
*
*  ����������
*         pxTaskCode     ָ����������ָ��
*         pcName         ����������󳤶�Ϊ configMAX_TASK_NAME_LEN
*         usStackDepth   �����ջ��С�� ��λ���֣�ע�⣬��λ�����ֽڣ�
*         pvParameters   ���ݸ��������Ĳ���
*         uxPriority     �������ȼ������ֵΪ(configMAX_PRIORITIES-1)
*         pxCreatedTask  ������������ɹ������󣬻᷵������������������
*                        �������������ƿ�
*
*  ��������ֵ��
*         pdPASS                                  ���񴴽��ɹ�
*         errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY   �ڴ治�㣬���񴴽�ʧ��
***************************************************************************/

/************************************************************************
*                       ��̬�뾲̬�������������
*
*  ��̬�������������������ƿ��Լ������ջ�ռ�������ڴ棬���� FreeRTOS 
*               �� FreeRTOS ����Ķ��з��䡣
*  ��̬�������������������ƿ��Լ������ջ�ռ�������ڴ棬��Ҫ���û������ṩ��
*
*  ����ɾ��ʱ�� ��̬�����������������ƿ��Լ������ջ�ռ��ڴ���ɿ�������Ḻ
*              ���ͷţ���̬�����������������ƿ��Լ������ջ�ռ��ڴ�����û�
*              �����ֶ��Լ�ȥ�ͷš�
*************************************************************************/

/**************************************************************
*  ע�⣺һ�㴴�������ǲ��ö�̬��������ķ�ʽ�����ڵ�ѧϰ��Ҳ��
*       ȫ���ö�̬��������ķ�ʽ����������
***************************************************************/

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
        vTaskDelay(500);   /* ��ʱ500��tick */
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
        vTaskDelay(500);   /* ��ʱ500��tick */
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
    while (1)
    {
        printf("��̬��������3\r\n");
        vTaskDelay(500);   /* ��ʱ500��tick */
    }
}

