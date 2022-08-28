/*
 * @Author: Snitro
 * @Date: 2021-02-23 17:42:27
 * @LastEditors: Snitro
 * @LastEditTime: 2021-02-24 15:30:55
 * @Description: SSD1306����
 */
#ifndef __OLED_I2C_H
#define __OLED_I2C_H

#define OLED_USING_HARDWARE_I2C  //ʹ��Ӳ��i2c
//#define OLED_USING_SOFTWARE_I2C //ʹ�����i2c

#include <string.h>

#include "FontLib.h"
#include "stm32f4xx_hal.h"  //�޸�Ϊ�����ͺ�
#ifdef OLED_USING_HARDWARE_I2C
#include "stm32f4xx_hal_i2c.h"
#endif

//#define OLED_INVERSE_COLOR  // OLED��ɫ��ʾ
#define OLED_Addr 0x78  // OLED IIC ��ַ

#define CmdReg 0x00     //��ʾ����ָ��
#define DataReg 0x40    //��ʾ��������
#define OLED_ON 0xaf    //����ʾ��
#define OLED_OFF 0xae   //�ر���ʾ��
#define OLED_Width 128  // OLED���128����
#define OLED_High 64    // OLED�߶�64����

#define OLED_BOOL_Replace ((uint8_t)0x00)  //�滻
#define OLED_BOOL_ADD ((uint8_t)0x01)      //��
#define OLED_BOOL_Subtract ((uint8_t)0x02) //��

#ifdef OLED_USING_HARDWARE_I2C
void OLED_Init(I2C_HandleTypeDef);
#endif

HAL_StatusTypeDef OLED_Write_Byte(uint8_t, uint8_t);
void OLED_Fill(uint8_t);
void OLED_Refresh(void);
void OLED_SetPos(uint8_t, uint8_t);
HAL_StatusTypeDef OLED_BOOL_DrawColumn(uint8_t, uint8_t, uint8_t*, uint8_t, uint8_t);
HAL_StatusTypeDef OLED_BOOL_DrawChar(uint8_t, uint8_t, uint8_t, uint8_t);
HAL_StatusTypeDef OLED_BOOL_DrawStr(uint8_t, uint8_t, uint8_t*, uint8_t);
HAL_StatusTypeDef OLED_BOOL_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint8_t);
HAL_StatusTypeDef OLED_DrawColumn(uint8_t, uint8_t, uint8_t*, uint8_t);
HAL_StatusTypeDef OLED_DrawChar(uint8_t, uint8_t, uint8_t);
HAL_StatusTypeDef OLED_DrawStr(uint8_t, uint8_t, uint8_t*);
HAL_StatusTypeDef OLED_DrawBMP(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*);

#endif
