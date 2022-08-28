/*
 * @Author: Snitro
 * @Date: 2021-02-23 17:42:08
 * @LastEditors: Snitro
 * @LastEditTime: 2021-02-24 15:30:45
 * @Description: SSD1306����
 */

#include "oled_i2c.h"

uint8_t OLED_buffer[8][128];    // ������
uint8_t OLED_buffer_flag[128];  // ˢ�±�־
#ifdef OLED_USING_HARDWARE_I2C
I2C_HandleTypeDef OLED_hi2c;
#endif

/**
 * @description: ���������OLED
 * @param {uint8_t} arg DataReg ��������
 *                      CmdReg  ����ָ��
 * @param {uint8_t} data    1�ֽ�����
 * @return {HAL_StatusTypeDef} HAL ״̬
 */
HAL_StatusTypeDef OLED_Write_Byte(uint8_t arg, uint8_t data) {
#ifdef OLED_USING_HARDWARE_I2C
    return HAL_I2C_Mem_Write(&OLED_hi2c, OLED_Addr, arg, I2C_MEMADD_SIZE_8BIT,&data, 1, 0xff);
#endif
}

/**
 * @description:  ��Ӳ��I2C��ʽ��ʼ��OLED
 * @param {I2C_HandleTypeDef} hi2c Ӳ��i2c���
 * @return {*}
 */
#ifdef OLED_USING_HARDWARE_I2C
void OLED_Init(I2C_HandleTypeDef hi2c) {
    OLED_hi2c = hi2c;
#endif
    OLED_Write_Byte(CmdReg, OLED_OFF);  //�ر���ʾ��
    OLED_Write_Byte(CmdReg, 0x20);  //�����ڴ�ѰַģʽSet Memory Addressing Mode
    // 00,ˮƽѰַģʽ 01,��ֱѰַģʽ 02,ҳ��Ѱַģʽ(��λ)
    OLED_Write_Byte(CmdReg, 0x01);
    OLED_Write_Byte(CmdReg, 0x81);  //���öԱȶ�
    OLED_Write_Byte(CmdReg, 0xff);  //�Աȶ�,��ֵԽ��Աȶ�Խ��
    OLED_Write_Byte(CmdReg, 0xc8);  //ɨ�跽�� �����·�תCom scan direction
    OLED_Write_Byte(CmdReg, 0xa1);  //���ö�����ӳ�� �����ҷ�תset segment remap
    OLED_Write_Byte(CmdReg, 0xa8);  //���ö�·���ñ�(1-64)
    OLED_Write_Byte(CmdReg, 0x3f);  //�趨ֵ1/32  1/32 duty
    OLED_Write_Byte(CmdReg, 0xd3);  //������ʾƫ�� set display offset
    OLED_Write_Byte(CmdReg, 0x00);  //
    OLED_Write_Byte(CmdReg, 0xd5);  //����osc���� set osc division
    OLED_Write_Byte(CmdReg, 0x80);  //
    OLED_Write_Byte(CmdReg, 0xd8);  //�ر�������ɫģʽ set area color mode off
    OLED_Write_Byte(CmdReg, 0x05);  //
    OLED_Write_Byte(CmdReg, 0xd9);  //����Ԥ����� Set Pre-Charge Period
    OLED_Write_Byte(CmdReg, 0xf1);  //
    OLED_Write_Byte(CmdReg, 0xda);  //����com�������� set com pin configuartion
    OLED_Write_Byte(CmdReg, 0x12);  //
    OLED_Write_Byte(CmdReg, 0xdb);  //����vcomh set Vcomh
    OLED_Write_Byte(CmdReg, 0x30);  //
    OLED_Write_Byte(CmdReg, 0x8d);  //���õ�Դ������ set charge pump enable
    OLED_Write_Byte(CmdReg, 0x14);  //
    OLED_Write_Byte(CmdReg, 0xa4);  //����ȫ����ʾ  bit0��1�ף�0��
#ifdef OLED_INVERSE_COLOR
    OLED_Write_Byte(CmdReg, 0xa7);  //������ʾ
#endif
#ifndef OLED_INVERSE_COLOR
    OLED_Write_Byte(CmdReg, 0xa6);  //������ʾ
#endif
    OLED_Fill(0x00);                      //����
    OLED_Write_Byte(CmdReg, OLED_ON);  //��oled��� turn on oled panel
}

/**
 * @description: ��ջ��棬��������Ļ
 * @param {*}
 * @return {*}
 */
void OLED_Fill(uint8_t data){
    uint16_t i;
    OLED_SetPos(0, 0);

    for (i = 0; i < 8 * OLED_Width; i++) OLED_Write_Byte(DataReg, data);

    //���»���
    memset(OLED_buffer, data, sizeof(OLED_buffer));
    memset(OLED_buffer_flag, 0, sizeof(OLED_buffer_flag));
}

