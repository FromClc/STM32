#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"  

//ROM2: 28d84516a8003c7b
//ROM1: 28274016a8013cff
//IO方向设置
//数据端口PB10
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<8;} 
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<8;}
//测试git提交
//这个第二次提交
//////数据端口PB10 输入/输出
#define	DS18B20_DQ_OUT PBout(10) //数据端口	
#define	DS18B20_DQ_IN  PBin(10)  //数据端口	
   	
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(u8 i);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20   
void DS18B20_SearchRom(void); //自动搜寻rom
u8 DS18B20_Read_2Bit(void);
void DS18B20_Write_Bit(u8 dat);
#endif
