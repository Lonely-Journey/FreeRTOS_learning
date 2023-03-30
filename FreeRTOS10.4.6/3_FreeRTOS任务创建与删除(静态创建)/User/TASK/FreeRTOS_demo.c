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
*                 xTaskCreateStatic()��̬��������API����
*
*      �˺�������ʹ�þ�̬�ķ�ʽ�������������������ƿ��Լ������ջ�ռ�
*  ������ڴ棬��Ҫ���û������ṩ����ʹ�ô˺�������Ҫ��FreeRTOSConfig.h��
*  ���н���configSUPPORT_STATIC_ALLOCATION ����Ϊ1���˺����������������
*  �̽������̬��������������������С�
*
*  ע�⣺����FreeRTOSConfig.h�ļ��н���configSUPPORT_STATIC_ALLOCATION��
*        ��Ϊ1�󣬲��ۺ�configSUPPORT_DYNAMIC_ALLOCATION����Ϊ��ֵ��ϵͳ
*        ������ʹ�ö�̬��ʽ�����ڴ棬�����Ҫ�û��ṩ�����ṩ������������
*        ��ʱ������������������������ʱ�����ڴ�������ص���������������
*        �������ֱ�Ϊ��
*            vApplicationGetIdleTaskMemory()
*            vApplicationGetTimerTaskMemory()
*
*  ����ԭ�ͣ�
*         TaskHandle_t xTaskCreateStatic(
*                TaskFunction_t                pxTaskCode,
*                const char * const            pcName,
*                const uint32_t                ulStackDepth,
*                void * const                  pvParameters,
*                UBaseType_t                   uxPriority,
*                StackType_t * const           puxStackBuffer,
*                StaticTask_t * const          pxTaskBuffer);
*
*  ����������
*         pxTaskCode     ָ����������ָ��
*         pcName         ����������󳤶�Ϊ configMAX_TASK_NAME_LEN
*         ulStackDepth   �����ջ��С����λ���֣�ע�⣬��λ�����ֽڣ�
*         pvParameters   ���ݸ��������Ĳ���
*         uxPriority     �������ȼ������ֵΪ(configMAX_PRIORITIES-1)
*         puxStackBuffer ����ջָ�룬�ڴ����û������ṩ
*         pxTaskBuffer   ������ƿ�ָ�룬�ڴ����û������ṩ
*
*  ��������ֵ��
*         NULL       �û�û���ṩ��Ӧ���ڴ棬���񴴽�ʧ��
*         ����ֵ     �����������񴴽��ɹ�
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
* ��������Ͷ�ʱ��������������
* ����: �����ջ ������ƿ�
*/
static StackType_t  IdleTaskStack[configMINIMAL_STACK_SIZE];        /* �������������ջ */
static StaticTask_t IdleTaskTCB;                                    /* ����������ƿ� */
static StackType_t  TimerTaskStack[configTIMER_TASK_STACK_DEPTH];   /* ��ʱ�����������ջ */
static StaticTask_t TimerTaskTCB;                                   /* ��ʱ������������ƿ� */


/**********************************************************************
*@funcName ��vApplicationGetIdleTaskMemory
*@brief    ����ȡ��������������ջ��������ƿ��ڴ�
*@param    ��ppxIdleTaskTCBBuffer     ������ƿ��ڴ�
*            ppxIdleTaskStackBuffer   �����ջ�ڴ�
*            pulIdleTaskStackSize     �����ջ��С
*@retval   ��void(��)
*@fn       ��
*               ��ȡ��������������ջ��������ƿ��ڴ棬��Ϊ������ʹ�õ�
*           ��̬�ڴ棬��˿�������������ջ��������ƿ���ڴ��Ӧ������
*           �����ṩ��FreeRTOS�ṩ�˽ӿں���vApplicationGetIdleTaskMemory()
*           ʵ�ִ˺������ɡ�
************************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
                                   StackType_t  **ppxIdleTaskStackBuffer, 
                                   uint32_t     *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/**********************************************************************
*@funcName ��vApplicationGetTimerTaskMemory
*@brief    ����ȡ��ʱ����������������ջ��������ƿ��ڴ�
*@param    ��
*            ppxTimerTaskTCBBuffer    ������ƿ��ڴ�
*            ppxTimerTaskStackBuffer  �����ջ�ڴ�
*            pulTimerTaskStackSize    �����ջ��С
*@retval   ��void(��)
*@fn       ��
*               ��ȡ��ʱ����������������ջ��������ƿ��ڴ棬��Ϊ������ʹ�õ�
*           ��̬�ڴ棬��˶�ʱ����������������ջ��������ƿ���ڴ��Ӧ������
*           �����ṩ��FreeRTOS�ṩ�˽ӿں���vApplicationGetTimerTaskMemory()
*           ʵ�ִ˺������ɡ�
************************************************************************/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t  **ppxTimerTaskStackBuffer,
                                    uint32_t     *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer  = &TimerTaskTCB;
    *ppxTimerTaskStackBuffer= TimerTaskStack;
    *pulTimerTaskStackSize  = configTIMER_TASK_STACK_DEPTH;
}

/**
* ���񴴽���������
* ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
*/
#define START_PRIO      1                     /* �������ȼ� */
#define START_STK_SIZE  64                    /* �����ջ��С */
StackType_t  StartTaskStack[START_STK_SIZE];  /* �����ջ */
StaticTask_t StartTaskTCB;                    /* ������ƿ� */
static TaskHandle_t StartTask_Handler=NULL;   /* ��������������� */
static void StartTaskCreate(void* parameter); /* ����������������� */

