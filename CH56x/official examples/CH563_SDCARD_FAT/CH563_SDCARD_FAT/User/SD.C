/********************************** (C) COPYRIGHT *******************************
* File Name          : SD.C
* Author             : tech11
* Version            : V1.00
* Date               : 2015/11/28
* Description        : SD加载文件系统部分操作程序
*                      (1)、CH563 Examples by KEIL;
*                      (2)、串口1输出监控信息,115200bps;
*                      (3)、本程序用于采集到USB通讯数据后 ，存储到SD卡
*******************************************************************************/



/*******************************************************************************/
/* 头文件包含 */
#include  <stdio.h>
#include "CH563BAS.H" 															/* CH563基本定义相关头文件 */
#include "CH563SFR.H"															/* CH563寄存器定义相关头文件 */	 															/* 硬件底层相关头文件 */
#include "SD_INC.H"						   			 			 				/* SD卡命令码定义头文件 */												
#include "PRINTF.H"															    /* 串口打印输出控制头文件 */
#include "EHCI_HCD.H"
//#include "CH56xufi.H"															/* 文件系统库涉及到子函数 */
#include "SYSFREQ.H"
#include "SD_HW.H"	
#include "CH56XUFI.H"

//#include "SD.H"

#define DEF_DISK_IN_FLAG  0x01
#define DEF_DISK_ERR_FLAG 0x02
#define DEF_ERR_SD        0x04

/*******************************************************************************/
/* 常、变量、结构体定义 */
volatile struct _SD_STAU  SD_Status;							 				/* SD卡状态结构体的定义 */
volatile sd_struct  sds;										 				/* SD卡信息结构体定义 */
UINT8   Cur_SD_Status;									 	 	 				/* 当前SD卡状态: 00---未插入; 01---插入但未初始化; 02---初始化失败; 03---初始化成功 */ 	


UINT8V  gDisk_Status;															/* 存储媒介(SD卡或U盘)状态 */
UINT16V Module_ErrCode;										   					/* 模块错误状态(见main.h相关宏定义) */

//UINT8 CH563DiskStatus;

/*******************************************************************************
* Function Name  : SD_Port_Init
* Description    : SD卡接口初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Port_Init( void )
{
	/**************************************************************************/
	/* 设置SPI1相关配置 */
	/* 设置SPI1为主机模式: MOSI\SCK输出、主机时钟模式0、3线接口、SPI主机模式 */
	R8_SPI1_CTRL_MOD = ( RB_SPI_MOSI_OE | RB_SPI_SCK_OE );
	R8_SPI1_CTRL_DMA = 0x00;									 			    /* 不启用DMA */
	R8_SPI1_CLOCK_DIV = 0x10;													/* SPI时钟为系统时钟16分频 */	

	/* 设置SPI口对应的IO口方向 */
	R32_PB_DIR &= ~( PIN_SD_SPI_MISO );											/* 设置MISO方向为输入 */
	R32_PB_DIR |= ( PIN_SD_SPI_SCS | PIN_SD_SPI_SCK | PIN_SD_SPI_MOSI );		/* 设置SCS、SCK、MOSI方向为输出 */
//	R32_PB_PD &= ~( PIN_SD_SPI_MISO );											/* 设置MISO上拉 */
//	R32_PB_PU |= ( PIN_SD_SPI_MISO );	

	/* 开启响应IO口斯密特功能 */
//	R32_PB_SMT = ( PIN_SD_SPI_SCS | PIN_SD_SPI_SCK | PIN_SD_SPI_MOSI | PIN_SD_SPI_MISO );

	/* 中断配置 */
	R8_SPI1_INTER_EN = 0x00;													/* 禁止全部中断 */

	/**************************************************************************/
	/* 设置SD_INSERT、SD_WP对应的IO口方向为输入 */	
	R32_PB_DIR &= ~( PIN_SD_INSERT );                                           /* 设置方向为输入 */
    R32_PB_PD &= ~( PIN_SD_INSERT );                                            /* 设置上拉 */
    R32_PB_PU |= ( PIN_SD_INSERT );
    R32_PD_DIR &= ~( PIN_SD_WP  );
    R32_PD_PD &= ~( PIN_SD_WP );
    R32_PD_PU |= ( PIN_SD_WP );  

}

/*******************************************************************************
* Function Name  : SD_SPI_HighSpeed_Set
* Description    : 设置SD卡接口的SPI读写时为高速模式
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_SPI_HighSpeed_Set( void )
{   
	R8_SPI1_CLOCK_DIV = 0x04;													/* SPI时钟为系统时钟4分频 */	
}

/*******************************************************************************
* Function Name  : SD_Check_Insert
* Description    : 检测SD卡是否插入,插入则返回TRUE; 否则返回FALSE     
* Input          : None
* Output         : None
* Return         : FALSE or TRUE
*******************************************************************************/
UINT8 SD_Check_Insert( void )
{
	if( SD_INSERT_PIN_IN( ) )
    {
		SD_Status.SD_Inster = 0x00;
        return( FALSE );										 				/* SD卡未插入 */
  	}
  	else 
  	{
		Delay_us( 1 );
		if( !SD_INSERT_PIN_IN( ) )
		{
	    	SD_Status.SD_Inster = 0x01;
	        return( TRUE );										 				/* SD卡插入 */
		}
		else
		{
			return( FALSE );									 				/* SD卡未插入 */
		}
  	}
}

