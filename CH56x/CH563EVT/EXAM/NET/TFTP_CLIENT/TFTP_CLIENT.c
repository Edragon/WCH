/****************************ch563TFTP�ӿں���*****************************/


/***************************************************************************/
/* ͷ�ļ�����*/
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "CH563BAS.H"
#include "CH563NET.H"
#include "TFTP.H"

/***************************************************************************/
/* �������� */
char *downfile="111.txt";                                                       //�ļ���
char *upfile="sendfile"; 
char *trmode="octet";                                                           //�ļ������ʽ
char *option="tsize";                                                           //����ѡ��ļ���С
char filesize[10];
char *sendfile="123456789ABCDEFGHIJKLMN123456789ABCDEFGHIJKLMN";
               
UINT8  desip[4];
UINT8  ReqBuffer[50];                                                            //���ڱ��������
UINT8  ACKBuffer[20];                                                            //���ڱ���ACK��
UINT8  DATABuffer[600];
UINT16 datasize ;                                                               //�����tftp�����������ص��ļ�
UINT8  errorcode;                                                               //������
UINT8  opcode;
UINT16 block_num;
UINT16 sendport;
UINT32 sendnum=0;
UINT8  RW;                                                                      //��д��������ɱ�־��0-����1-д
/*************************************************************************
���ܣ���װ�ͻ��������

���룺filename���ļ���
      trmode���ļ�����ģʽ
      option���ļ���Сѡ��
      filesize-�ļ����䳤�ȣ���������Ϊ0��д������Ϊʵ���ϴ��ļ�����
      
�������װ��ɺ�������ĳ���
**************************************************************************/

UINT8 Create_Request_Package(UINT8 opcode,const char *filename )
{
      UINT16 i=0;
      UINT16 pos=0;

      memset(ReqBuffer,0,50);
      ReqBuffer[pos]=0;
      pos++;
      ReqBuffer[pos]=opcode;                                                    //�����룬1Ϊ������2Ϊд����
      pos++;
      for(i=0;i<strlen(filename);i++){                                          //�ļ���        
          ReqBuffer[pos]=filename[i];
          pos++;
        }
          ReqBuffer[pos]=0;
           pos++;        
      for(i=0;i<strlen(trmode);i++) {                                           //�ļ����䷽ʽ��Ĭ��octet��        
          ReqBuffer[pos]=trmode[i];
          pos++;
        }
          ReqBuffer[pos]=0;
           pos++;        
      for(i=0;i<strlen(option);i++){                                            //����ѡ��ļ���Сtsize��        
          ReqBuffer[pos]=option[i];
          pos++;
        }
          ReqBuffer[pos]=0;
          pos++;        
      if(opcode==2) {                                                           //���ݶ�/д�����������ļ����ȣ��� ������Ϊ0��д �������ļ�ʵ�ʳ���        
          datasize=strlen(sendfile);  
          sprintf(filesize,"%d",datasize);
          RW=1;
        }            
      else{
          RW=0;
          filesize[0]='0';
        }                            
      for(i=0;i<strlen(filesize);i++){                                          //����ѡ��ļ���Сtsize��        
          ReqBuffer[pos]=filesize[i];    
          pos++;
        }
          ReqBuffer[pos]=0;
           pos++;
        
      return pos;                                                               //���������ĵĳ��ȡ�
        
}

/*************************************************************************
���ܣ���װ�ͻ���ACK��

���룺block_num:���ݰ����
      
�������װ��ɺ�ACK���ĳ���
**************************************************************************/
UINT8 Create_Ack_Package(UINT16 block_num)
{
      UINT8 pos=0;
      memset(ACKBuffer,0,20);
      ACKBuffer[pos]=0;
      pos++;
      ACKBuffer[pos]=TFTP_ACK;                                                  //opcode=4;
      pos++;
      ACKBuffer[pos]=(char)(block_num>>8);                                      //����
      pos++;
      ACKBuffer[pos]=(char)(block_num);
      pos++;
      
      return pos;
  
}
   
/*************************************************************************
���ܣ���װ�ͻ���data��

���룺block_num:���ݰ����
      datasize:һ�����ݵĳ���
       
�������װ��ɺ�data���ĳ���
**************************************************************************/
UINT16 Create_Data_Package(UINT16 block_num,UINT16 datasize,char *sendbuf)
{
      UINT16 sendlen;                                                           //�������ݳ��ȣ����512
      UINT16 pos=0;
      memset(DATABuffer,0,600);
      DATABuffer[pos]=0;
      pos++;
      DATABuffer[pos]=TFTP_DATA;                                                //opcode=3;
      pos++;
      DATABuffer[pos]=(char)(block_num>>8);                                     //����
      pos++;
      DATABuffer[pos]=(char)(block_num);
      pos++;
      if(datasize>=512)
        sendlen=512;
      else 
        sendlen=datasize;
      memcpy(&DATABuffer[pos],sendbuf,sendlen);
      pos+=sendlen;
      
      return pos;
  
}

/*************************************************************************
���ܣ��������յ������ݰ�

����:  opcode:���հ�����
       block_num:�����
             
�������װ��ɺ�data���ĳ���
**************************************************************************/
void Parse_Recv_Package(UINT8 *buf)
{
  UINT32 len;

  switch(buf[1])                                                                //�������հ��Ĳ�����(���ͣ�
  {
    
    case TFTP_DATA:
           opcode=TFTP_DATA; 
           block_num=buf[3];                                                    //���յ����ݰ���ȡ�����ݰ����          
           printf("blocknum=%-8d  DATA\r\n",block_num);        
           len=Create_Ack_Package(block_num);
           CH563NET_SocketUdpSendTo(0,ACKBuffer,&len,desip,sendport);           //������Ӧ��ŵ�ack��              
           break;  
    case TFTP_ACK:
           opcode=TFTP_ACK;
           block_num=buf[3];                                                    //���յ�ACK����ȡ�����ݰ����
           printf("blocknum=%-8d  ACK\r\n",block_num);          
           if(datasize!=0){
              len=Create_Data_Package(block_num+1, datasize,sendfile);
              datasize-=(len-4);
              CH563NET_SocketUdpSendTo(0,DATABuffer,&len,desip,sendport);       //�յ�ack���󣬷���data��
           }
           else
              printf("\nupload over : len=%s\n",filesize);
           break;    
    case TFTP_ERROR:
           opcode=TFTP_ERROR;
           errorcode=buf[3];                                                    //�յ��������ȡ��������           
           printf("error:%d",errorcode);
           break;    
    case TFTP_OACK:
           opcode=TFTP_OACK;                                                    //�յ�OACK��          
           block_num=buf[3];
           printf("blocknum=0         OACK\r\n");
           if(RW==0) {                                                          //oack��Ӧ�ġ��Ƕ�����        
              len=Create_Ack_Package(0);                        
              CH563NET_SocketUdpSendTo(0,ACKBuffer,&len,desip,sendport);        //�յ�oack���󣬷��ͱ��Ϊ0��ACK��
           } 
           if(RW==1) {                                                          //oack��Ӧ����д����              
              len=Create_Data_Package(1, datasize,sendfile);
              datasize-=(len-4);
              CH563NET_SocketUdpSendTo(0,DATABuffer,&len,desip,sendport);       //�յ�oack���󣬷��ͱ��Ϊ0��data��
           }
           break;              
    default:  
           break;
  }      
}  

      
  
    
  
  
  
   
  
  
  
  
  
  
  