/**
* TASK1��������
* ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
*/
#define TASK1_PRIO      1                     /* �������ȼ� */
#define TASK1_STK_SIZE  64                    /* �����ջ��С */
StackType_t  Task1TaskStack[TASK1_STK_SIZE];  /* �����ջ */
StaticTask_t Task1TaskTCB;                    /* ������ƿ� */
static TaskHandle_t Task1_Handle=NULL;        /* ����1�������� */
static void Task1(void* parameter);           /* ����1���������� */
/**
* TASK2��������
* ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
*/
#define TASK2_PRIO      2                     /* �������ȼ� */
#define TASK2_STK_SIZE  64                    /* �����ջ��С */
StackType_t  Task2TaskStack[TASK2_STK_SIZE];  /* �����ջ */
StaticTask_t Task2TaskTCB;                    /* ������ƿ� */
static TaskHandle_t Task2_Handle=NULL;        /* ����2�������� */
static void Task2(void* parameter);           /* ����2���������� */
/**
* TASK3��������
* ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
*/
#define TASK3_PRIO      3                     /* �������ȼ� */
#define TASK3_STK_SIZE  64                    /* �����ջ��С */
StackType_t  Task3TaskStack[TASK3_STK_SIZE];  /* �����ջ */
StaticTask_t Task3TaskTCB;                    /* ������ƿ� */
static TaskHandle_t Task3_Handle=NULL;        /* ����3�������� */
static void Task3(void* parameter);           /* ����3���������� */


/**********************************************************
*@funcName ��App_demo
*@brief    ��ѧϰ������demo
*@param    ��void(��)
*@retval   ��void(��)
*@fn       ��
************************************************************/
void App_demo(void)
{
    /* ��̬����������������� */
    StartTask_Handler = xTaskCreateStatic(
                                          (TaskFunction_t   )StartTaskCreate,   /* ������ */
                                          (const char*      )"StartTaskCreate", /* �������� */
                                          (uint32_t         )START_STK_SIZE,    /* �����ջ��С */
                                          (void*            )NULL,              /* ���ݸ��������Ĳ��� */
                                          (UBaseType_t      )START_PRIO,        /* �������ȼ� */
                                          (StackType_t*     )StartTaskStack,    /* �����ջ */
                                          (StaticTask_t*    )&StartTaskTCB);    /* ������ƿ� */
                        
    if(StartTask_Handler != NULL) printf("StartTaskCreate���񴴽��ɹ�!\r\n");
    else                          printf("StartTaskCreate���񴴽�ʧ��!\r\n");
    
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
    taskENTER_CRITICAL(); /* �����ٽ�������������������Ǳ��뱣֤���ٽ��� */
    
    /* ��̬����Task1���� */          
    Task1_Handle = xTaskCreateStatic(
                                    (TaskFunction_t   )Task1,             /* ������ */
                                    (const char*      )"Task1",           /* �������� */
                                    (uint32_t         )TASK1_STK_SIZE,    /* �����ջ��С */
                                    (void*            )NULL,              /* ���ݸ��������Ĳ��� */
                                    (UBaseType_t      )TASK1_PRIO,        /* �������ȼ� */
                                    (StackType_t*     )Task1TaskStack,    /* �����ջ */
                                    (StaticTask_t*    )&Task1TaskTCB);    /* ������ƿ� */

	if(Task1_Handle != NULL) printf("Task1���񴴽��ɹ�!\r\n");
	else                     printf("Task1���񴴽�ʧ��!\r\n");
    
    /* ��̬����Task2���� */
    Task2_Handle = xTaskCreateStatic(
                                    (TaskFunction_t   )Task2,             /* ������ */
                                    (const char*      )"Task2",           /* �������� */
                                    (uint32_t         )TASK2_STK_SIZE,    /* �����ջ��С */
                                    (void*            )NULL,              /* ���ݸ��������Ĳ��� */
                                    (UBaseType_t      )TASK2_PRIO,        /* �������ȼ� */
                                    (StackType_t*     )Task2TaskStack,    /* �����ջ */
                                    (StaticTask_t*    )&Task2TaskTCB);    /* ������ƿ� */

	if(Task2_Handle != NULL) printf("Task2���񴴽��ɹ�!\r\n");
	else                     printf("Task2���񴴽�ʧ��!\r\n");
    
    /* ��̬����Task3���� */
    Task3_Handle = xTaskCreateStatic(
                                    (TaskFunction_t   )Task3,             /* ������ */
                                    (const char*      )"Task3",           /* �������� */
                                    (uint32_t         )TASK3_STK_SIZE,    /* �����ջ��С */
                                    (void*            )NULL,              /* ���ݸ��������Ĳ��� */
                                    (UBaseType_t      )TASK3_PRIO,        /* �������ȼ� */
                                    (StackType_t*     )Task3TaskStack,    /* �����ջ */
                                    (StaticTask_t*    )&Task3TaskTCB);    /* ������ƿ� */

	if(Task3_Handle != NULL) printf("Task3���񴴽��ɹ�!\r\n");
	else                     printf("Task3���񴴽�ʧ��!\r\n");
    
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

/*********************************************************************
*@funcName ��Task1
*@brief    ������1
*@param    ��void* parameter(δ�õ�)
*@retval   ��void(��)
*@fn       ���þ�̬��������ķ�ʽ����������1
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
*@fn       ���þ�̬��������ķ�ʽ����������2
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
*@fn       ���þ�̬��������ķ�ʽ����������3
**********************************************************************/
static void Task3(void* parameter)
{
    while (1)
    {
        printf("��̬��������3\r\n");
        vTaskDelay(500);   /* ��ʱ500��tick */
    }
}

