/********************************** (C) COPYRIGHT *********************************
* File Name          : ModuleConfig.c
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : ģ�������ļ�
**********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "CH563SFR.H"
#include "SYSFREQ.H"

#include "CH563NET.H"
#include "ModuleConfig.h"
#include "ISPXT56X.H"        

UINT8 Default_cfg[MODULE_CFG_LEN]= {                                            /*ģ��Ĭ������*/
  'C','H','5','6','3','M','O','D','U','L','E',0,0,0,0,0,0,0,0,0,0,              /*ģ������*/    
  NET_MODULE_TYPE_NONE,                                                         /*��ʶģ�鴦����ģʽ(Ĭ�ϲ������κ�ģʽ)*/
  192,168,10,10,                                                                /*ģ�鱾���IP��ַ  */
  255,255,255,0,                                                                /*ģ�鱾����������� */
  192,168,10,1,                                                                 /*ģ���Ӧ�����ص�ַ */
  1000%256,1000/256,                                                            /*ģ��Դ�˿� */
  192,168,10,100,                                                               /*Ŀ��IP��ַ */
  1000%256,1000/256 ,                                                           /*Ŀ�Ķ˿�  */
  checkcode1,checkcode2                                                         /*��֤�룬������֤������Ϣ*/
}; 

UINT8   BrocatIp[4]={255,255,255,255}; 
UINT16  BrocatPort=60000;
extern  UINT8 MACAddr[6] ;     
extern  UINT8 Configbuf[MODULE_CFG_LEN]  ;


/*�����������*/
UINT8   ParseConfigbuf(UINT8 *buf)
{   
UINT16 i;
UINT32 len; 
pnet_comm net = (pnet_comm)buf;                                                 //netͨ�Žṹ��
 
   i =memcmp(net->flag,NET_MODULE_FLAG,15);                                     //�ж����������ʶ
   if(i) return 0;
   switch  (net->cmd)
   {
     case  NET_MODULE_CMD_SEARCH :                                              //��Ѱģ������
           memcpy(net->id,MACAddr,6);
           net->cmd    =   NET_MODULE_ACK_SEARCH ;          
           sprintf((char *)net->dat,"%d.%d.%d.%d",(UINT16)CFG->src_ip[0],(UINT16)CFG->src_ip[1],(UINT16)CFG->src_ip[2],(UINT16)CFG->src_ip[3]);
           net->len = strlen((char *)net->dat);
           sprintf((char*)&net->dat[net->len + 1],"%s",CFG->module_name);
           net->len += strlen((char*)&net->dat[net->len + 1]) + 1;
           len = sizeof(net_comm);
           Delay_ms(5);
           CH563NET_SocketUdpSendTo(0,buf,&len,BrocatIp,BrocatPort);  
           Delay_ms(5);          
           printf("CMD_SEARCH********************\n");
           return 0;

     case  NET_MODULE_CMD_SET :                                                 //����ģ������
           i =memcmp(net->id,MACAddr,6);
           if(i) return 0;
           net->cmd  = NET_MODULE_ACK_SET ;
           net->len = 0;   
           memcpy(Configbuf,net->dat,MODULE_CFG_LEN);
           CFG->cfg_flag[0]=checkcode1;                                         //�ڽ�β������֤��
           CFG->cfg_flag[1]=checkcode2;
           i = EEPROM_ERASE(0x1000,4096);            
           i = EEPROM_WRITE(0x1000,Configbuf,MODULE_CFG_LEN );                                 
           len = sizeof(net_comm);
           Delay_ms(5);
           CH563NET_SocketUdpSendTo(0,buf,&len,BrocatIp,BrocatPort);  
           Delay_ms(5);
           printf("CMD_SET***********************\n");
           return 1;

    case   NET_MODULE_CMD_GET :                                                 //��ȡģ������
           i =memcmp(net->id,MACAddr,6); 
           if(i) return 0;
           net->cmd  = NET_MODULE_ACK_GET ;
           net->len = (MODULE_CFG_LEN-2);                                       //����������Ϣ����λ����ʱ��Ҫ��������ֽڵ���֤��ȥ��
           memcpy(net->dat,Configbuf,(MODULE_CFG_LEN-2))  ;
           len = sizeof(net_comm);
           Delay_ms(5);
           CH563NET_SocketUdpSendTo(0,buf,&len,BrocatIp,BrocatPort);  
           Delay_ms(5);
           printf("CMD_GET***********************\n");
           return 0;
         
    case   NET_MODULE_CMD_RESET :                                               //��λģ������
           i =memcmp(net->id,MACAddr,6);
           if(i) return 0;     
           i = EEPROM_ERASE(0x1000,4096);
           i = EEPROM_WRITE(0x1000,(pmodule_cfg)Default_cfg,MODULE_CFG_LEN );                                    
           net->cmd = NET_MODULE_ACK_RESET ;
           len = sizeof(net_comm);
           Delay_ms(5);
           Delay_ms(5);
           printf("CMD_RESET*********************\n");
           return 1;
   default:
           return 0 ;
   }

}
