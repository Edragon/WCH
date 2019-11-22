
#ifndef _TFTP_H_
#define _TFTP_H_

#include "CH563BAS.H"


/*******************TFTP���Ĳ����붨��*****************/
#define TFTP_RRQ          01                                //������
#define TFTP_WRQ          02                                //д����
#define TFTP_DATA         03                                //���ݰ�
#define TFTP_ACK          04                                //ACK��
#define TFTP_ERROR        05                                //������
#define TFTP_OACK         06                                //��ѡ���OACK��



extern char *downfile;                                      //�ļ���
extern char *upfile; 
extern char *trmode;                                        //�ļ������ʽ
extern  char *option;                                       //����ѡ��ļ���С
extern char filesize[10];
extern UINT16 datasize ;
extern const UINT8 DESIP[];

extern UINT8 desip[4];  
extern UINT8 ReqBuffer[50];                                 //���ڱ��������
extern UINT8 ACKBuffer[20];                                 //���ڱ���ACK��
extern UINT8 DATABuffer[600];                               //���淢�͵����ݰ�
extern UINT8 errorcode;                                     //������
extern UINT8  oack_flag;                                    //���յ�OACK��־λ��
extern UINT8  opcode;
extern UINT16 block_num;
extern UINT16 sendport;
extern UINT8 Create_Request_Package(UINT8 opcode,const char *filename);

extern UINT8 Create_Ack_Package(UINT16 block_num);

extern UINT16 Create_Data_Package(UINT16 block_num,UINT16 datasize,char *sendbuf);

extern void Parse_Recv_Package(UINT8 *buf);


#endif