/*******************************************************************************
* Function Name  : SD_Check_WP
* Description    : 检测SD卡是否写保护,写保护返回TRUE; 否则返回FALSE(写保护输出             
*                  高电平,否则低电平)
* Input          : None
* Output         : None
* Return         : FALSE or TRUE
*******************************************************************************/
UINT8 SD_Check_WP( void )
{
	return( FALSE );										 					/* SD卡未写保护 */
}

/*******************************************************************************
* Function Name  : SD_SendCmd
* Description    : 向SD卡发送命令   
* Input          : cmd---命令码
*                  *param---命令码相关4个字节参数
*                  resptype---当前命令码返回类型(字节数)
*                  *resp---返回参数 
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_SendCmd( UINT8 cmd, UINT8 *param, UINT8 resptype, UINT8 *resp )
{
    UINT32 i, rlen;
    UINT8 tmp;

    SD_CS_PIN_LOW( );                                               
    SD_SPI1_SENDBYTE( ( cmd & 0x3F ) | 0x40 );                                  /* 发送命令头和命令字 send command header and word */    
    SD_SPI1_SENDBYTE( param[ 3 ] );                                             /* 发送参数 send parameters */
    SD_SPI1_SENDBYTE( param[ 2 ] );                                             /* 发送参数 send parameters */
    SD_SPI1_SENDBYTE( param[ 1 ] );                                             /* 发送参数 send parameters */
    SD_SPI1_SENDBYTE( param[ 0 ] );                                             /* 发送参数 send parameters */ 
    SD_SPI1_SENDBYTE( 0x95 );                                                   /* CRC校验码,只用于第1个命令 CRC,only used for the first command */
    
    rlen = 0;
    switch( resptype )                                                          /* 根据不同的命令,得到不同的响应长度 */
    {                                                                           /* according various command,get the various response length */
        case R1:
        case R1B: 
            rlen = 1;  
            break;         
        case R2:  
            rlen = 2;     
            break;         
        case R3:  
            rlen = 5;     
            break;                
        default:  
            SD_SPI1_SENDBYTE( 0xFF );    
            SD_CS_PIN_HIGH( );                         
            return ( SD_ERR_CMD_RESPTYPE );                                     /* 返回命令响应类型错误 return error of command response type */
    }
    i = 0;                
    do                                                                          /* 等待响应,响应的开始位为0 */
    {                                                                           /* Wait for a response,a response is a start bit(zero) */ 
        SD_SPI1_RECVBYTE( tmp );
        i++;
    }while( ( ( tmp & 0x80 ) != 0x00 ) && ( i < SD_CMD_TIMEOUT ) );

    if( i >= SD_CMD_TIMEOUT )
    {                                                 
        SD_CS_PIN_HIGH( );
        return( SD_ERR_CMD_TIMEOUT );                                           /* 返回命令超时 return response timeout of command */
    }   
    resp[ 0 ] = 0xfe;
    i = ( rlen - 1 );
    do{
        resp[ i ] = tmp;
        SD_SPI1_RECVBYTE( tmp );                                                /* 循环的最后发送8clock  at the last recycle,clock out 8 clock */
    }while(i--);
    SD_CS_PIN_HIGH( );
    return( SD_NO_ERR );                                                        /* 返回执行成功 return perform sucessfully */
}

/*******************************************************************************
* Function Name  : SD_Read_Register
* Description    : 读取SD卡寄存器的值    
* Input          : buf---返回数据缓冲区
*                  length---需要读取的长度
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Read_Register( UINT8 *buf , UINT32 length )
{
    UINT8 i,res;
   
    i = 0;
    SD_CS_PIN_LOW( );
    do{
        SD_SPI1_RECVBYTE( res );
        i++;
    }while( ( res == 0xFF ) && ( i < SD_READREG_TIMEOUT ) );  
    if( i > SD_READREG_TIMEOUT ) 
    {
        SD_CS_PIN_HIGH( );
        return( SD_ERR_TIMEOUT_READ );                                          /* 返回读操作超时错误 */            
    }
    if( res != SD_TOK_READ_STARTBLOCK ){                                        /* 不是收到数据起始令牌 not receive data start token */
        buf[ 0 ] = res;                            
        i = 1;                                                                  /* 还有len - 1个字节要接收 still len - 1 bytes will be received */
    }
    else 
    {
        i = 0;
    }    
    for( ; i < length; i++ )
    {
        SD_SPI1_RECVBYTE( buf[ i ] );                                           /* 接收数据 receive data */
    }         
    SD_SPI1_RECVBYTE( i );                                
    SD_SPI1_RECVBYTE( i );                                                      /* 读取16位CRC get 16-bit CRC */        
    SD_SPI1_SENDBYTE( 0xFF );                                                   /* 返回之前发送8个clock  clock out 8 clk before return */
    SD_CS_PIN_HIGH( );        
    return ( SD_NO_ERR );
}

/*******************************************************************************
* Function Name  : SD_Spi_Delay
* Description    : SPI总线延时    
* Input          : value---延时值,不超过255
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Spi_Delay( UINT8 value )
{
    UINT8 i;

    for( i = 0; i < value; i++ )
    {
        SD_SPI1_SENDBYTE( 0xFF );                                               /* 发送0xFF clock out 0xFF */
    }
}

