#include "ds18b20.h"
#include "delay.h"
#define MaxSensorNum 4
u8 DS18B20_ID[MaxSensorNum][8];	// ���⵽�Ĵ�����DS18B20_ID������,ǰ���ά���������ߴ�������������
u8 DS18B20_SensorNum;			// ��⵽�Ĵ���������(��1��ʼ��������ʾ1����1����8����8��)

void DS18B20_Rst(void)	   
{
	DS18B20_IO_OUT(); //SET PC6 OUTPUT
  DS18B20_DQ_OUT=0; //����DQ
  delay_us(750);    //����750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}


//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����

u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//SET PA0 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS18B20_IO_OUT();//SET PC6 OUTPUT
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PC6 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//SET PC6 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//��ʼ�¶�ת��
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();
		DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PORTB��ʱ�� 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//PB10 �������
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_10);    //���1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(u8 i)
{
    u8 temp;
	  u8 j;
    u8 TL,TH;
	  short tem;
    DS18B20_Rst();
    DS18B20_Check();
		DS18B20_Write_Byte(0xcc);// skip rom
		DS18B20_Write_Byte(0x44);// convert
		DS18B20_Rst();
    DS18B20_Check();

	// DS18B20_Write_Byte(0xcc);// skip rom
	//ƥ��ID��iΪ�β�
	DS18B20_Write_Byte(0x55);
	for (j = 0; j < 8; j++)
	{
		DS18B20_Write_Byte(DS18B20_ID[i][j]);
	}
		DS18B20_Write_Byte(0xbe);// convert
    TL=DS18B20_Read_Byte(); // LSB
    TH=DS18B20_Read_Byte(); // MSB  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL;
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(float)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
} 

// read two bit
u8 DS18B20_Read_2Bit(void)//����λ �ӳ���
{
	u8 i;
	u8 dat = 0;
	for (i = 2; i > 0; i--)
	{
		dat = dat << 1;
		DS18B20_IO_OUT();
		DS18B20_DQ_OUT=0;
		delay_us(2);
		DS18B20_DQ_OUT=1;
		DS18B20_IO_IN();
		delay_us(12);
		if (DS18B20_DQ_IN)	dat |= 0x01;
		delay_us(50);
	}
	return dat;
}


// д1λ��DS18B20
void DS18B20_Write_Bit(u8 dat)
{
		DS18B20_IO_OUT();
	if (dat)
	{
		DS18B20_DQ_OUT=0;
		delay_us(2);
		DS18B20_DQ_OUT=1;
		delay_us(60);
	}
	else
	{
		DS18B20_DQ_OUT=0;
		delay_us(60);
		DS18B20_DQ_OUT=1;
		delay_us(2);
	}
}


//serach rom
void DS18B20_SearchRom(void){
	u8 k, l, chongtuwei, m, n, num;
	u8 zhan[5]={0};
	u8 ss[64];
	u8 tempp;
	l = 0;
	num = 0;
	do
	{
		DS18B20_Rst(); //ע�⣺��λ����ʱ����
		delay_us(420); //480��720
		DS18B20_Write_Byte(0xf0);
		for (m = 0; m < 8; m++)
		{
			u8 s = 0;
			for (n = 0; n < 8; n++)
			{
				k = DS18B20_Read_2Bit();//����λ����
				k = k & 0x03;
				s >>= 1;
				if (k == 0x01)//01����������Ϊ0 д0 ��λΪ0��������Ӧ
				{
					DS18B20_Write_Bit(0);
					ss[(m * 8 + n)] = 0;
				}
				else if (k == 0x02)//����������Ϊ1 д1 ��λΪ1��������Ӧ
				{
					s = s | 0x80;
					DS18B20_Write_Bit(1);
					ss[(m * 8 + n)] = 1;
				}
				else if (k == 0x00)//����������Ϊ00 �г�ͻλ �жϳ�ͻλ
				{
					//�����ͻλ����ջ��д0 С��ջ��д��ǰ���� ����ջ��д1
					chongtuwei = m * 8 + n + 1;
					if (chongtuwei > zhan[l])
					{
						DS18B20_Write_Bit(0);
						ss[(m * 8 + n)] = 0;
						zhan[++l] = chongtuwei;
					}
					else if (chongtuwei < zhan[l])
					{
						s = s | ((ss[(m * 8 + n)] & 0x01) << 7);
						DS18B20_Write_Bit(ss[(m * 8 + n)]);
					}
					else if (chongtuwei == zhan[l])
					{
						s = s | 0x80;
						DS18B20_Write_Bit(1);
						ss[(m * 8 + n)] = 1;
						l = l - 1;
					}
				}
				else
				{
					//û��������
				}
			}
			tempp = s;
			DS18B20_ID[num][m] = tempp; // ������������ID
		}
		num = num + 1;// �����������ĸ���
	}while(zhan[l] != 0 && (num < MaxSensorNum));
	DS18B20_SensorNum = num; 
}


