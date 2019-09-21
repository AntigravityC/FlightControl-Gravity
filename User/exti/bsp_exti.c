
#include "include.h" 


 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
  
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = CAPTURE_EXIT_IRQ;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ж�Դ������2������ʹ������������� */  
//  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
//  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */


//void CAPTURE_EXTI_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;
//  
//	/*��������GPIO�ڵ�ʱ��*/
////	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK ,ENABLE);
//  	RCC_AHB1PeriphClockCmd(CAPTURE_ICPWM_GPIO_CLK,ENABLE);
//  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//  
//  /* ���� NVIC */
//  NVIC_Configuration();
//  
//	/* ѡ�񰴼�1������ */ 
//  GPIO_InitStructure.GPIO_Pin =CAPTURE_ICPWM_PIN|CAPTURE_ICPWM_PIN2|CAPTURE_ICPWM_PIN3|CAPTURE_ICPWM_PIN4;
//	
//	
//	
//  /* ��������Ϊ����ģʽ */ 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
//  /* ������������ */
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  /* ʹ������Ľṹ���ʼ������ */
//  GPIO_Init(CAPTURE_ICPWM_GPIO_PORT, &GPIO_InitStructure); 

//	/* ���� EXTI �ж�Դ ��key1���� */
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource6|EXTI_PinSource7|EXTI_PinSource8|EXTI_PinSource9);




//  /* ѡ�� EXTI �ж�Դ */
//  EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9;
//  /* �ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* �½��ش��� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//  /* ʹ���ж�/�¼��� */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  
////  /* ѡ�񰴼�2������ */ 
////  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;  
////  /* ����������������ͬ */
////  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      

////	/* ���� EXTI �ж�Դ ��key2 ���� */
////  SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);

////  /* ѡ�� EXTI �ж�Դ */
////  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
////  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
////  /* �����ش��� */
////  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
////  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
////  EXTI_Init(&EXTI_InitStructure);
//}
/*********************************************END OF FILE**********************/