/*******************************************************************************
* Function Name  : SD_Reset
* Description    : 复位SD卡,进入SPI模式
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Reset( void )
{
    UINT8 PARA[ 4 ] = { 0,0,0,0 };
    UINT8 d;
  
    SD_CS_PIN_LOW( );                                                           /* 1. 置CS为低 assert CS */  
    SD_Spi_Delay( 30 );                                                         /* 2. 至少延时 74 clock delay more than 74 clock */
    SD_CS_PIN_HIGH( );                                                          /* 3. 置CS为高 dessert CS */
    SD_Spi_Delay( 2 );                                                          /* 4. 延时2(8 clock) delay 2(8 clock) */
    return( SD_SendCmd( CMD0,PARA,CMD0_R,&d ) );
}

/*******************************************************************************
* Function Name  : SD_ReadOCR
* Description    : 读取OCR寄存器,并返回相关数据,主要是获取相关的SD卡信息
* Input          : *buf---返回数据缓冲区
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_ReadOCR( UINT8 *buf )
{  
    UINT8 PARA[ 4 ] = { 0,0,0,0 };
    UINT8 mStatus, k = 0; 

    Delay_ms( 1 );                                                              /* 延时 */
    while( 1 )
    {
        mStatus = SD_SendCmd( CMD58,PARA,CMD58_R,buf );                         /* 读取CSD寄存器并分析 */
        if( mStatus )   
        {
            return( mStatus );
        }
        if( buf[ 0 ] != 0x00 ) 
        {
            return( SD_ERR_CMD_RESP );
        }
        k++;
        if( ( k > 0x20 ) && ( ( buf[ 3 ]& 0x80 ) == 0 ) ) 
        {
            return ( SD_Power_Up_Busy );                                        /*上电忙防止读不成功,循环32次读 */
        }
        else 
        {
            break;
        }    
    }
    if( buf[ 0 ] != 0 )  
    {
        return ( SD_ERR_CMD_RESP );                                             /* 返回错误响应 */
    }
    if( ( buf[ 2 ] & 0x30 ) != 0x30 ) 
    {
        return ( SD_ERR_VOL_NOTSUSP );                                          /* 不支持3.3V */
    }
    return ( mStatus );
}

/*******************************************************************************
* Function Name  : SD_CRC_OFF
* Description    : 读取CSD寄存器并分析
* Input          : *buf---返回数据缓冲区
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_CRC_OFF( UINT8 *buf )
{  
    UINT8 PARA[ 4 ] = { 0,0,0,0 };
    UINT8 mStatus; 

    Delay_ms( 1 );                                                              /* 延时 */
    while( 1 )
    {
        mStatus = SD_SendCmd( CMD59,PARA,CMD59_R,buf );                         /* 读取CSD寄存器并分析 */
        if( mStatus )   
        {
            return( mStatus );
        }
        else 
        {
            break;
        }    
    }
    return ( mStatus );
}

/*******************************************************************************
* Function Name  : SD_ReadStatusRegister
* Description    : 读取状态寄存器
* Input          : *buffer---返回数据缓冲区
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_ReadStatusRegister( UINT8 *buffer )
{
   UINT8 param[ 4 ] = {0,0,0,0};
   
   return( SD_SendCmd( CMD13, param, CMD13_R, buffer ) );                       /* 读Card Status寄存器 */
} 

/*******************************************************************************
* Function Name  : SD_Get_SD_Type
* Description    : 获取SD卡版本V1.0还是V2.0
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Get_SD_Type( void )
{
    UINT8 mStatus,count;
    UINT8 PARA[ 4 ] = { 0x22,0x01,0x00,0x00 };
    UINT8 buf[ 16 ];
    SD_Status.SD_Type = 0x00;                                                   /* 默认SD卡为第一版 */    
    for( count = 0x00; count < 0x02; count++ )
    {
        Delay_ms( 1 );
        mStatus = SD_SendCmd( CMD8,PARA,CMD8_R,buf );                           /* 第二版接口识别 */
        if( mStatus ) 
            {
            continue; 
        }
        /* CMD8返回的第一个字节,如果为0x01则为第二版;
           如果位2为1则为第一版,如果位2为0则重试1次 */
        if( buf[ 4 ] == 0x01 )
        {
            SD_Status.SD_Type = 0x01;                                           /* 当前SD卡为第二版 */    
        }
        else if( ( buf[ 4 ] & 0x04 ) != 0x00 )
        {
            SD_Status.SD_Type = 0x00;                                           /* 当前SD卡为第一版 */                        
        }
        else
        {
            continue;
        }
    }
    return( mStatus );
}

