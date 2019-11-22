/*****************************************
CH563网络内存配置：保留一个连接
******************************************/
#ifndef __CH563NETConfig_H__
#define __CH563NETConfig_H__
	
#define CH563NET_MAX_SOCKET_NUM               4                                 /* Socket的个数，用户可以配置，默认为4个Socket,最大为32 */
	
#define RX_QUEUE_ENTRIES                      3                                 /* CH563 MAC接收描述符队列，此值不得小于2,可配置默认为7，最大为32 */
	
#define CH563NET_TCP_MSS                      512                              /* tcp MSS的大小*/
	
#define CH563_MEM_HEAP_SIZE                   4600                              /* 内存堆大小 */

#define CH563NET_NUM_IPRAW                    1                                 /* IPRAW连接的个数 */ 	
	
#define CH563NET_NUM_TCP                      1                                 /* TCP连接的个数 */ 
	
#define CH563NET_NUM_UDP                      1                                 /* UDP连接的个数 */   

#define CH563NET_NUM_TCP_LISTEN               1                                 /* TCP监听的个数 */


#endif


