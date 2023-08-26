#include <STC15.h>
#include <intrins.h>
#include <I2C.h>

sbit LED = P3^4;
sbit I2C_SCL = P3^6;
sbit I2C_SDA = P3^7;

void delay(unsigned int delay_time)		//1毫秒@22.1184MHz
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

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

/*----------------------------
初始化ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //设置P1口为AD口，0xff表示8通道全开
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    delay(20);                       //ADC上电并延时
}

/*----------------------------
读取ADC结果
----------------------------*/
unsigned char GetADCResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //返回ADC结果
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
主程序由此开始
----------------------------*/

#define ADDR 7		//定义从机地址/编号
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
		while(!I2C_Write(TX_data,ADDR,8));//ADDR：从机地址
		LED = 1;
		//delay(10);
	}
}


/*void Timer0Init(void)
{
	ET0 = 1;		//定时器0开中断
	PT0 = 1;		//定时器0中断最高优先级
}*/

/*void Timer2Init(void)
{
	IE2 |= 0x04;	//定时器2开中断
}*/

/*void PCAInit()	//定时100Hz
{
	CCON = 0; //初始化PCA控制寄存器
	//PCA定时器停止
	//清除CF标志
	//清除模块中断标志
	CL = 0; //复位PCA寄存器
	CH = 0;
	CMOD = 0x00; //设置PCA时钟源
	//禁止PCA定时器溢出中断
	CCAP0L = 0x00;
	CCAP0H = 0x6C; //初始化PCA模块0
	CCAPM0 = 0x49; //PCA模块0为16位定时器模式
	CR = 1; //PCA定时器开始工作
}*/

/*void Uart1Init(void)		//115200bps@22.1184MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	ES = 1; 		//使能串口1中断
}*/

/*void Uart2Init(void)		//115200bps@22.1184MHz
{
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	IE2 |= 0x01;		//使能串口2中断
}*/
