#include <STC15.h>
#include <intrins.h>
#include <I2C.h>

sbit LED = P3^4;
sbit I2C_SCL = P3^6;
sbit I2C_SDA = P3^7;

void delay(unsigned int delay_time)		//1����@22.1184MHz
{
	unsigned char i, j;
	for(;delay_time>0;delay_time--)
	{
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
}

void InitADC();
unsigned char GetADCResult(unsigned char ch);

#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

/*----------------------------
��ʼ��ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //����P1��ΪAD�ڣ�0xff��ʾ8ͨ��ȫ��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    delay(20);                       //ADC�ϵ粢��ʱ
}

/*----------------------------
��ȡADC���
----------------------------*/
unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //����ADC���
}
/*
void test(void){
	while(1){
		I2C_SCL=1;
		delay(10);
		I2C_SDA=1;
		delay(10);
		I2C_SCL=0;
		delay(10);
		I2C_SDA=0;
		delay(10);
		LED = !LED;
	}
}*/

/*----------------------------
�������ɴ˿�ʼ
----------------------------*/

#define ADDR 7		//����ӻ���ַ/���
unsigned char TX_data[8],i;

main()
{
	delay(0);
	InitADC();
	EA = 1;
	LED = 1;
	delay(10);
//	test();
	while(1)
	{
		for(i=0;i<8;i++)TX_data[i]=GetADCResult(i);
		LED = 0;
		while(!I2C_Write(TX_data,ADDR,8));//ADDR���ӻ���ַ
		LED = 1;
		//delay(10);
	}
}


/*void Timer0Init(void)
{
	ET0 = 1;		//��ʱ��0���ж�
	PT0 = 1;		//��ʱ��0�ж�������ȼ�
}*/

/*void Timer2Init(void)
{
	IE2 |= 0x04;	//��ʱ��2���ж�
}*/

/*void PCAInit()	//��ʱ100Hz
{
	CCON = 0; //��ʼ��PCA���ƼĴ���
	//PCA��ʱ��ֹͣ
	//���CF��־
	//���ģ���жϱ�־
	CL = 0; //��λPCA�Ĵ���
	CH = 0;
	CMOD = 0x00; //����PCAʱ��Դ
	//��ֹPCA��ʱ������ж�
	CCAP0L = 0x00;
	CCAP0H = 0x6C; //��ʼ��PCAģ��0
	CCAPM0 = 0x49; //PCAģ��0Ϊ16λ��ʱ��ģʽ
	CR = 1; //PCA��ʱ����ʼ����
}*/

/*void Uart1Init(void)		//115200bps@22.1184MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	ES = 1; 		//ʹ�ܴ���1�ж�
}*/

/*void Uart2Init(void)		//115200bps@22.1184MHz
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	IE2 |= 0x01;		//ʹ�ܴ���2�ж�
}*/
