#include "some_filter.h"
#include "some_math.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"



static void delay(void)
{
	;
}

int get_ad(void)
{
return 0;
}
//1���޸��˲�
/*  Aֵ�ɸ���ʵ���������
    valueΪ��Чֵ��new_valueΪ��ǰ����ֵ  
    �˲����򷵻���Ч��ʵ��ֵ  */
#define A 10

char value;

char filter_limit(void)
{
   char  new_value;
   new_value = get_ad();
   if ( ( new_value - value > A ) || ( value - new_value > A ))
      return value;
   return new_value;
         
}

//2����λֵ�˲���
/*  Nֵ�ɸ���ʵ���������
    �������ð�ݷ�*/
#define N  12

char filter_mid(void)
{
   char value_buf[N];
   char count,i,j,temp;
   for ( count=0;count<N;count++)
   {
      value_buf[count] = get_ad();
      delay();
   }
   for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j-1;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
      }
   }
	 
	 if((N&1)>0)
    temp	= 	 value_buf[(N+1)/2];
	 else 
	  temp = (value_buf[N / 2] + value_buf[N / 2 + 1]) / 2;  
	 return temp;
		 
}     

//3������ƽ���˲���


char filter_average(void)
{
   int  sum = 0;
   for ( int count=0;count<N;count++)
   {
      sum += get_ad();
      delay();
   }
   return (char)(sum/N);
}

//4������ƽ���˲������ֳƻ���ƽ���˲�����

char value_buf[N];
char i=0;

char filter_slip(void)
{
   char count;
   int  sum=0;
   value_buf[i++] = get_ad();
   if ( i == N )   i = 0;
   for ( count=0;count<N;count++)
      sum += value_buf[count];
   return (char)(sum/N);
}


float filter_buf[255]={0};
float filter_slip_use(float get_ad,uint8_t n)
{
   static uint8_t i=0;
   uint8_t count;
    float  sum=0;
   filter_buf[i++] = get_ad;
   if ( i == n )   i = 0;
   for ( count=0;count<n;count++)
	 sum += filter_buf[count];
   
   return (float)(sum/n);
}



//5����λֵƽ���˲������ֳƷ��������ƽ���˲�����


char filter_mid_average(void)
{
  
   char value_buf[N];
   char count,i,j,temp;
	 int sum;
   for ( count=0;count<N;count++)
   {
      value_buf[count] = get_ad();
      delay();
   }
   for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j-1;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
      }
   }
   for(count=1;count<N-1;count++)
      sum +=value_buf [count];
   return (char)(sum/(N-2));
}

//6���޷�ƽ���˲���
/*
*/  
//�� �ο��ӳ���1��3

//7��һ���ͺ��˲���
/* Ϊ�ӿ�������ٶȼٶ�����Ϊ100��a=0~100 */

#define a 50

char value;

char filter_later(void)
{
   char  new_value;
   new_value = get_ad();
   return (100-a)*value + a*new_value; 
}

//8����Ȩ����ƽ���˲���
/* coe����Ϊ��Ȩϵ�������ڳ���洢����*/


char  coe[N] = {1,2,3,4,5,6,7,8,9,10,11,12};
#define sum_coe   (1+2+3+4+5+6+7+8+9+10+11+12)

char filter_push(void)
{
   char count;
   char value_buf[N];
   int  sum=0;
   for (count=0;count<N;count++)
   {
      value_buf[count] = get_ad();
      delay();
   }
   for (count=0;count<N;count++)
      sum += value_buf[count]*coe[count];
   return (char)(sum/sum_coe);
}

//9�������˲���


char filter_shake_eliminate(void)
{
   char count=0;
   char new_value;
   new_value = get_ad();
   while (value !=new_value);
   {
      count++;
      if (count>=N)   return new_value;
       delay();
      new_value = get_ad();
   }
   return value;    
}

//10���޷������˲���
/*
*/
//�� �ο��ӳ���1��9 

//11����ͨ�˲��� 
#define  a  0.7

int Low_pass(void)
{
  int input,input_last,input_take;
  input = get_ad();
//��ͨ�˲�
  input_take = a *input + (1.0f-a)*input_last;
	
	input_last = input;
	
  return input_take;
}



int Low_pass2(void)
{
  static int input,input_last,input_take;
  input = get_ad();
//��ͨ�˲�
	input_take *= a;
	
	input_take += input*(1.0f-a);

  return input_take;
}

#define T 1
int Low_pass3(void)
{
  static int input,input_take;
  input = get_ad();
//��ͨ�˲�
	input_take += 10*3.14f*T*(input-input_take);

  return input_take;
}

//11�������˲��� 
#define k 0.9
int ang_from_acc(void)
{ 
	int acc;
	//........
	//�޷� gyro +-10;
  return acc;	
}

