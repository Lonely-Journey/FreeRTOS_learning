#include "FreeRTOS_demo.h"
#include "key.h"

/*****************************************************************************************
*                              �б����
*
*      �б��� FreeRTOS ���������һ�����ݽṹ����������洢��Ԫ
*  ���Ƿ���������˳���(�б�ı��ʾ�������)���б��� FreeRTOS ��
*  ��Ӧ��ʮ�ֹ㷺��Ҫע����ǣ� FreeRTOS �е��б���һ��˫������
*  ��list.h �ļ��У����б����ض��塣
*
*  ����ԭ�ͣ�
*          typedef struct xLIST
*          {
*               listFIRST_LIST_INTEGRITY_CHECK_VALUE                У��ֵ 
*               volatile UBaseType_t              uxNumberOfItems;  �б����б�������� 
*               ListItem_t * configLIST_VOLATILE  pxIndex;          ���ڱ����б� 
*               MiniListItem_t                    xListEnd;         ���һ���б���(�����б���)
*               listSECOND_LIST_INTEGRITY_CHECK_VALUE               У��ֵ
*          } List_t;
*
*  ��Ա������
*           listFIRST_LIST_INTEGRITY_CHECK_VALUE �� listSECOND_LIST_INTEGRITY_CHECK_VALUE ��
*           �����������ڴ��ȷ����֪������ FreeRTOSͨ�����������������ֵ�����ж��б��������
*           �������й����У��Ƿ��⵽�ƻ������������ĺ궨�����б���������б�����Ҳ�г��֡��ù�
*           ��һ�����ڵ��ԣ� Ĭ���ǲ������ģ���˱��̳��ݲ�����������ܡ�
*
*           uxNumberOfItems�����ڼ�¼�б����б���ĸ����������� xListEnd���������б��в�����
*                            ����ʱ����ֵ�� 1�������б����Ƴ��б���ʱ����ֵ�� 1��
*
*           pxIndex        ������ָ���б��е�ĳ���б��һ�����ڱ����б��е������б��(Ĭ��
*                            ��ָ�������б���)
*
*           xListEnd       �������б���б��������б����ֵһ�㱻����Ϊ���ֵ�����ڽ��б���
*                            �������б����������ʱ��������ĩβ��ͬʱ xListEnd Ҳ���ڹ�����
*                            �����뵽�б��е��б��
*
**********************************************************************************************/

/****************************************************************************************************
*                             �б���
*
*      �б������б������ڴ�����ݵĵط����� list.h �ļ��У����б����
*  ��ض���
*
*  ����ԭ�ͣ�
*           struct xLIST_ITEM
*           {
*               listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE             ���ڼ���б�������������� 
*               configLIST_VOLATILE TickType_t           xItemValue;  �б����ֵ
*               struct xLIST_ITEM * configLIST_VOLATILE  pxNext;      ��һ���б��� 
*               struct xLIST_ITEM * configLIST_VOLATILE  pxPrevious;  ��һ���б���
*               void *                                   pvOwner;     �б����ӵ����
*               struct xLIST * configLIST_VOLATILE       pxContainer; �б��������б�
*               listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE            ���ڼ���б��������������
*           };
*           typedef struct xLIST_ITEM ListItem_t;
*
*  ��Ա������
*          listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE �� listSECOND_LIST_ITEM_INTEGRITY_CHECK_VALUE ��
*          ��ͬ�б�һ�����б�����Ҳ�������������ڼ���б������������Եĺ궨�塣
*          
*          xItemValue           ���б����ֵ�����ֵ�����ڰ�������б��е��б����������
*
*          pxNext �� pxPrevious ���ֱ�����ָ���б����б������һ���б������һ���б���ֱ�����ָ���б���
*                                 �б������һ���б������һ���б��
*
*          pvOwner              ������ָ������б���Ķ���ͨ����������ƿ飩����ˣ��б���Ͱ����б���
*                                 �Ķ���֮�����˫�����ӡ�
*
*          pxContainer          ������ָ���б��������б�
******************************************************************************************************/

/***************************************************************************************************
*                              �����б�
*      �����б���Ҳ���б���������б�������ڱ���б��ĩβ�͹������������б��е��б���û����ò�
*  �������б���ģ��� list.h �ļ��У��������б������ض��塣
*
*  ����ԭ�ͣ�
*           struct xMINI_LIST_ITEM
*           {
*               listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE             ���ڼ���б��������������  
*               configLIST_VOLATILE TickType_t           xItemValue;  �б����ֵ 
*               struct xLIST_ITEM * configLIST_VOLATILE  pxNext;      ��һ���б��� 
*               struct xLIST_ITEM * configLIST_VOLATILE  pxPrevious;  ��һ���б���  
*           };
*           typedef struct xMINI_LIST_ITEM MiniListItem_t; 
*
*  ��Ա������
*          listFIRST_LIST_ITEM_INTEGRITY_CHECK_VALUE �����ڼ���б������������Եĺ궨�塣
*
*          xItemValue           ���б����ֵ�����ֵ�����ڰ�������б��е��б����������(����һ����Ϊ���ֵ)
*
*          pxNext �� pxPrevious ���ֱ�����ָ���б����б������һ���б������һ���б��
*          
****************************************************************************************************/


/*************************************************************************
*                       �б������ҪAPI����
*
*           vListInitialise()        ��ʼ���б�
*           vListInitialiseItem()    ��ʼ���б���
*           vListInsertEnd()         �б�ĩβ�����б���
*           vListInsert()            �б�����б���
*           uxListRemove()           �б��Ƴ��б���
***************************************************************************/