/*******************************************************************************
* Function Name  : SD_Activation
* Description    : 使能SD卡
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Activation( void )
{
    UINT8 PARA[ 4 ] = { 0,0,0,0 },res[ 16 ];
    UINT8 mStatus,count;
    UINT32 i = 0; 
      
    for( count = 0x00; count <= 250; count++ )
    {
        /* 发送CMD58 */
        mStatus = SD_SendCmd( CMD58,PARA,CMD58_R,res );                         /* 读取CSD寄存器 */
        if( mStatus )   
        {
            continue;
        }
        /* 发送CMD55、ACMD41 */
        mStatus = SD_SendCmd(CMD55,PARA,CMD55_R,res);                           /* 特殊命令前导 */
        if( mStatus )  
        {
            continue;
        }
        if( SD_Status.SD_Type )                                                 /* SD卡为第二版 */
        {
            PARA[ 3 ] = PARA[ 3 ] | 0x40;                                       /* 第二版则HCS=1 */
        }
        mStatus = SD_SendCmd( ACMD41,PARA,ACMD41_R,res );                       /* 激活SD卡 */
        if( mStatus )  
        {
            continue;
        }
        if( ( res[ 0 ] & 0x01 ) == 0x00 )                                       /* 返回最低位为0,则初始化完成 */
         {
            break;
        }
        Delay_ms( 5 );        
    }
    if( ( i >= 250 ) && ( SD_Status.SD_Type == 0x00 )  )
    {
        /* 可能是MMC卡 */    
        for( i = 0x00; i <= 250; i++ )
        {
            mStatus = SD_SendCmd( CMD1,PARA,CMD1_R,res );                       /* 通过资料还是显示其实CMD1主要是查询是不是初始化完毕 */     
            if( mStatus )  
            {
                continue;
            }
            if( ( res[ 0 ] & MSK_IDLE ) == 0x00 )                               /* 初始化完成 */
            {
                break;        
            }
            Delay_ms( 5 );
        }
        if( i == 250 )
        {
            return( SD_ERR_TIMEOUT_WAITIDLE );                                  /* 超时,返回错误 */
        }
    }

    for( i = 0x00; i <= 250; i++ )
    {
        Delay_ms( 5 );
         /* 发送CMD58 */
        mStatus = SD_SendCmd( CMD58,PARA,CMD58_R,res );                         /* 读取CSD寄存器并分析 */
        if( mStatus )   
        {
            continue;
        }
        if( SD_Status.SD_Type == 0x01 )                                         /* 第二版SD卡  */            
        {
            if( ( res[ 3 ] & 0x80 ) == 0x00 )                                   /* SD卡电源状态错误 */
            {
                continue;    
            }
            SD_Status.SD_Cap_Type = 0x00;                                       /* 当前为小容量SD卡 */
            if( ( res[ 3 ] & 0x40 ) == 0x40 )                                   /* 检查容量状态 */
            {
                SD_Status.SD_Cap_Type = 0x01;                                   /* 当前为大容量SD卡 */    
            }
            break;
        }
        else
        {
            break;
        }
    }
    if( i == 250 )
    {
        return( SD_ERR_TIMEOUT_WAITIDLE );                                      /* 超时,返回错误 */
    }
    return ( mStatus );
}

/*******************************************************************************
* Function Name  : SD_TYPE
* Description    : 字节写
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 SD_TYPE( void )
{
    UINT8 PARA[ 4 ] = { 0,0,0,0 },res[ 100 ];
    UINT8 mStatus,count;
    UINT8 *pbuf = res;
    UINT32 i = 0; 
    UINT32 tmp,timeout=0xfffff;

    for( count = 0x00; count <= 250; count++ )
    {
        /* 发送CMD55 */
        mStatus = SD_SendCmd(CMD55,PARA,CMD55_R,res);                           /* 特殊命令前导 */
        if( mStatus )  
        {
            continue;
        }
        mStatus = SD_SendCmd( ACMD13,PARA,ACMD13_R,res );                       /* 激活SD卡 */
        if( mStatus )  
        {
            continue;
        }
        if( ( res[ 0 ] & 0x01 ) == 0x00 )                                       /* 返回最低位为0,则初始化完成 */
         {
            break;
        }
        Delay_ms( 5 );        
    }
    SD_CS_PIN_LOW( );
    i = 0;
    do
    {                                                                           /* 等待接收数据开始令牌0xFE  wait for receiving data start token 0xFE */
        SD_SPI1_RECVBYTE( tmp );
        i++;
    }while( ( tmp == 0xFF ) && ( i < timeout ) );                               /* 0xFE */
    if( i >= timeout ) 
    {
        SD_CS_PIN_HIGH( );
        return( SD_ERR_TIMEOUT_READ );
    }
    if( tmp != SD_TOK_READ_STARTBLOCK )                                         /* 块读开始令牌错误 */
    {
        SD_SPI1_SENDBYTE( 0xFF );
        SD_CS_PIN_HIGH( );
        return( SD_ERR_DATA_START_TOK );
    }
    for( i=0;i!=64;i++ )res[i] = 0;  
    /* 连续读取32个字节 */ 
    for( i=0;i!=4;i++ ){
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
        SD_SPI1_RECVBYTE( *pbuf++ );                                            /* 读取一个字节 */
    }
    SD_SPI1_RECVBYTE( i );                                
    SD_SPI1_RECVBYTE( i );                                                      /* 读取16位CRC get 16-bit CRC */
    SD_SPI1_SENDBYTE( 0xFF ); 
    SD_CS_PIN_HIGH( );
    return mStatus;
}