int ang_from_gyro(void)
{ 
	int k1=1;
	int gyro;
	gyro+=gyro*k1;
  return gyro;	
}	
	
int complementation(void)
{	
int ang_out,ang_err,ang_acc,ang_gyro;
	int k2=2;
	ang_acc=ang_from_acc();
	ang_gyro=ang_from_gyro();
	
	ang_err = ang_acc-ang_gyro;
  ang_gyro += ang_err*k2;
	
return ang_out;
}

//13.�������˲���
double frand()
{
	return 2*((rand()/(double)RAND_MAX) - 0.5); //�������
}

int kalman_filte_test(float z_measure,float Q,float R)
{

float x_last=0;     //ϵͳ��ʼֵ
float p_last=0.02;  //����Ȩ��
//float Q=0.018;
//float R=0.542;
float kg;     //kg Ϊ kalman filter
float x_mid;  //����ֵ ��Ԥ�⣬�ٶ���֮ǰһ����
float x_now;  //���Ƴ�����ֵ��output��
float p_mid;       
float p_now;
//float z_real=0.56;//0.56
//float sumerror_kalman=0;
//float sumerror_measure=0;
int i;
//x_last=z_real+frand()*0.03;
//x_last=z_real;
x_mid=x_last;
	
for(i=0;i<20;i++)
{
		x_mid=x_last;                    //x_last=x(k-1|k-1),x_mid=x(k|k-1)
		p_mid=p_last+Q;                 //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
		kg=p_mid/(p_mid+R);          		//kg Ϊ kalman filter��R Ϊ����
//		z_measure=z_real+frand()*0.03;	//����ֵ
		x_now=x_mid+kg*(z_measure-x_mid);		//���Ƴ�������ֵ
		p_now=(1-kg)*p_mid;									//����ֵ��Ӧ�� covariance
	
//		printf("Real position: %6.3f \n",z_real); 		//��ʾ��ֵ
//		printf("Mesaured position: %6.3f [diff:%.3f]\n",z_measure,fabs(z_real-z_measure)); //��ʾ����ֵ�Լ���ֵ�����ֵ֮������																																						
//		printf("Kalman position: %6.3f [diff:%.3f]\n",x_now,fabs(z_real - x_now)); //��ʾ	kalman ����ֵ�Լ���ֵ�Ϳ���������ֵ�����
	
//		sumerror_kalman += fabs(z_real - x_now); 					//kalman ����ֵ���ۻ����
//		sumerror_measure += fabs(z_real-z_measure); //��ֵ�����ֵ���ۻ����
		p_last = p_now; 														//���� covariance ֵ
		x_last = x_now; 																//����ϵͳ״ֵ̬
}

//printf("���������� : %f\n",sumerror_measure); //��������ۻ����
//printf("���忨�����˲����: %f\n",sumerror_kalman); //��� kalman �ۻ����
//printf("�����������ռ����: %d%% \n",100-(int)((sumerror_kalman/sumerror_measure)*100));

return  x_now;
	
}

int kalman_filte(float z_measure,float Q,float R,float x_last,float p_last)
{

//float x_last=0;     //ϵͳ��ʼֵ
//float p_last=0.02;  //����Ȩ��
//float Q=0.018;
//float R=0.542
float kg;     //kg Ϊ kalman filter
float x_mid;  //����ֵ ��Ԥ�⣬�ٶ���֮ǰһ����
float x_now;  //���Ƴ�����ֵ��output��
float p_mid;       
float p_now;

int i;

x_mid=x_last;
	
for(i=0;i<20;i++)
{
		x_mid=x_last;                      		//x_last=x(k-1|k-1),x_mid=x(k|k-1)
		p_mid=p_last+Q;                    	    //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
		kg=p_mid/(p_mid+R);          			//kg Ϊ kalman filter��R Ϊ����

		x_now=x_mid+kg*(z_measure-x_mid);		//���Ƴ�������ֵ
		p_now=(1-kg)*p_mid;						//����ֵ��Ӧ�� covariance

		p_last = p_now; 						//���� covariance ֵ
		x_last = x_now; 						//����ϵͳ״ֵ̬
}


return  x_now;
	
}


float angle, angle_dot; 	
float Q_angle=0.001;// ����������Э����
float Q_gyro=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle=0.5;// ����������Э���� �Ȳ���ƫ��
float dt=0.005;//                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter(float Accel,float Gyro)		
{
	angle+=(Gyro - Q_bias) * dt; //�������
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	angle_dot   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
//	rt_kprintf("angle %d\r\n",(int)angle_dot);
}

#define lvbo_K1 0.2
float Yijielvbo(float angle_m, float gyro_m)
{
	float angle;
	angle = lvbo_K1 * angle_m+ (1-lvbo_K1) * (angle + gyro_m * 0.005);

//	rt_kprintf("angle %d\r\n",(int)angle);
	return angle;
}
