#include "STC15.h"
#include <intrins.h>

#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^6;
sbit I2C_SDA = P3^7;

/* I2C����д������dat-��д���ֽڣ�����ֵ-����Ӧ��λ��ֵ */
bit I2CWrite(unsigned char dat)
{
	bit ack;  //�����ݴ�Ӧ��λ��ֵ
    unsigned char mask;  //����̽���ֽ���ĳһλֵ���������

    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        if ((mask&dat) == 0)  //��λ��ֵ�����SDA��
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        while(I2C_SCL == 0);      //�ȴ�SCL������
        while(I2C_SCL == 1);      //�ȴ�SCL�½���
    }
    I2C_SDA = 1;   //8λ���ݷ�����󣬴ӻ��ͷ�SDA���Լ��Ӧ��
    while(I2C_SCL == 0);      //�ȴ�SCL������
    ack = I2C_SDA; //��ȡ��ʱ��SDAֵ����Ϊ�ӻ���Ӧ��ֵ
    while(I2C_SCL == 1);      //�ȴ�SCL�½���

    return (~ack); //Ӧ��ֵȡ���Է���ͨ�����߼���
                   //0=�����ڻ�æ��д��ʧ�ܣ�1=�����ҿ��л�д��ɹ�   
}
/* I2C���߶������������ͷ�Ӧ���źţ�����ֵ-�������ֽ� */
unsigned char I2CReadNAK(unsigned char addr)
{
    unsigned char mask;
    unsigned char dat=0;

    //I2C_SDA = 1;  //����ȷ���ͷ�SDA
	//I2C_SCL = 1;  //����ȷ���ͷ�SDA
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        while(I2C_SCL == 0);      //�ȴ�SCL������
        if(I2C_SDA == 0)  //��ȡSDA��ֵ
            dat &= ~mask; //Ϊ0ʱ��dat�ж�Ӧλ����
        else
            dat |= mask;  //Ϊ1ʱ��dat�ж�Ӧλ��1
        while(I2C_SCL == 1);      //�ȴ�SCL�½���
    }
	if((dat>>1)==addr)I2C_SDA = 0;	//��ַ��ȷ������Ӧ���ź�
    else I2C_SDA = 1;   			//��ַ���󣬷��ͷ�Ӧ���ź�
    while(I2C_SCL == 0);      //�ȴ�SCL������
    while(I2C_SCL == 1);      //�ȴ�SCL�½���
	I2C_SDA = 1;

    return dat;
}

/* ��ȡ������buf-���ݽ���ָ�룬addr-��ʼ��ַ��len-��ȡ���� */
/*void I2C_Read(unsigned char *buf, unsigned char addr, unsigned char len)
{
    I2CStart();                //�����ظ������ź�
    I2CWrite((addr<<1)|0x01);  //Ѱַ����������Ϊ������
    while (len > 1)            //������ȡlen-1���ֽ�
    {
        *buf++ = I2CReadACK(); //����ֽ�֮ǰΪ��ȡ����+Ӧ��
        len--;
    }
    *buf = I2CReadNAK();       //���һ���ֽ�Ϊ��ȡ����+��Ӧ��
    I2CStop();
}*/
/* д�뺯����buf-Դ����ָ�룬addr-��ʼ��ַ��len-д�볤�� */
bit I2C_Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
    unsigned char I2C_SDA_new,I2C_SDA_old;
	//I2C_SDA = 1;  //����ȷ���ͷ�SDA
	//I2C_SCL = 1;  //����ȷ���ͷ�SCL
	I2C_SDA_new=I2C_SDA;
	do{
		do I2C_SDA_old=I2C_SDA_new;
		while(((I2C_SDA_new=I2C_SDA)==1)||(I2C_SDA_old==0)); //�ȴ�SDA�½���				
	}while(I2C_SCL==0);				  //����SCLΪ�ߵ�ƽ
	while(I2C_SCL == 1);      //�ȴ�SCL�½���
	
	if((I2CReadNAK(addr)>>1)==addr)
	{
		while (len > 0)
        {
            I2CWrite(*buf++);
			//if(I2CWrite(*buf++)==0)break;     //д��һ���ֽ�����
            len--;                //��д�볤�ȼ����ݼ�
        }
		return 1;//���ͳɹ�	
	}
	return 0;//����ʧ��
}
