#ifndef __ANDROID_H__
#define __ANDROID_H__

#include "CH563BAS.H"
#include "EHCI_HCD.H"

#define		NONE_ADNROID_DEVICE			0xa0			  //�ǰ�׿�豸��
#define		SET_MANU_NAME_ERROR			0xa1			  //���ó�����Ϣʧ��
#define		SET_MODEL_NAME_ERROR		0xa2			  //����ģʽʧ��
#define		SET_DESCR_NAME_ERROR		0xa3			  //������������ʧ��
#define		SET_VERSION_ERROR			0xa4			  //���ð汾ʧ��
#define		SET_URI_ERROR			    0xa5			  //����URIʧ��
#define		SET_SERAAL_NUMBER_ERROR		0xa6			  //���ô������к�ʧ��
#define		START_AOA_ERROR				0xa7			  //����AOAЭ��

/*��׿�豸Э��*/
extern UINT16 Android_Portocol;



typedef struct _AOA_INFO 
{ 
	ATTACH_DEVICE_INFO  Device;													
	qHD_Structure *qHD_Bulk_In; 												/* �����ϴ��˵�EHCI��ؽṹ��ָ�붨�� */
	qHD_Structure *qHD_Bulk_Out; 												/* �����´��˵�EHCI��ؽṹ��ָ�붨�� */
	qTD_Structure *IntInEp1qTD;
	UINT8  BulkInEp;															/* AOA�豸��IN�˵��ַ */
	UINT8  BulkOutEp;															/* AOA�豸��OUT�˵��ַ */
	UINT16 BulkInEpSize;  	    												/* AOA�豸��IN�˵�������С */	
	UINT16 BulkOutEpSize;  														/* AOA�豸��OUT�˵�������С */	
	UINT8  InterfNumber;														/* AOA�豸�Ľӿں� */   
}AOA_INFO1;																		/* AOA��Ϣ�ṹ��,�洢��ǰAOA�豸��Ҫ���� */

extern UINT8 MS_CofDescrAnalyse( AOA_INFO1 *Diskinfo, UINT8 *pbuf );
extern UINT8 AOA_IN_Data( AOA_INFO1 *Aoainfo,UINT8 *pbuf,UINT16 len );
extern UINT8 AOA_OUT_Data( AOA_INFO1 *Aoainfo,UINT8 *pbuf,UINT16 len );

#endif


