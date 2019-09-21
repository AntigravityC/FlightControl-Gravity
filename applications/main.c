/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       chong
 * 2018-11-06     SummerGift  
 * 2018-11-19     flybreak     
 */

#include "RTT_task.h"
#include "include.h"
#include <rtconfig.h>



void RTT_startup_thread(rt_thread_t thread, char *task);
void RTT_startup_sem(rt_sem_t sem, char *task);
//UART_HandleTypeDef huart3;

int main(void)
{
	
    init_all();                     

	
	start_thread	=  rt_thread_create("start",
                                     start_thread_entry,
									 RT_NULL,
									 START_STK_SIZE,
									 START_THREAD_PRIO,
									 START_THREAD_TIME);	
	RTT_startup_thread(start_thread,"start_thread_entry");	
	
	
	
    return RT_EOK;
}

void start_thread_entry(void* parameter)
{

	
	
	test_sem =        rt_sem_create("test_sem",         	 			/*  �ź������� */
							 1,     									/*  �ź�����ʼֵ��Ĭ����һ���ź��� */
							 RT_IPC_FLAG_FIFO);							/*  �ź���ģʽ FIFO(0x00)*/

	RTT_startup_sem( test_sem, "test_sem");

	receive_thread =   rt_thread_create( "receive", 	                /* �߳����� */
										 receive_thread_entry, 			/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 RECEIVE_STK_SIZE, 				/* �߳�ջ��С */
										 RECEIVE_THREAD_PRIO, 			/* �̵߳����ȼ� */
										 RECEIVE_THREAD_TIME);		    /* �߳�ʱ��Ƭ */
	RTT_startup_thread(receive_thread,"receive_thread_entry");

	sensors_thread =   rt_thread_create( "sensors", 	                /* �߳����� */
										 sensors_thread_entry, 			/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 SENSORS_STK_SIZE, 				/* �߳�ջ��С */
										 SENSORS_THREAD_PRIO, 			/* �̵߳����ȼ� */
										 SENSORS_THREAD_TIME);		    /* �߳�ʱ��Ƭ */

	RTT_startup_thread(sensors_thread,"sensors_thread_entry");	 
									 
	control_thread =   rt_thread_create( "control", 					/* �߳����� */
										 control_thread_entry, 			/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 CONTROL_STK_SIZE, 				/* �߳�ջ��С */
										 CONTROL_THREAD_PRIO, 			/* �̵߳����ȼ� */
										 CONTROL_THREAD_TIME); 			/* �߳�ʱ��Ƭ */
	RTT_startup_thread(control_thread,"control_thread_entry");

	angle_control_thread =   rt_thread_create( "control_angle", 		/* �߳����� */
										 angle_control_thread_entry, 	/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 ANGLE_CONTROL_STK_SIZE, 		/* �߳�ջ��С */
										 ANGLE_CONTROL_THREAD_PRIO, 	/* �̵߳����ȼ� */
										 ANGLE_CONTROL_THREAD_TIME);    /* �߳�ʱ��Ƭ */
   
	RTT_startup_thread(angle_control_thread,"angle_control_thread_entry");
	 
	send_thread   =    rt_thread_create( "send", 				    	/* �߳����� */
										 send_thread_entry, 			/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 SEND_STK_SIZE, 				/* �߳�ջ��С */
										 SEND_THREAD_PRIO, 				/* �̵߳����ȼ� */
										 SEND_THREAD_TIME); 			/* �߳�ʱ��Ƭ */
	RTT_startup_thread(send_thread,"send_thread_entry");

	flight_mode_thread = rt_thread_create( "flight_mode",				/* �߳����� */
										 mode_thread_entry, 			/* �߳���ں��� */
										 RT_NULL, 						/* �߳���ں������� */
										 MODE_STK_SIZE, 				/* �߳�ջ��С */
										 MODE_THREAD_PRIO, 				/* �̵߳����ȼ� */
										 MODE_THREAD_TIME);				/* �߳�ʱ��Ƭ */	
	RTT_startup_thread(flight_mode_thread,"flight_mode_thread_entry");			

	
	
	led_thread   =    rt_thread_create("led",                           /* �߳���ں��� */
								         led_thread_entry,        		/* �߳���ں������� */
										 RT_NULL,                 		/* �߳�ջ��С */
										 LED_STK_SIZE,            		/* �̵߳����ȼ� */
										 LED_THREAD_PRIO,        		/* �߳�ʱ��Ƭ */
										 LED_THREAD_TIME);	 		

	RTT_startup_thread(led_thread,"led_thread_entry"); 
	
}

void RTT_startup_thread(rt_thread_t thread, char *task)
{ 
	
	if (thread != RT_NULL)
	{
		rt_thread_startup(thread);
	    rt_kprintf("%s created success \r\n",task);	   
	}
	else	 
	{
		rt_kprintf("%s created failed \r\n",task);	   
		while(1);
	}
}

void RTT_startup_sem(rt_sem_t sem, char *task)
{ 
	
	if (sem != RT_NULL)
	{		
	    rt_kprintf("%s created success \r\n",task);	   
	}
	else	 
	{
		rt_kprintf("%s created failed \r\n",task);	   
		while(1);
	}
}