/*******************************************************************************
* Function Name  : SD_Get_Information
* Description    : 获取SD卡相关信息
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Get_Information( void )
{
    UINT8 mStatus,tmp;
    UINT8 PARA[ 4 ] = {0,0,0,0};
    UINT8 buf[ 16 ];

    Delay_ms( 1 );
    mStatus = SD_SendCmd( CMD9,PARA,CMD9_R,buf );                               /* 读OCR寄存器 */
    if( mStatus ) 
    {
        return ( mStatus ); 
    }
    mStatus = SD_Read_Register( buf,16 );
    if( mStatus ) 
    {
        SD_CS_PIN_HIGH( );
        return ( mStatus );
    }
    /* 对于SD卡容量的处理,第一版、第二版不一样 */
    if( ( buf[ 0 ] & 0x80 ) == 0x80 )                                           /* CSD数据错误,容量清0 */
    {
        return( SD_ERR_VOL_NOTSUSP );    
    }
    else
    {
        /* READ_BL_LEN: 位83--80: 1001------512byte */
        sds.block_len = 1 << ( buf[ 5 ] & 0x0F );
        if( sds.block_len < 512 )  
        {
            return ( SD_BLOCK_ERROR );
        }
        if( ( buf[ 0 ] & 0x40 ) == 0x40  )                                      /* CSD结构版本:第二版 */
        {
            /* 对于第二版计算容量方法:
               位83--80:------READ_BL_LEN: 
               位69--48:------C_SIZE    
               容量 = READ_BL_LEN * ( C_SIZE + 1 ) */
            sds.block_num = ( ( UINT32 )( buf[ 7 ] & 0x3F ) ) << 16 ;
            sds.block_num += ( UINT32 )buf[ 8 ] << 8;
            sds.block_num += ( UINT32 )buf[ 9 ];
            sds.block_num = sds.block_num << 10;

            /* 计算总容量 */
            sds.SD_Capability = ( ( UINT32 ) sds.block_len ) * ( ( UINT32 )( sds.block_num / 1024 ) );
        }
        else                                                                    /* CSD结构版本:第一版 */
        {
            /* 对于第一版计算容量方法:
               位83--80:------READ_BL_LEN: 
               位73--62:------C_SIZE
               位49--47:------C_SIZE_MULT 

               BLOCKNR = ( C_SIZE + 1 ) * MULT
               MULT = ( C_SIZE_MULT < 8 ) * 2 ^ ( C_SIZE_MULT + 2 )
               BLOCK_LEN = ( READ_BL_LEN < 12 ) * 2 ^ ( READ_BL_LEN )
               存储器容量 = BLOCKNR * BLOCK_LEN */

            /* 计算C_SIZE_MULT */
            tmp  = ( buf[ 9 ] & 0x03 ) << 1;
            tmp += ( ( buf[ 10 ]& 0x80 ) >> 7 ) + 2;             
            /* 计算C_SIZE */
            sds.block_num  = ( buf[ 6 ] & 0x03 ) << 10;
            sds.block_num += ( buf[ 7 ] << 2 );
            sds.block_num += ( ( buf[ 8 ] & 0xc0 ) >> 6 );
            /* 获得卡中块的数量 */                                         
            sds.block_num = ( sds.block_num ) * ( 1 << tmp );                
            /* 计算总容量 */
            sds.SD_Capability = ( ( UINT32 ) sds.block_len ) * ( ( UINT32 ) ( sds.block_num / 1024 ) );
        }
//      printf( "sds.block_len = %d\n",(UINT16)sds.block_len );    
//      printf("sds.block_num = %08ld\n",( UINT32 )sds.block_num );
        sds.block_num =  sds.SD_Capability*2;
        printf("SD_Capability =  %f G\n",( float )( ( ( float )( sds.SD_Capability / 1024 ) / 1024 )  )  );  
    }
    return( mStatus );
}

/*******************************************************************************
* Function Name  : SD_Set_Blocklen
* Description    : 设置块长度
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Set_Blocklen( UINT16 blocklen )
{
   UINT8 PARA[ 4 ],mStatus,res;
   
   PARA[ 3 ] = 0x00;
   PARA[ 2 ] = 0x00;
   PARA[ 1 ] = ( UINT8 )( blocklen >> 8 );
   PARA[ 0 ] = ( UINT8 )( blocklen );
   mStatus = SD_SendCmd( CMD16,PARA,CMD16_R,&res );
   return  ( mStatus );
}

/*******************************************************************************
* Function Name  : SD_Init
* Description    : SD卡初始化
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_Init( void )
{
    UINT8 i,mStatus;
    UINT8 buf[ 16 ];

    SD_Status.SD_Type = 0x00;                                                   /* 设置默认为V1.0卡 */            
    SD_Status.SD_Cap_Type = 0x00;                                               /* 设置默认为标准容量卡 */    
    SD_Status.SD_Write_Pro = 0x00;                                              /* 设置默认SD卡未写保护 */

/* 检测设备是否写保护 */
    mStatus = SD_Check_WP( );    
    SD_Status.SD_Write_Pro = 0x00;                                             
//  printf("SD_Status.SD_Write_Pro=%02x\n",(UINT16)SD_Status.SD_Write_Pro);
	
/* 一、初始化前准备 */
    SD_CS_PIN_HIGH( );                                                          /* 禁止片选 */
    Delay_ms( 5 );    
    SD_CS_PIN_LOW( );                                                           /* 允许片选 */
    Delay_ms( 5 );    
    for( i = 0x00; i < 250; i++ )
    {
        SD_SPI1_SENDBYTE( 0xFF );    
    }
    SD_CS_PIN_HIGH( );                                                          /* 禁止片选 */
    while( SD_Check_Insert( ) != TRUE  );                                       /* 等待SD卡连接 */
    Delay_ms( 50 );    
/* 二、复位SD卡,执行命令码: CMD0 */      
    i = 0;
    while( 1 )          
    {
        mStatus = SD_Reset( );                                                              
        if( ( !mStatus ) ) 
        {
            break;
        }
        if( i++ > 0x50 ) break; 
    }
    if( mStatus ) 
    {
        return( mStatus );
    }
#if MY_DEBUG_PRINTF
    printf("reset\n");
#endif
    Delay_ms( 50 );    

/* 三、检测当前SD卡版本(V1.0还是V2.0),执行命令码: CMD8 */      
    mStatus = SD_Get_SD_Type( );    
    if( mStatus ) 
    {
        return( mStatus );
    }  
    mStatus = SD_CRC_OFF( buf );
