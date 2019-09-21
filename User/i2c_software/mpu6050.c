

#include "mpu6050.h"
#include "i2c_software.h"



/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}


/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(uint8_t reg_add,unsigned char*Read,uint8_t num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
	
	
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
//  for(i=0;i<1000;i++)
//  {
//    for(j=0;j<1000;j++)
//    {
//      ;
//    }
//  }
	  for(i=0;i<500;i++)
  {
    for(j=0;j<500;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ����ʣ�1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //��ͨ�˲��������ã���ֹƵ����1K��������5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //���ü��ٶȴ�����������2Gģʽ�����Լ�
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		rt_kprintf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
//		rt_kprintf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}
/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}
	short Accel[3];
	short Gyro[3];
    float Angle[3];

void MPU6050_read(void)
{

	float Temp;
	if (MPU6050ReadID() == 1)
	{	
				
		MPU6050ReadAcc(Accel);			
		
		
		MPU6050ReadGyro(Gyro);
		
				
//		MPU6050_ReturnTemp(&Temp); 
		
//		rt_kprintf("�����ǣ� %8d%8d%8d\r\n    ",Gyro[0],Gyro[1],Gyro[2]);
//		rt_kprintf("���ٶȣ� %8d%8d%8d\r\n    ",Accel[0],Accel[1],Accel[2]);
//		rt_kprintf("�¶ȣ� %8.2f\r\n",Temp);
			
			

			//*************************************	��������������ĸ�ʽ************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}   
	
	else
	{
		rt_kprintf("\r\nû�м�⵽MPU6050��������\r\n");

		while(1);	
	}
	
}

void MPU6050_gyro_prepare(short *gyroData,float *gyro)
{
	gyro[0]=(float)gyroData[0];
	gyro[1]=(float)gyroData[1];
	gyro[2]=(float)gyroData[2];
	
	gyro[0]+=GYRO_ROLL_CORRECT;
	gyro[1]+=GYRO_PITCH_CORRECT;
	gyro[2]+=GYRO_YAW_CORRECT;

	gyro[0]*=GYRO_AFS_SEL3_K;
	gyro[1]*=GYRO_AFS_SEL3_K;	
	gyro[2]*=GYRO_AFS_SEL3_K;
//	rt_kprintf("gyrox:%d    gyroy:%d   gyroz:%d\r\n ",(int)gyro[0],(int)gyro[1],(int)gyro[2]);
}

void MPU6050_acc_prepare(short *accData,float *acc)
{
	
	acc[0]=(float)accData[0];
    acc[1]=(float)accData[1];
    acc[2]=(float)accData[2];
		
	
    acc[0]=acc[0]/ACC_AFS_SEL3_K;
	acc[1]=acc[1]/ACC_AFS_SEL3_K;
	acc[2]=acc[2]/ACC_AFS_SEL3_K;
//	printf("accx:%f\r\n",acc[0]);
}

void MPU6050_task(float *gyro,float *acc)
{
	MPU6050_read();
	MPU6050_gyro_prepare(Gyro,gyro);
	MPU6050_acc_prepare(Accel,acc);

	
//	printf("Target:%f   measure:%f      PwmOut:%f\r\n",measure.Acc[ROLL],measure.Acc[PITCH],measure.Acc[YAW]);
//	printf("Target:%f   measure:%f      PwmOut:%f\r\n",measure.Angle[ROLL],measure.Angle[PITCH],measure.Angle[YAW]);
}


