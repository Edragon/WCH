/*****************************************
CH563�����ڴ����ã�����һ������
******************************************/
#ifndef __CH563NETConfig_H__
#define __CH563NETConfig_H__
	
#define CH563NET_MAX_SOCKET_NUM               4                                 /* Socket�ĸ������û��������ã�Ĭ��Ϊ4��Socket,���Ϊ32 */
	
#define RX_QUEUE_ENTRIES                      3                                 /* CH563 MAC�������������У���ֵ����С��2,������Ĭ��Ϊ7�����Ϊ32 */
	
#define CH563NET_TCP_MSS                      512                              /* tcp MSS�Ĵ�С*/
	
#define CH563_MEM_HEAP_SIZE                   4600                              /* �ڴ�Ѵ�С */

#define CH563NET_NUM_IPRAW                    1                                 /* IPRAW���ӵĸ��� */ 	
	
#define CH563NET_NUM_TCP                      1                                 /* TCP���ӵĸ��� */ 
	
#define CH563NET_NUM_UDP                      1                                 /* UDP���ӵĸ��� */   

#define CH563NET_NUM_TCP_LISTEN               1                                 /* TCP�����ĸ��� */


#endif