/***********************************************************************************
*                    vListInitialise()����
*
*      �˺������ڳ�ʼ���б��ڶ����б�֮����Ҫ�ȶ�����г�ʼ���� ֻ�г�ʼ������б�
*  ���ܹ������ر�ʹ�á��б��ʼ���Ĺ��̣���ʵ���ǳ�ʼ���б��еĳ�Ա������ 
*
*  ����ԭ��  ��void vListInitialise(List_t * const pxList);
*  ��������  ��pxList    �б�
*  ��������ֵ�� ��
************************************************************************************/

/***********************************************************************************
*                     vListInitialiseItem()����
*
*      �˺������ڳ�ʼ���б����ͬ�б�һ�����ڶ����б���֮��Ҳ��Ҫ�ȶ����
*  �г�ʼ����ֻ�г�ʼ���е��б�����ܹ���������ʹ�á��б����ʼ���Ĺ��̣�Ҳ
*  �ǳ�ʼ���б����еĳ�Ա������ 
*
*  ����ԭ��  ��void vListInitialiseItem(ListItem_t * const pxItem);
*  ��������  ��pxItem    �б���
*  ��������ֵ�� ��
************************************************************************************/

/***********************************************************************************
*                      vListInsertEnd()����
*
*      �˺������ڽ��������б���б�����뵽�б� pxIndex ָ��ָ���б���
*  ��ǰ�棬��һ������Ĳ��뷽���� 
*
*  ����ԭ��  ��void vListInsertEnd(
*                                  List_t * const     pxList,
*                                  ListItem_t * const pxNewListItem);
*  ��������  ��
*             pxList          �б�
*             pxNewListItem   �������б���
*  ��������ֵ�� ��
************************************************************************************/

/***********************************************************************************
*                      vListInsert()����
*
*      �˺������ڽ��������б���б�����б���ֵ���������˳������
*  �ز��뵽�б��С�
*
*  ����ԭ��  ��void vListInsert(
*                              List_t * const     pxList,
                               ListItem_t * const pxNewListItem);
*  ��������  ��
*             pxList          �б�
*             pxNewListItem   �������б���
*  ��������ֵ�� ��
************************************************************************************/

/***********************************************************************************
*                      uxListRemove()����
*
*      �˺������ڽ��б�����б��������б����Ƴ���
*
*  ����ԭ��  ��UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove);
*  ��������  ��pxItemToRemove  ���Ƴ����б���
*  ��������ֵ������   ���Ƴ��б����Ƴ��������б�ʣ���б��������
************************************************************************************/

/**********************************************************************************
*                             �б�����������
*
*  �б��൱�������б����൱�ڽڵ㣬FreeRTOS �е��б���һ��˫�������� 
*
*  �б���ص㣺
*             �б����ĵ�ַ�������ģ�����Ϊ�����ӵ�һ��ġ��б������Ŀ
*             ���ɺ�����ӵĸ��������ģ���ʱ���Ըı䡣
*  ������ص㣺
*             �����Ա��ַ�������ģ����������ȷ���˳�Ա���������޷��ı䡣
*
*  ��OS������������ǲ�ȷ���ģ���������״̬�ǻᷢ���ı�ģ����Էǳ�������
*  ��(����)�������ݽṹ��
**********************************************************************************/

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
* �����б�Ͳ����б����
*/
List_t          TestList;           /* ��������б� */
ListItem_t      ListItem1;          /* ��������б���1 */
ListItem_t      ListItem2;          /* ��������б���2 */
ListItem_t      ListItem3;          /* ��������б���3 */


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
    /* ��һ����ʼ���б���б��� */
    vListInitialise(&TestList);         /* ��ʼ���б� */
    vListInitialiseItem(&ListItem1);    /* ��ʼ���б���1 */
    vListInitialiseItem(&ListItem2);    /* ��ʼ���б���2 */
    vListInitialiseItem(&ListItem3);    /* ��ʼ���б���3 */
    ListItem1.xItemValue = 40;
    ListItem2.xItemValue = 60;
    ListItem3.xItemValue = 50;
    
    printf("/**************�ڶ�������ӡ�б���б���ĵ�ַ**************/\r\n");
    printf("��Ŀ\t\t\t��ַ\r\n");
    printf("TestList\t\t0x%p\t\r\n", &TestList);
    printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);
    printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));
    printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    printf("/**************************����***************************/\r\n");
    
    printf("\r\n/*****************���������б���1�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem1);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("/**************************����***************************/\r\n");
    
    printf("\r\n/*****************���Ĳ����б���2�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem2);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("/**************************����***************************/\r\n");
    
    printf("\r\n/*****************���岽���б���3�����б�******************/\r\n");
    vListInsert((List_t*    )&TestList,         /* �б� */
                (ListItem_t*)&ListItem3);       /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************����***************************/\r\n");
    
    printf("\r\n/*******************���������Ƴ��б���2********************/\r\n");
    uxListRemove((ListItem_t*   )&ListItem2);   /* �Ƴ��б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/**************************����***************************/\r\n");
    
    printf("\r\n/****************���߲����б�ĩβ����б���2****************/\r\n");
    TestList.pxIndex = &ListItem1;
    vListInsertEnd((List_t*     )&TestList,     /* �б� */
                   (ListItem_t* )&ListItem2);   /* �б��� */
    printf("��Ŀ\t\t\t\t��ַ\r\n");
    printf("TestList->pxIndex\t\t0x%p\r\n", TestList.pxIndex);
    printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
    printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
    printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
    printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
    printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
    printf("/************************ʵ�����***************************/\r\n");

    while (1)
    {
        vTaskDelay(900);   /* ��ʱ500��tick */
    }
}


