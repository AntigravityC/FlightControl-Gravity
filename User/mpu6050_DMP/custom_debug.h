/**
******************************************************************************
* @brief   ���Ը������
* @details None
* @file    custom_debug.h
* @author  GYC
* @version V1.0

* @date    2017-11-23
* @note    ע��
******************************************************************************
* @attention None
* @warning None
* @enum  	������ĳ��ö�٣�Doxygen���ڸ�ö�ٴ�����һ������. eg��@enum CTest::MyEnum
* @var      ������ĳ��������Doxygen���ڸ�ö�ٴ�����һ������. eg��@var CTest::m_FileKey
******************************************************************************

======================================================================================
Revision History:
					  Nodification
	          Author			     Data		                       Major Changes
          --------------      -------------------         --------------------------------------
* @since	   GYC					2017.11.23								creat
          ======================================================================================
*              GYC                  2017.12.18                          ���Ӷ����ƴ�ӡ
*              GYC                  2017.12.20                          �޸�DEBUG_MSG(����__func__)

*/


/**
 * @brief     ���
 * @see       һ�ΰ��������������õ�ע�ͣ��м��������������������ƣ��Զ�����������������ӡ�
 * @param     �������� eg: cnt: ����
 * @return    ��������ֵ��� eg: @return ����������ִ�н�������ɹ��򷵻�TRUE�����򷵻�FLASE
 * @retval    ��������ֵ���� eg: @retval NULL ���ַ��� @retval !NULL �ǿ��ַ�����
 * @exception ���ܲ������쳣���� eg: @exception ������ִ�п��ܻ����������Χ���쳣
 * @enum  	  ������ĳ��ö�٣�Doxygen���ڸ�ö�ٴ�����һ������. eg��@enum CTest::MyEnum
 * @var       ������ĳ��������Doxygen���ڸ�ö�ٴ�����һ������. eg��@var CTest::m_FileKey
 * @attention None
 * @warning None
 * @since ͨ������˵����ʲô�汾��ʱ��д�˲��ִ���
 * @todo �Խ�Ҫ�����������ע��
 * @pre  ����˵���������ǰ��������
 * @post ����˵��������֮���ʹ��������
 * @deprecated ����������ܻ��ڽ����İ汾��ȡ��
 * @par ʾ��: 
   @code
    // @code  ��ע���п�ʼ˵��һ�δ��룬ֱ��@endcode���
    extern  IThread *pThread;
    HANDLE hEvent = pThread->GetEventHandle();
    while(WaitForSingleObject(hEvent,0) != WAIT_OBJECT_0)
    {
    //Do something
    }
    @endcode
 */
 
/**    ��ɫ��ö�ٶ���  
  *   
  *    ��ö�ٶ�����ϵͳ����Ҫ�õ�����ɫ\n  
  *    ����ʹ�ø�ö����Ϊϵͳ����ɫ�ı�ʶ  
  */ 

  
  
#ifndef __CUSTOM_DEBUG_H
#define __CUSTOM_DEBUG_H

#include <string.h>
//example: printf("filename:%s\n", filename(__FILE__));		//Ϊ��ȥ��·��
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x

#ifdef CUSTOM_DEBUG_ON
#define DEBUG_MSG(fmt,arg...)	printf("<<DMSG>@File:%s@Line:%d@FUNC:%s>"fmt"\n",filename(__FILE__),__LINE__,__func__,##arg)
#else
#define DEBUG_MSG(fmt,arg...)    
#endif


//��ӡ����(����������) 
#define PRINT_ARRAY(array, length) do {         \
    for (int i = 0; i<length; i++) {            \
        printf(#array);	                        \
        printf("[%d]:%d\n", i, array[i]);       \
    }                                           \
}while(0);

//��һ�������Զ����Ʒ�ʽ��ӡ����
#define PRINT_BINARY(num) do {                  \
    printf(#num);                               \
    printf("_L:");                              \
    for (uint32_t i = num; i>0; i /= 2) {       \
        printf("%d", i%2);                      \
    }                                           \
    printf("\r\n");                             \
}while(0);

#endif

