#ifndef __ANDROID_H__
#define __ANDROID_H__

#include "CH563BAS.H"
#include "EHCI_HCD.H"

#define		NONE_ADNROID_DEVICE			0xa0			  //非安卓设备类
#define		SET_MANU_NAME_ERROR			0xa1			  //设置厂商信息失败
#define		SET_MODEL_NAME_ERROR		0xa2			  //设置模式失败
#define		SET_DESCR_NAME_ERROR		0xa3			  //设置名称描述失败
#define		SET_VERSION_ERROR			0xa4			  //设置版本失败
#define		SET_URI_ERROR			    0xa5			  //设置URI失败
#define		SET_SERAAL_NUMBER_ERROR		0xa6			  //设置串行序列号失败
#define		START_AOA_ERROR				0xa7			  //开启AOA协议

/*安卓设备协议*/
extern UINT16 Android_Portocol;



typedef struct _AOA_INFO 
{ 
	ATTACH_DEVICE_INFO  Device;													
	qHD_Structure *qHD_Bulk_In; 												/* 批量上传端点EHCI相关结构体指针定义 */
	qHD_Structure *qHD_Bulk_Out; 												/* 批量下传端点EHCI相关结构体指针定义 */
	qTD_Structure *IntInEp1qTD;
	UINT8  BulkInEp;															/* AOA设备的IN端点地址 */
	UINT8  BulkOutEp;															/* AOA设备的OUT端点地址 */
	UINT16 BulkInEpSize;  	    												/* AOA设备的IN端点最大包大小 */	
	UINT16 BulkOutEpSize;  														/* AOA设备的OUT端点最大包大小 */	
	UINT8  InterfNumber;														/* AOA设备的接口号 */   
}AOA_INFO1;																		/* AOA信息结构体,存储当前AOA设备必要参数 */

extern UINT8 MS_CofDescrAnalyse( AOA_INFO1 *Diskinfo, UINT8 *pbuf );
extern UINT8 AOA_IN_Data( AOA_INFO1 *Aoainfo,UINT8 *pbuf,UINT16 len );
extern UINT8 AOA_OUT_Data( AOA_INFO1 *Aoainfo,UINT8 *pbuf,UINT16 len );

#endif


