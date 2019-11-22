/********************************** (C) COPYRIGHT ******************************
* File Name          : CHAR.C
* Author             : WCH
* Version            : V1.0
* Date               : 2013/11/15
* Description        : �ַ���ʾ
*******************************************************************************/



/******************************************************************************/
/* ͷ�ļ����� */
#include <stdio.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"
#include "..\PARA\PARA_TFT\LCD\LCD.H"
#include "CHAR.H"

/*******************************************************************************
* Function Name  : show_pic
* Description    : ��ʾ�ַ���
* Input          : x,y:�������
*                  width,height:�����С  
*                  size:�����С
*                  p:�ַ�����ʼ��ַ    
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowString(UINT16 x,UINT16 y,UINT16 width,UINT16 height,UINT8 size,UINT8 *p)
{         
    UINT8 x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' ')){                                                /* �ж��ǲ��ǷǷ��ַ� */
        if(x>=width){x=x0;y+=size;}
        if(y>=height) break;                                                    /* �˳� */
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/*******************************************************************************
* Function Name  : show_pic
* Description    : �ַ������Գ���
* Input          : None    
* Output         : None
* Return         : None
*******************************************************************************/
void TEST_CHAR( void )
{
    UINT8 lcd_id[12];                                                           /* ���LCD ID�ַ��� */
    UINT16 c; 

    POINT_COLOR=BLACK;
    BACK_COLOR=WHITE;
    c = 40;
    LCD_ShowString(c,50,200,16,16,"3.5' TFT Test");
    sprintf((char *)lcd_id,"LCD ID:0x%x\r\n",lcddev.id);  
    LCD_ShowString(c,70,200,16,16,lcd_id);                                      /* ��ʾLCD ID */    
    LCD_ShowString(c,90,200,16,16,"2013/9/2");
    LCD_ShowString(c,110,200,16,16,"CH563 demo...");    
    Delay_ms(500);
    LCD_ShowString(c,150,200,16,16,"Mail: ");
    Delay_ms(500);
    c += 8*6;
    LCD_ShowString(c,150,200,16,16,"tech@wch.cn");
    Delay_ms(500);
    c = 40;
    LCD_ShowString(c,170,200,16,16,"Web : ");
    Delay_ms(500);
    c += 8*6;
    LCD_ShowString(c,170,200,16,16,"http://wch.cn");
    Delay_ms(500);
    c = 40;
    LCD_ShowString(c,190,200,16,16,"Tel : ");
    c += 8*6;
    Delay_ms(500);
    LCD_ShowString(c,190,200,16,16,"+86-");
    c += 8*4;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"025-");
    c += 8*4;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"5");
    c += 8;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"2");
    c += 8;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"6");
    c += 8;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"3");
    c += 8;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"8");
    c += 8;
    Delay_ms(300);
    LCD_ShowString(c,190,200,16,16,"370");
    c = 40;
    Delay_ms(500);
    LCD_ShowString(c,210,200,16,16,"Fax : ");
    c += 8*6;
    Delay_ms(500);
    LCD_ShowString(c,210,200,16,16,"+86-025-84730778"); 
}

/*********************************** endfile **********************************/