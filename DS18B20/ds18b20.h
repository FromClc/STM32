#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"  

//ROM2: 28d84516a8003c7b
//ROM1: 28274016a8013cff
//IO��������
//���ݶ˿�PB10
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<8;} 
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<8;}
//����git�ύ
//����ڶ����ύ
//////���ݶ˿�PB10 ����/���
#define	DS18B20_DQ_OUT PBout(10) //���ݶ˿�	
#define	DS18B20_DQ_IN  PBin(10)  //���ݶ˿�	
   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(u8 i);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20   
void DS18B20_SearchRom(void); //�Զ���Ѱrom
u8 DS18B20_Read_2Bit(void);
void DS18B20_Write_Bit(u8 dat);
#endif