/**
 * @description: ���ô�ֱѰַģʽ������
 * @param {uint8_t} x ��ʼ�е�ַ
 * @param {uint8_t} up ��ʼҳ��ַ
 * @param {uint8_t} down   ����ҳ��ַ
 * @return {*}
 */
static void OLED_SetVerticalPos(uint8_t x, uint8_t up, uint8_t down) {
    OLED_Write_Byte(CmdReg, 0x22);  //����ҳ��ַ
    OLED_Write_Byte(CmdReg, up);    //��ʼ
    OLED_Write_Byte(CmdReg, down);  //����
    OLED_Write_Byte(CmdReg, 0x21);  //�����е�ַ
    OLED_Write_Byte(CmdReg, x);     //��ʼ
    OLED_Write_Byte(CmdReg, 0x7f);  //����
}

/**
 * @description: ���ô�ֱѰַģʽ�����꣬���ֽ���ҳ��ַΪ��β
 * @param {uint8_t} x ��ʼ�е�ַ
 * @param {uint8_t} page ��ʼҳ��ַ
 * @return {*}
 */
void OLED_SetPos(uint8_t x, uint8_t page) {
    OLED_SetVerticalPos(x, page, 0x07);
}

/**
 * @description: ����ˢ��
 * @param {uint8_t} l   ��ʼ�е�ַ
 * @param {uint8_t} r   �����е�ַ
 * @return {*}
 */
static void OLED_Area_Refresh(uint8_t l, uint8_t r) {
    uint8_t count = 0, i, up = 0, down = 7, x, page;

    for (i = l; i <= r; i++) count |= OLED_buffer_flag[i];

    while (!(count & (1 << up))) up++;

    while (!(count & (1 << down))) down--;

    OLED_SetVerticalPos(l, up, down);

    for (x = l; x <= r; x++)
        for (page = up; page <= down; page++)
            OLED_Write_Byte(DataReg, OLED_buffer[page][x]);

    memset(OLED_buffer_flag + l, 0, r - l + 1);
}

/**
 * @description: ȫ��ˢ��
 * @param {*}
 * @return {*}
 */
void OLED_Refresh() {
    uint8_t l = 0, r;

    while (l <= OLED_Width) {
        while (l <= OLED_Width && !OLED_buffer_flag[l]) l++;

        r = l;
        while (r <= OLED_Width && OLED_buffer_flag[r]) r++;
        r--;

        if (l <= OLED_Width) OLED_Area_Refresh(l, r);

        l = r + 1;
    }
}

/**
 * @description: ��ȡָ�����ȵĶ���������
 * @param {uint8_t} *data ��ȡԴ
 * @param {uint8_t} start ��ʼλ��
 * @param {uint8_t} size  ��ȡ����
 * @return {uint8_t}      �Ϊ8λ�Ľ��
 */
static uint8_t getInt8Data(uint8_t *data, uint8_t start, uint8_t size) {
    uint8_t ret = 0, delta;

    ret = data[start / 8] >> (start % 8);
    delta = 8 - (start % 8);
    size -= delta;
    start += delta;

    if (((int8_t)size) <= 0)
        return ret & (0xFF >> (-((int8_t)size)));
    else
        return ret | (getInt8Data(data, start, size) << delta);
}

/**
 * @description: ��ָ�����ص����»��Ʋ�����һ��
 * @param {uint8_t} x   ���ص������
 * @param {uint8_t} y   ���ص�������
 * @param {uint8_t} *data   ��������Դ
 * @param {uint8_t} size    ���Ƴ���
 * @param {uint8_t} bool    ������������
 * @return {HAL_StatusTypeDef}  HAL״̬
 */
HAL_StatusTypeDef OLED_BOOL_DrawColumn(uint8_t x, uint8_t y, uint8_t *data,
                                       uint8_t size, uint8_t bool) {
    if (x >= OLED_Width || y + size > OLED_High) return HAL_ERROR;

    uint8_t pos = 0;
    while (size > 0) {
        uint8_t data_n = OLED_buffer[y / 8][x], delta;

        if (bool == OLED_BOOL_Replace)
            if (8 - (y % 8) >= size) {
                data_n = data_n - (data_n & ((0xff & (0xff << (8 - size))) >>
                                             (8 - size - (y % 8))));
                data_n |= getInt8Data(data, pos, size) << (y % 8);
                delta = size;
            } else {
                data_n = data_n - (data_n & (0xff << (y % 8)));
                data_n |= getInt8Data(data, pos, 8 - (y % 8)) << (y % 8);
                delta = 8 - (y % 8);
            }
        else if (bool == OLED_BOOL_ADD)
            if (8 - (y % 8) >= size) {
                data_n |= getInt8Data(data, pos, size) << (y % 8);
                delta = size;
            } else {
                data_n |= getInt8Data(data, pos, 8 - (y % 8)) << (y % 8);
                delta = 8 - (y % 8);
            }
        else if (bool == OLED_BOOL_Subtract)
            if (8 - (y % 8) >= size) {
                data_n &= ~(getInt8Data(data, pos, size) << (y % 8));
                delta = size;
            } else {
                data_n &= ~(getInt8Data(data, pos, 8 - (y % 8)) << (y % 8));
                delta = 8 - (y % 8);
            }

        if (data_n != OLED_buffer[y / 8][x])
            OLED_buffer_flag[x] |= 1 << (y / 8);
        OLED_buffer[y / 8][x] = data_n;

        size -= delta;
        y += delta;
        pos += delta;
    }

    return HAL_OK;
}

