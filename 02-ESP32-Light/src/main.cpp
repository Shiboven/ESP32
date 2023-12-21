/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-12-13 20:23:18
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-12-15 00:46:10
 * @FilePath: \C8T6d:\system\Desktop\ESP32\02-ESP32-Light\src\main.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include <Arduino.h>

#define CS1   15
#define WR1   14
#define DT1   13

#define CS2   5
#define WR2   18
#define DT2   23

#define L1621_1_WR digitalWrite(WR1, LOW);
#define L1621_1_CS digitalWrite(CS1, LOW);
#define L1621_1_DAT digitalWrite(DT1, LOW);
#define L1621_2_WR digitalWrite(WR2, LOW);
#define L1621_2_CS digitalWrite(CS2, LOW);
#define L1621_2_DAT digitalWrite(DT2, LOW);

#define H1621_1_WR digitalWrite(WR1, HIGH);
#define H1621_1_CS digitalWrite(CS1, HIGH);
#define H1621_1_DAT digitalWrite(DT1, HIGH);
#define H1621_2_WR digitalWrite(WR2, HIGH);
#define H1621_2_CS digitalWrite(CS2, HIGH);
#define H1621_2_DAT digitalWrite(DT2, HIGH);

//        命令数据        命令代码     （命令代码 = 命令标识100+命令数据）
#define BIAS    0x52  //0b1000 0101 0010 1/3duty 4com
#define SYSDIS  0X00  //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN   0X02  //0b1000 0000 0010 打开系统振荡器
#define LCDOFF  0X04  //0b1000 0000 0100 关LCD偏压
#define LCDON   0X06  //0b1000 0000 0110 打开LCD偏压
#define XTAL    0x28  //0b1000 0010 1000 外部接时钟
#define RC256   0X30  //0b1000 0011 0000 内部时钟
#define TONEON  0X12  //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10  //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A  //0b1000 0000 1010 禁止看门狗


static const int spiClk = 1000000; // 1 MHz


//写数据函数，DATA为数据，Index为数据位数
void HT1621_1_WRData(uint16_t DATA,uint16_t Index)
{
    uint16_t i;
    for(i=0; i<Index; i++)
    {
        L1621_1_WR;
        if(DATA&0x80)
        {
            H1621_1_DAT;
        }
        else
        {
            L1621_1_DAT;
        }
        H1621_1_WR;
        DATA<<=1;
    }
}
//命令写入函数，命令代码 = 命令标志（100/0x80）+命令数据。Cmd命令数据
void HT1621_1_WRCmd(uint16_t Cmd)
{
    L1621_1_CS;
    HT1621_1_WRData(0x80,4);//命令模式代码100
    HT1621_1_WRData(Cmd,8); //命令数据
    H1621_1_CS;
}
//连续数据写入函数，
void HT1621_1_WRAllData(uint16_t Addr,uint16_t *Data,uint16_t index)
{
    uint16_t i;
    L1621_1_CS;
    HT1621_1_WRData(0xa0,3); //写入数据标志101
    HT1621_1_WRData(Addr<<2,6); //写入地址数据
    for(i=0; i<index; i++)
    {
        HT1621_1_WRData(*Data,8); //写入数据
        Data++;
    }
    H1621_1_CS;
}

/***************************************************/
//写数据函数，DATA为数据，Index为数据位数
void HT1621_2_WRData(uint16_t DATA,uint16_t Index)
{
    uint16_t i;
    for(i=0; i<Index; i++)
    {
        L1621_2_WR;
        if(DATA&0x80)
        {
            H1621_2_DAT;
        }
        else
        {
            L1621_2_DAT;
        }
        H1621_2_WR;
        DATA<<=1;
    }
}
//命令写入函数，命令代码 = 命令标志（100/0x80）+命令数据。Cmd命令数据
void HT1621_2_WRCmd(uint16_t Cmd)
{
    L1621_2_CS;
    HT1621_2_WRData(0x80,4);//命令模式代码100
    HT1621_2_WRData(Cmd,8); //命令数据
    H1621_2_CS;
}
//连续数据写入函数，
void HT1621_2_WRAllData(uint16_t Addr,uint16_t *Data,uint16_t index)
{
    uint16_t i;
    L1621_2_CS;
    HT1621_2_WRData(0xa0,3); //写入数据标志101
    HT1621_2_WRData(Addr<<2,6); //写入地址数据
    for(i=0; i<index; i++)
    {
        HT1621_2_WRData(*Data,8); //写入数据
        Data++;
    }
    H1621_2_CS;
}
//HT1621初始化函数,初始化后，液晶屏所有字段均显示
void Delay(uint16_t us)
{
    while(--us);
}
void DelayMS(uint16_t iMs)
{
    uint16_t i,j;
    for(i=0; i<iMs; i++)
        for(j=0; j<65; j++) Delay(1);
}
void HT1621_1_Init(void)
{
    H1621_1_CS;
    H1621_1_WR;
    H1621_1_DAT;
    H1621_2_CS;
    H1621_2_WR;
    H1621_2_DAT;
    DelayMS(2000); //延时使LCD工作电压稳定
    HT1621_1_WRCmd(BIAS);
    HT1621_1_WRCmd(RC256); //使用内部振荡器
    HT1621_1_WRCmd(SYSDIS);
    HT1621_1_WRCmd(WDTDIS);
    HT1621_1_WRCmd(SYSEN);
    HT1621_1_WRCmd(LCDOFF);

    HT1621_2_WRCmd(BIAS);
    HT1621_2_WRCmd(RC256); //使用内部振荡器
    HT1621_2_WRCmd(SYSDIS);
    HT1621_2_WRCmd(WDTDIS);
    HT1621_2_WRCmd(SYSEN);
    HT1621_2_WRCmd(LCDOFF);
}


void setup() {
  pinMode(CS1, OUTPUT);
  pinMode(WR1, OUTPUT);
  pinMode(DT1, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(WR2, OUTPUT);
  pinMode(DT2, OUTPUT);

  HT1621_1_Init();

  DelayMS(200); //延时使LCD工作电压稳定
  
  HT1621_1_WRCmd(LCDON);
  HT1621_2_WRCmd(LCDON);
}

// the loop function runs over and over again until power down or reset
void loop() {

  uint16_t Ht1621Tab[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  uint16_t Ht1621Tab2[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

  HT1621_1_WRAllData(0,Ht1621Tab,11);     //写IC1
  HT1621_2_WRAllData(0,Ht1621Tab2,9);    //写IC2

    // H1621_1_CS;
    // H1621_1_WR;
    // H1621_1_DAT;
    // H1621_2_CS;
    // H1621_2_WR;
    // H1621_2_DAT;

    // delay(1000);


    // L1621_1_CS;
    // L1621_1_WR;
    // L1621_1_DAT;
    // L1621_2_CS;
    // L1621_2_WR;
    // L1621_2_DAT;
    // delay(1000);

}
