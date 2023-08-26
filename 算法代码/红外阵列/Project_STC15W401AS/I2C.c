#include "STC15.h"
#include <intrins.h>

#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^6;
sbit I2C_SDA = P3^7;

/* I2C总线写操作，dat-待写入字节，返回值-主机应答位的值 */
bit I2CWrite(unsigned char dat)
{
	bit ack;  //用于暂存应答位的值
    unsigned char mask;  //用于探测字节内某一位值的掩码变量

    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        if ((mask&dat) == 0)  //该位的值输出到SDA上
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        while(I2C_SCL == 0);      //等待SCL上升沿
        while(I2C_SCL == 1);      //等待SCL下降沿
    }
    I2C_SDA = 1;   //8位数据发送完后，从机释放SDA，以检测应答
    while(I2C_SCL == 0);      //等待SCL上升沿
    ack = I2C_SDA; //读取此时的SDA值，即为从机的应答值
    while(I2C_SCL == 1);      //等待SCL下降沿

    return (~ack); //应答值取反以符合通常的逻辑：
                   //0=不存在或忙或写入失败，1=存在且空闲或写入成功   
}
/* I2C总线读操作，并发送非应答信号，返回值-读到的字节 */
unsigned char I2CReadNAK(unsigned char addr)
{
    unsigned char mask;
    unsigned char dat=0;

    //I2C_SDA = 1;  //首先确保释放SDA
	//I2C_SCL = 1;  //首先确保释放SDA
    for (mask=0x80; mask!=0; mask>>=1) //从高位到低位依次进行
    {
        while(I2C_SCL == 0);      //等待SCL上升沿
        if(I2C_SDA == 0)  //读取SDA的值
            dat &= ~mask; //为0时，dat中对应位清零
        else
            dat |= mask;  //为1时，dat中对应位置1
        while(I2C_SCL == 1);      //等待SCL下降沿
    }
	if((dat>>1)==addr)I2C_SDA = 0;	//地址正确，发送应答信号
    else I2C_SDA = 1;   			//地址错误，发送非应答信号
    while(I2C_SCL == 0);      //等待SCL上升沿
    while(I2C_SCL == 1);      //等待SCL下降沿
	I2C_SDA = 1;

    return dat;
}

/* 读取函数，buf-数据接收指针，addr-起始地址，len-读取长度 */
/*void I2C_Read(unsigned char *buf, unsigned char addr, unsigned char len)
{
    I2CStart();                //发送重复启动信号
    I2CWrite((addr<<1)|0x01);  //寻址器件，后续为读操作
    while (len > 1)            //连续读取len-1个字节
    {
        *buf++ = I2CReadACK(); //最后字节之前为读取操作+应答
        len--;
    }
    *buf = I2CReadNAK();       //最后一个字节为读取操作+非应答
    I2CStop();
}*/
/* 写入函数，buf-源数据指针，addr-起始地址，len-写入长度 */
bit I2C_Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
    unsigned char I2C_SDA_new,I2C_SDA_old;
	//I2C_SDA = 1;  //首先确保释放SDA
	//I2C_SCL = 1;  //首先确保释放SCL
	I2C_SDA_new=I2C_SDA;
	do{
		do I2C_SDA_old=I2C_SDA_new;
		while(((I2C_SDA_new=I2C_SDA)==1)||(I2C_SDA_old==0)); //等待SDA下降沿				
	}while(I2C_SCL==0);				  //并且SCL为高电平
	while(I2C_SCL == 1);      //等待SCL下降沿
	
	if((I2CReadNAK(addr)>>1)==addr)
	{
		while (len > 0)
        {
            I2CWrite(*buf++);
			//if(I2CWrite(*buf++)==0)break;     //写入一个字节数据
            len--;                //待写入长度计数递减
        }
		return 1;//发送成功	
	}
	return 0;//发送失败
}