/**
 * @description: ��ָ�����ص����»��Ʋ�����һ��
 * @param {uint8_t} x   ���ص������
 * @param {uint8_t} y   ���ص�������
 * @param {uint8_t} *data   ��������Դ
 * @param {uint8_t} size    ���Ƴ���
 * @return {HAL_StatusTypeDef}  HAL״̬
 */
HAL_StatusTypeDef OLED_DrawColumn(uint8_t x, uint8_t y, uint8_t *data,
                                  uint8_t size) {
    return OLED_BOOL_DrawColumn(x, y, data, size, OLED_BOOL_Replace);
}

/**
 * @description: ��ָ�����ص�Ϊ���Ͻǣ����Ƶ����ַ�
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} c   �ַ�
 * @param {uint8_t} bool    ������������
 * @return {HAL_StatusTypeDef}  HAL ״̬
 */
HAL_StatusTypeDef OLED_BOOL_DrawChar(uint8_t x, uint8_t y, uint8_t c,
                                     uint8_t bool) {
    return OLED_BOOL_DrawBMP(x, y, 6, 8, ((uint8_t *)ASCII) + 6 * (c - 32),
                             bool);
}

/**
 * @description: ��ָ�����ص�Ϊ���Ͻǣ����Ƶ����ַ�
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} c   �ַ�
 * @return {HAL_StatusTypeDef}  HAL ״̬
 */
HAL_StatusTypeDef OLED_DrawChar(uint8_t x, uint8_t y, uint8_t c) {
    return OLED_BOOL_DrawBMP(x, y, 6, 8, ((uint8_t *)ASCII) + 6 * (c - 32),
                             OLED_BOOL_Replace);
}
/**
 * @description: ��ָ�����ص�Ϊ���Ͻǣ������ַ���
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} *str    �ַ���
 * @param {uint8_t} bool    ������������
 * @return {HAL_StatusTypeDef}  HAL ״̬
 */
HAL_StatusTypeDef OLED_BOOL_DrawStr(uint8_t x, uint8_t y, uint8_t *str,
                                    uint8_t bool) {
    uint16_t i = 0;

    uint8_t ret = 0;

    while (str[i] != '\0') {
        if (x + 5 >= OLED_Width) {
            x = 0;
            y += 8;
        }

        ret |= OLED_BOOL_DrawChar(x, y, str[i], bool);

        x += 6;
        i++;
    }

    return ret ? HAL_ERROR : HAL_OK;
}

/**
 * @description: ��ָ�����ص�Ϊ���Ͻǣ������ַ���
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} *str    �ַ���
 * @return {HAL_StatusTypeDef}  HAL ״̬
 */
HAL_StatusTypeDef OLED_DrawStr(uint8_t x, uint8_t y, uint8_t *str) {
    return OLED_BOOL_DrawStr(x, y, str, OLED_BOOL_Replace);
}

/**
 * @description: ����λͼ
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} width   ���
 * @param {uint8_t} high    �߶�
 * @param {uint8_t} *data   ����
 * @param {uint8_t} bool    ������������
 * @return {HAL_StatusTypeDef}  HAL״̬
 */
HAL_StatusTypeDef OLED_BOOL_DrawBMP(uint8_t x, uint8_t y, uint8_t width,
                                    uint8_t high, uint8_t *data, uint8_t bool) {
    uint8_t ret = 0;
    while (width > 0) {
        ret |= OLED_BOOL_DrawColumn(x, y, data, high, bool);
        data += (high + 7) / 8;
        width--;
        x++;
    }

    return ret ? HAL_ERROR : HAL_OK;
}

/**
 * @description: ����λͼ
 * @param {uint8_t} x   ������
 * @param {uint8_t} y   ������
 * @param {uint8_t} width   ���
 * @param {uint8_t} high    �߶�
 * @param {uint8_t} *data   ����
 * @return {HAL_StatusTypeDef}  HAL״̬
 */
HAL_StatusTypeDef OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t width,
                               uint8_t high, uint8_t *data) {
    return OLED_BOOL_DrawBMP(x, y, width, high, data, OLED_BOOL_Replace);
}