#if MY_DEBUG_PRINTF
    printf("s5=%02x\n",(UINT16)mStatus);
#endif
	
/* 四、读取OCR,执行命令码: CMD58 */
    mStatus = SD_ReadOCR( buf );
    if( mStatus ) 
    {
        return( mStatus );
    }
	
/* 五、激活SD卡 */
    mStatus = SD_Activation( );                                                          
    if( mStatus ) 
    {
        return( mStatus );
    }
#if MY_DEBUG_PRINTF
    printf("SD_Status.SD_Cap_Type=%02x\n",(UINT16)SD_Status.SD_Cap_Type);
    printf("s0\n");
#endif	  
	
/* 六、第一版SD卡,需要设置每块的大小为512字节 */
    if( SD_Status.SD_Type )
    {    
        mStatus = SD_Set_Blocklen( 512 );                                       /* 设置每块大小为512字节 */
        if( mStatus ) 
        {
            return( mStatus );
        }
    }
    SD_TYPE( );
	
/* 七、获取SD卡信息,并计算SD卡物理容量 */
    mStatus = SD_Get_Information( ); 
    if( mStatus ) 
    {
        return( mStatus );
    }    
#if MY_DEBUG_PRINTF
    printf("SD Card is V%x.0\n",( UINT16 )SD_Status.SD_Type + 1 );    
    printf("SD Card Cap_Type is %x \n",( UINT16 )SD_Status.SD_Cap_Type );
#endif
	
/* 八、初始化完毕之后,设置SPI接口速度为高速模式 */
    SD_SPI_HighSpeed_Set( );
    return( mStatus );
}

/*******************************************************************************
* Function Name  : SD_StopTransmission
* Description    : 强制停止传输
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_StopTransmission( void )
{
    UINT8 PARA[ 4 ] = {0,0,0,0};
    UINT8 d;
  
    return( SD_SendCmd( CMD12,PARA,CMD12_R, &d ) );
}

/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 读取多个个连续块，直到发送CMD12才被终止
* Input          : startadress---开始的Block地址
*                  *buf---缓冲区
*                  blockmun---读取的块大小
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_ReadMultiBlock( UINT32 startadress, UINT8 *buf, UINT32 blockmun )
{
    UINT8  PARA[ 4 ],rep,tmp,mStatus;
    UINT32 i = 0, timeout = 0xfFFFF;
    UINT32 k;
    UINT8  *pbuf;

    PARA[ 3 ] = ( UINT8 )( startadress >> 24 );
    PARA[ 2 ] = ( UINT8 )( startadress >> 16 );
    PARA[ 1 ] = ( UINT8 )( startadress >> 8 );
    PARA[ 0 ] = ( UINT8 )( startadress );
    mStatus = SD_SendCmd( CMD18, PARA, CMD18_R, &rep );
    if( mStatus ) 
    {
        return( mStatus ); 
    }
    pbuf = buf;
    for( k = 0; k != blockmun; k++ ){
        SD_CS_PIN_LOW( );
        i = 0;
        do
        {                                                                       /* 等待接收数据开始令牌0xFE  wait for receiving data start token 0xFE */
            SD_SPI1_RECVBYTE( tmp );
            i++;
        }while( ( tmp == 0xFF ) && ( i < timeout ) );                           /* 0xFE */
        if( i >= timeout ) 
        {
            SD_CS_PIN_HIGH( );
            return( SD_ERR_TIMEOUT_READ );
        }
        if( tmp != SD_TOK_READ_STARTBLOCK )                                     /* 块读开始令牌错误 */
        {
            SD_SPI1_SENDBYTE( 0xFF );
            SD_CS_PIN_HIGH( );
            return( SD_ERR_DATA_START_TOK );
        }
        i = 16;
        do
        {   
            /* 连续读取32个字节 */ 
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            SD_SPI1_RECVBYTE( *pbuf++ );                                        /* 读取一个字节 */
            i--;            
        }while( i ); 
        SD_SPI1_RECVBYTE( i );                                
        SD_SPI1_RECVBYTE( i );                                                  /* 读取16位CRC get 16-bit CRC */
        SD_SPI1_SENDBYTE( 0xFF ); 
        SD_CS_PIN_HIGH( );
    } 
    mStatus = SD_StopTransmission( );
    mStatus = SD_StopTransmission( );                                           /* 注意:此处必须是发送两次,否则会出错 */    
    SD_CS_PIN_LOW( );
    for( i=0;i!=50;i++ )SD_SPI1_SENDBYTE( 0xFF );
    SD_CS_PIN_HIGH( );
    return( mStatus );
}

/*******************************************************************************
* Function Name  : SD_StopMultiToken
* Description    : 发送多块写停止令牌
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_StopMultiToken( void )
{
    UINT8  i;

    i = i;                                                                      /* 仅仅去警告 */
    SD_CS_PIN_LOW( );
    SD_SPI1_SENDBYTE( 0xFF );                                                   /* 先发送8个clock send 8 clock first */
    SD_SPI1_SENDBYTE( SD_TOK_STOP_MULTI );                                      /* 发送停止数据传输令牌 send stop transmission token */
    SD_SPI1_RECVBYTE( i );
    SD_CS_PIN_HIGH( );
}

/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 连续写入多个块
* Input          : startadress---开始的Block地址
*                  *buf---缓冲区
*                  blockmun---写入的块大小
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 SD_WriteMultiBlock( UINT32 startadress, UINT8 *buf, UINT32 blockmun )
{
    UINT8  PARA[ 4 ], rep, tmp, mStatus;
    UINT32 i = 0, timeout = 0xfffff, k;
 
    PARA[ 3 ] = ( UINT8 )( startadress >> 24 );
    PARA[ 2 ] = ( UINT8 )( startadress >> 16);
    PARA[ 1 ] = ( UINT8 )( startadress >> 8);
    PARA[ 0 ] = ( UINT8 )( startadress );
    mStatus = SD_SendCmd( CMD25,PARA,CMD25_R,&rep );
    if( mStatus )  
    {    
        return( mStatus ); 
    }
    for( k = 0; k != blockmun; k++ )
    {
        SD_CS_PIN_LOW( );
        SD_SPI1_SENDBYTE( 0xFF );
        SD_SPI1_SENDBYTE( 0XFC );                                               /* 发送开始令牌包,表示写多个BLOCK,表示开始 */
    
        i = 16;
        do
        {
            /* 一次性发送32个字节再循环 */
            SD_SPI1_SENDBYTE( *buf++ );                                         /* 1 */
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );                                        
            SD_SPI1_SENDBYTE( *buf++ );                                         /* 10 */    
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );                                         /* 20 */
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );                                         /* 30 */
            SD_SPI1_SENDBYTE( *buf++ );
            SD_SPI1_SENDBYTE( *buf++ );
            i--;
        }while( i );
        SD_SPI1_SENDBYTE( 0x81 );                                               /* 发送CRC16校验码 send CRC16 check code */
        SD_SPI1_SENDBYTE( 0x81 );                                               /* 发送CRC16校验码 send CRC16 check code */
       
        SD_SPI1_RECVBYTE( i );                                                  /* 检测应答 */
        if( ( i & SD_RESP_DATA_MSK ) != SD_RESP_DATA_ACCETPTED )    
        {        
            SD_SPI1_SENDBYTE( 0xFF );                                           /* 返回之前发送8个clock  clock out 8 clk before return */
            SD_CS_PIN_HIGH( );
            return( SD_ERR_DATA_RESP );                                         /* 数据响应错误 data response error */
        }         
        SD_CS_PIN_HIGH( );        
        SD_CS_PIN_LOW( );
        i = 0;
        do
        {                                                                       /* 等待忙结束 wait for being busy end */
            SD_SPI1_RECVBYTE( tmp );                                            /* 数据为0时候表示忙 */
            i++;
        }while( ( tmp != 0xFF ) && ( i < timeout ) );                           /* 忙时收到的值为0 always receive 0 when card is busy */    
        if( i > timeout )
        {
            SD_CS_PIN_HIGH( );
            return( SD_ERR_TIMEOUT_WAIT );
        }  
        SD_SPI1_SENDBYTE( 0xFF );
        SD_CS_PIN_HIGH( );            
    }
    /* 强制结束数据传输 */
    SD_StopMultiToken( );                                                       /* 发送结束令牌 */
    SD_CS_PIN_LOW( );                                 
    i = 0;
    do
    {                                                                           /* 等待忙结束 wait for being busy end */
        SD_SPI1_RECVBYTE( tmp );                                                /* 数据为0时候表示忙 */
        i++;
    }while( ( tmp != 0xFF ) && ( i < timeout ) );                               /* 忙时收到的值为0 always receive 0 when card is busy */    
    if( i > timeout )
    {
        SD_CS_PIN_HIGH( );
        return(    SD_ERR_TIMEOUT_WAIT );
    }
    SD_SPI1_SENDBYTE( 0xFF ); 
    SD_CS_PIN_HIGH( );
    /* 判断是否写正确 */
    mStatus = SD_ReadStatusRegister( PARA );
    if( mStatus )  
    {    
        SD_CS_PIN_HIGH( );
        return( mStatus );
    }
    if( ( PARA[ 0 ] != 0 ) || ( PARA[ 1 ] != 0 ) )
    {
        mStatus = SD_ERR_WRITE_BLK;
        SD_CS_PIN_HIGH( );
    }
    SD_CS_PIN_LOW( );                                 
    for( i=0;i!=50;i++ )SD_SPI1_SENDBYTE( 0xFF );
    SD_CS_PIN_HIGH( );
    return( mStatus );
}

/*******************************************************************************
* Function Name  : DISK_Pre_Init
* Description    : 存储介质预初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DISK_Pre_Init( void )
{
	/* 操作SD卡存储设备 */
//	gSave_Object = 0x00;														/* 数据存储在SD卡 */

	/* SD卡操作前期准备 */
	SD_Port_Init( );															/* 初始化SD卡操作相关配置 */
	CH56xLibInit( ); 															/* CH563文件系统库初始化 */
}

/*******************************************************************************
* Function Name  : DISK_Check_Connect
* Description    : 检测存储介质是否连接
* Input          : None
* Output         : None
* Return         : 返回USB_OPERATE_SUCCESS标志成功连接;
*				   返回USB_OPERATE_ERROR表示未连接;
*******************************************************************************/
UINT8 DISK_Check_Connect( void )
{
	/***************************************************************************/
	/* 操作SD卡存储设备 */
	if( SD_Check_Insert( ) )													/* 检查SD卡是否存在 */
	{
		if( gDisk_Status & DEF_DISK_IN_FLAG )
		{
			return( 0x00 );										                /* 之前已经检测到存在直接返回 */	
		}				
		Delay_ms( 10 );
		if( SD_Check_Insert( ) )											
		{
			CH56xDiskStatus = DISK_CONNECT;										/* 磁盘已经连接,但是尚未初始化或者无法识别该磁盘 */
			gDisk_Status |= DEF_DISK_IN_FLAG;									/* 设置SD卡或U盘连接标志 */
			return( 0x00 );
		}	
	}
	else
	{
		gDisk_Status &= ~DEF_DISK_IN_FLAG;										/* 设置SD卡或U盘断开标志 */
	}
	return( 0x01 );
}

/*******************************************************************************
* Function Name  : DISK_Init
* Description    : 存储介质初始化
* Input          : None
* Output         : None
* Return         : 返回USB_OPERATE_SUCCESS标志成功连接;
*				   返回USB_OPERATE_ERROR表示未连接;
*******************************************************************************/
UINT8 DISK_Init( void )
{
	UINT8  i;
	UINT8  status = 0xFF;
	UINT32 temp32;

	/***************************************************************************/
	/* 操作SD卡存储设备 */
	/* 对SD卡进行初始化操作 */
	status = SD_Init( );
	printf("status = %02x\n",status);						 								/* SD卡初始化 */	
	if( status == 0x00 )
	{
		/* 初始化部分库需要用到的参数或变量 */
		CH56xDiskStatus = DISK_MOUNTED;											/* 磁盘已经初始化成功,但是尚未分析文件系统或者文件系统不支持 */
		//CH56xDiskStatus = DISK_READY;
		CH56xvSectorSize =  sds.block_len;
		CH56xvSectorSizeB = 0;
		temp32 = CH56xvSectorSize;
		printf("CH56xvSectorSize = %02x\n",CH56xvSectorSize);
		for( i = 0; i <= 12; i++ )
		{
			temp32 = temp32 >> 1;
			CH56xvSectorSizeB++;
			if( ( temp32 & 0x01 )== 0x01 )
			{
				break;
			} 	
		}
		printf("CH56xvSectorSizeB = %02x\n",CH56xvSectorSizeB);
		pDISK_BASE_BUF = DISK_BASE_BUF;											/* 注意：SD卡操作必须手动将磁盘缓冲区指向该指针 */
	}
	else
	{																 	
		gDisk_Status |= DEF_DISK_ERR_FLAG;										/* 设置SD卡或U盘初始化失败标志 */
		return( 0x01 );			
	}
	return( 0x00 );
}

/*******************************************************************************
* Function Name  : DISK_Remove_Deal
* Description    : 存储介质拔除处理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DISK_Remove_Deal( void )
{

}

/*******************************************************************************
* Function Name  : SD_Card_Init
* Description    : SD卡操作初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Card_Init( void ) 
{
	UINT8  status;
//	UINT16 i;

	/**************************************************************************/
	/* 四、SD卡检测、初始化、程序更新检测 */		
	printf(" Wait SD Card\n");	
	DISK_Pre_Init( );															/* 存储介质预初始化 */
	/* SD卡插拔检测 */																	
	while( DISK_Check_Connect( )!= 0x00 ) 									    /* 等待SD卡或U盘连接 */
	{
		Delay_ms( 100 );
	}  

	//READY_LED_L( );

	/* SD卡初始化 */	
	status = DISK_Init( );													    /* 存储介质初始化 */
	if( status == 0x00 )
	{
		printf("SD Init Success!\n");
	}
	else
	{
		/* 检测到TF初始化失败*/
		Module_ErrCode |= DEF_ERR_SD;										    /* 设置SD卡错误标志 */	
		printf("SD Init Error!\n");
	}
}

// /*******************************************************************************
// * Function Name  : SD_Save
// * Description    : SD卡文件存储子函数
// * Input          : pname：存储的文件名
//                    Num： 输出数据长度
//                    Data：输出数据
// * Output         : None
// * Return         : 操作成功返回：ERR_SUCCESS
// 				   其他值操作不成功	 
// *******************************************************************************/
// UINT8 SD_Save( UINT8 *pname, UINT32 Num, UINT8 *Data )
// {
// 	UINT8 i;
// 	strcpy( (PCHAR)&mCmdParam.Open.mPathName, (PCCHAR)pname ); 
// 	i = CH563FileOpen( );											            /* 打开文件 */
// #if MY_DEBUG_PRINTF
// 	printf( "CH563FileOpen:%x \n", i );
// #endif
// 	if( i == ERR_SUCCESS ){	
// 	    mCmdParam.ByteLocate.mByteOffset = 0xFFFFFFFF;
// 		CH563ByteLocate( );												        /* 文件偏移到文件尾 */
// 	}
// 	else if( i == ERR_MISS_FILE ){
// 		strcpy( (PCHAR)&mCmdParam.Create.mPathName, (PCCHAR)pname ); 
// 		CH563FileCreate( );   
// 	}																	        /* 创建文件 */
// 	else return( i );
// 	mCmdParam.ByteWrite.mByteBuffer = Data;
// 	mCmdParam.ByteWrite.mByteCount = Num;
// 	i = CH563ByteWrite( );											            /* 写文件 */
// #if MY_DEBUG_PRINTF
// 	printf( "CH563ByteWrite:%x \n", i );
// #endif
// 	if( i != ERR_SUCCESS )	 return ( i );
// 	mCmdParam.Close.mUpdateLen = 1;
// 	i = CH563FileClose( );											            /* 关闭文件，更新文件长度 */
// #if MY_DEBUG_PRINTF
// 	printf( "CH563FileClose:%x \n" , i );
// #endif
// 	return( i );
// }

/*********************************END OF FILE**********************************/


