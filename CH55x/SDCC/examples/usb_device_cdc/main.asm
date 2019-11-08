;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.9.0 #11195 (MINGW64)
;--------------------------------------------------------
	.module main
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _Manuf_Des
	.globl _Prod_Des
	.globl _SerDes
	.globl _LangDes
	.globl _CfgDesc
	.globl _DevDesc
	.globl _main
	.globl _Uart1_ISR
	.globl _DeviceInterrupt
	.globl _Config_Uart1
	.globl _USBDeviceEndPointCfg
	.globl _USBDeviceIntCfg
	.globl _USBDeviceCfg
	.globl _CH554UART1SendByte
	.globl _UART1Setup
	.globl _mInitSTDIO
	.globl _mDelaymS
	.globl _CfgFsys
	.globl ___memcpy
	.globl _UIF_BUS_RST
	.globl _UIF_DETECT
	.globl _UIF_TRANSFER
	.globl _UIF_SUSPEND
	.globl _UIF_HST_SOF
	.globl _UIF_FIFO_OV
	.globl _U_SIE_FREE
	.globl _U_TOG_OK
	.globl _U_IS_NAK
	.globl _ADC_CHAN0
	.globl _ADC_CHAN1
	.globl _CMP_CHAN
	.globl _ADC_START
	.globl _ADC_IF
	.globl _CMP_IF
	.globl _CMPO
	.globl _U1RI
	.globl _U1TI
	.globl _U1RB8
	.globl _U1TB8
	.globl _U1REN
	.globl _U1SMOD
	.globl _U1SM0
	.globl _S0_R_FIFO
	.globl _S0_T_FIFO
	.globl _S0_FREE
	.globl _S0_IF_BYTE
	.globl _S0_IF_FIRST
	.globl _S0_IF_OV
	.globl _S0_FST_ACT
	.globl _CP_RL2
	.globl _C_T2
	.globl _TR2
	.globl _EXEN2
	.globl _TCLK
	.globl _RCLK
	.globl _EXF2
	.globl _CAP1F
	.globl _TF2
	.globl _RI
	.globl _TI
	.globl _RB8
	.globl _TB8
	.globl _REN
	.globl _SM2
	.globl _SM1
	.globl _SM0
	.globl _IT0
	.globl _IE0
	.globl _IT1
	.globl _IE1
	.globl _TR0
	.globl _TF0
	.globl _TR1
	.globl _TF1
	.globl _RXD
	.globl _PWM1_
	.globl _TXD
	.globl _PWM2_
	.globl _AIN3
	.globl _VBUS1
	.globl _INT0
	.globl _TXD1_
	.globl _INT1
	.globl _T0
	.globl _RXD1_
	.globl _PWM2
	.globl _T1
	.globl _UDP
	.globl _UDM
	.globl _TIN0
	.globl _CAP1
	.globl _T2
	.globl _AIN0
	.globl _VBUS2
	.globl _TIN1
	.globl _CAP2
	.globl _T2EX
	.globl _RXD_
	.globl _TXD_
	.globl _AIN1
	.globl _UCC1
	.globl _TIN2
	.globl _SCS
	.globl _CAP1_
	.globl _T2_
	.globl _AIN2
	.globl _UCC2
	.globl _TIN3
	.globl _PWM1
	.globl _MOSI
	.globl _TIN4
	.globl _RXD1
	.globl _MISO
	.globl _TIN5
	.globl _TXD1
	.globl _SCK
	.globl _IE_SPI0
	.globl _IE_TKEY
	.globl _IE_USB
	.globl _IE_ADC
	.globl _IE_UART1
	.globl _IE_PWMX
	.globl _IE_GPIO
	.globl _IE_WDOG
	.globl _PX0
	.globl _PT0
	.globl _PX1
	.globl _PT1
	.globl _PS
	.globl _PT2
	.globl _PL_FLAG
	.globl _PH_FLAG
	.globl _EX0
	.globl _ET0
	.globl _EX1
	.globl _ET1
	.globl _ES
	.globl _ET2
	.globl _E_DIS
	.globl _EA
	.globl _P
	.globl _F1
	.globl _OV
	.globl _RS0
	.globl _RS1
	.globl _F0
	.globl _AC
	.globl _CY
	.globl _UEP1_DMA_H
	.globl _UEP1_DMA_L
	.globl _UEP1_DMA
	.globl _UEP0_DMA_H
	.globl _UEP0_DMA_L
	.globl _UEP0_DMA
	.globl _UEP2_3_MOD
	.globl _UEP4_1_MOD
	.globl _UEP3_DMA_H
	.globl _UEP3_DMA_L
	.globl _UEP3_DMA
	.globl _UEP2_DMA_H
	.globl _UEP2_DMA_L
	.globl _UEP2_DMA
	.globl _USB_DEV_AD
	.globl _USB_CTRL
	.globl _USB_INT_EN
	.globl _UEP4_T_LEN
	.globl _UEP4_CTRL
	.globl _UEP0_T_LEN
	.globl _UEP0_CTRL
	.globl _USB_RX_LEN
	.globl _USB_MIS_ST
	.globl _USB_INT_ST
	.globl _USB_INT_FG
	.globl _UEP3_T_LEN
	.globl _UEP3_CTRL
	.globl _UEP2_T_LEN
	.globl _UEP2_CTRL
	.globl _UEP1_T_LEN
	.globl _UEP1_CTRL
	.globl _UDEV_CTRL
	.globl _USB_C_CTRL
	.globl _TKEY_DATH
	.globl _TKEY_DATL
	.globl _TKEY_DAT
	.globl _TKEY_CTRL
	.globl _ADC_DATA
	.globl _ADC_CFG
	.globl _ADC_CTRL
	.globl _SBAUD1
	.globl _SBUF1
	.globl _SCON1
	.globl _SPI0_SETUP
	.globl _SPI0_CK_SE
	.globl _SPI0_CTRL
	.globl _SPI0_DATA
	.globl _SPI0_STAT
	.globl _PWM_CK_SE
	.globl _PWM_CTRL
	.globl _PWM_DATA1
	.globl _PWM_DATA2
	.globl _T2CAP1H
	.globl _T2CAP1L
	.globl _T2CAP1
	.globl _TH2
	.globl _TL2
	.globl _T2COUNT
	.globl _RCAP2H
	.globl _RCAP2L
	.globl _RCAP2
	.globl _T2MOD
	.globl _T2CON
	.globl _SBUF
	.globl _SCON
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _XBUS_AUX
	.globl _PIN_FUNC
	.globl _P3_DIR_PU
	.globl _P3_MOD_OC
	.globl _P3
	.globl _P2
	.globl _P1_DIR_PU
	.globl _P1_MOD_OC
	.globl _P1
	.globl _ROM_CTRL
	.globl _ROM_DATA_H
	.globl _ROM_DATA_L
	.globl _ROM_DATA
	.globl _ROM_ADDR_H
	.globl _ROM_ADDR_L
	.globl _ROM_ADDR
	.globl _GPIO_IE
	.globl _IP_EX
	.globl _IE_EX
	.globl _IP
	.globl _IE
	.globl _WDOG_COUNT
	.globl _RESET_KEEP
	.globl _WAKE_CTRL
	.globl _CLOCK_CFG
	.globl _PCON
	.globl _GLOBAL_CFG
	.globl _SAFE_MOD
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _B
	.globl _ACC
	.globl _PSW
	.globl _LineCoding
	.globl _Ep2Buffer
	.globl _Ep1Buffer
	.globl _Ep0Buffer
	.globl _UpPoint2_Busy
	.globl _USBBufOutPoint
	.globl _USBByteCount
	.globl _UartByteCount
	.globl _Uart_Output_Point
	.globl _Uart_Input_Point
	.globl _Receive_Uart_Buf
	.globl _SetupReqBuf
	.globl _pDescr
	.globl _UsbConfig
	.globl _Count
	.globl _SetupReq
	.globl _SetupLen
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_PSW	=	0x00d0
_ACC	=	0x00e0
_B	=	0x00f0
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_SAFE_MOD	=	0x00a1
_GLOBAL_CFG	=	0x00b1
_PCON	=	0x0087
_CLOCK_CFG	=	0x00b9
_WAKE_CTRL	=	0x00a9
_RESET_KEEP	=	0x00fe
_WDOG_COUNT	=	0x00ff
_IE	=	0x00a8
_IP	=	0x00b8
_IE_EX	=	0x00e8
_IP_EX	=	0x00e9
_GPIO_IE	=	0x00c7
_ROM_ADDR	=	0x8584
_ROM_ADDR_L	=	0x0084
_ROM_ADDR_H	=	0x0085
_ROM_DATA	=	0x8f8e
_ROM_DATA_L	=	0x008e
_ROM_DATA_H	=	0x008f
_ROM_CTRL	=	0x0086
_P1	=	0x0090
_P1_MOD_OC	=	0x0092
_P1_DIR_PU	=	0x0093
_P2	=	0x00a0
_P3	=	0x00b0
_P3_MOD_OC	=	0x0096
_P3_DIR_PU	=	0x0097
_PIN_FUNC	=	0x00c6
_XBUS_AUX	=	0x00a2
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_SCON	=	0x0098
_SBUF	=	0x0099
_T2CON	=	0x00c8
_T2MOD	=	0x00c9
_RCAP2	=	0xcbca
_RCAP2L	=	0x00ca
_RCAP2H	=	0x00cb
_T2COUNT	=	0xcdcc
_TL2	=	0x00cc
_TH2	=	0x00cd
_T2CAP1	=	0xcfce
_T2CAP1L	=	0x00ce
_T2CAP1H	=	0x00cf
_PWM_DATA2	=	0x009b
_PWM_DATA1	=	0x009c
_PWM_CTRL	=	0x009d
_PWM_CK_SE	=	0x009e
_SPI0_STAT	=	0x00f8
_SPI0_DATA	=	0x00f9
_SPI0_CTRL	=	0x00fa
_SPI0_CK_SE	=	0x00fb
_SPI0_SETUP	=	0x00fc
_SCON1	=	0x00c0
_SBUF1	=	0x00c1
_SBAUD1	=	0x00c2
_ADC_CTRL	=	0x0080
_ADC_CFG	=	0x009a
_ADC_DATA	=	0x009f
_TKEY_CTRL	=	0x00c3
_TKEY_DAT	=	0xc5c4
_TKEY_DATL	=	0x00c4
_TKEY_DATH	=	0x00c5
_USB_C_CTRL	=	0x0091
_UDEV_CTRL	=	0x00d1
_UEP1_CTRL	=	0x00d2
_UEP1_T_LEN	=	0x00d3
_UEP2_CTRL	=	0x00d4
_UEP2_T_LEN	=	0x00d5
_UEP3_CTRL	=	0x00d6
_UEP3_T_LEN	=	0x00d7
_USB_INT_FG	=	0x00d8
_USB_INT_ST	=	0x00d9
_USB_MIS_ST	=	0x00da
_USB_RX_LEN	=	0x00db
_UEP0_CTRL	=	0x00dc
_UEP0_T_LEN	=	0x00dd
_UEP4_CTRL	=	0x00de
_UEP4_T_LEN	=	0x00df
_USB_INT_EN	=	0x00e1
_USB_CTRL	=	0x00e2
_USB_DEV_AD	=	0x00e3
_UEP2_DMA	=	0xe5e4
_UEP2_DMA_L	=	0x00e4
_UEP2_DMA_H	=	0x00e5
_UEP3_DMA	=	0xe7e6
_UEP3_DMA_L	=	0x00e6
_UEP3_DMA_H	=	0x00e7
_UEP4_1_MOD	=	0x00ea
_UEP2_3_MOD	=	0x00eb
_UEP0_DMA	=	0xedec
_UEP0_DMA_L	=	0x00ec
_UEP0_DMA_H	=	0x00ed
_UEP1_DMA	=	0xefee
_UEP1_DMA_L	=	0x00ee
_UEP1_DMA_H	=	0x00ef
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_CY	=	0x00d7
_AC	=	0x00d6
_F0	=	0x00d5
_RS1	=	0x00d4
_RS0	=	0x00d3
_OV	=	0x00d2
_F1	=	0x00d1
_P	=	0x00d0
_EA	=	0x00af
_E_DIS	=	0x00ae
_ET2	=	0x00ad
_ES	=	0x00ac
_ET1	=	0x00ab
_EX1	=	0x00aa
_ET0	=	0x00a9
_EX0	=	0x00a8
_PH_FLAG	=	0x00bf
_PL_FLAG	=	0x00be
_PT2	=	0x00bd
_PS	=	0x00bc
_PT1	=	0x00bb
_PX1	=	0x00ba
_PT0	=	0x00b9
_PX0	=	0x00b8
_IE_WDOG	=	0x00ef
_IE_GPIO	=	0x00ee
_IE_PWMX	=	0x00ed
_IE_UART1	=	0x00ec
_IE_ADC	=	0x00eb
_IE_USB	=	0x00ea
_IE_TKEY	=	0x00e9
_IE_SPI0	=	0x00e8
_SCK	=	0x0097
_TXD1	=	0x0097
_TIN5	=	0x0097
_MISO	=	0x0096
_RXD1	=	0x0096
_TIN4	=	0x0096
_MOSI	=	0x0095
_PWM1	=	0x0095
_TIN3	=	0x0095
_UCC2	=	0x0095
_AIN2	=	0x0095
_T2_	=	0x0094
_CAP1_	=	0x0094
_SCS	=	0x0094
_TIN2	=	0x0094
_UCC1	=	0x0094
_AIN1	=	0x0094
_TXD_	=	0x0093
_RXD_	=	0x0092
_T2EX	=	0x0091
_CAP2	=	0x0091
_TIN1	=	0x0091
_VBUS2	=	0x0091
_AIN0	=	0x0091
_T2	=	0x0090
_CAP1	=	0x0090
_TIN0	=	0x0090
_UDM	=	0x00b7
_UDP	=	0x00b6
_T1	=	0x00b5
_PWM2	=	0x00b4
_RXD1_	=	0x00b4
_T0	=	0x00b4
_INT1	=	0x00b3
_TXD1_	=	0x00b2
_INT0	=	0x00b2
_VBUS1	=	0x00b2
_AIN3	=	0x00b2
_PWM2_	=	0x00b1
_TXD	=	0x00b1
_PWM1_	=	0x00b0
_RXD	=	0x00b0
_TF1	=	0x008f
_TR1	=	0x008e
_TF0	=	0x008d
_TR0	=	0x008c
_IE1	=	0x008b
_IT1	=	0x008a
_IE0	=	0x0089
_IT0	=	0x0088
_SM0	=	0x009f
_SM1	=	0x009e
_SM2	=	0x009d
_REN	=	0x009c
_TB8	=	0x009b
_RB8	=	0x009a
_TI	=	0x0099
_RI	=	0x0098
_TF2	=	0x00cf
_CAP1F	=	0x00cf
_EXF2	=	0x00ce
_RCLK	=	0x00cd
_TCLK	=	0x00cc
_EXEN2	=	0x00cb
_TR2	=	0x00ca
_C_T2	=	0x00c9
_CP_RL2	=	0x00c8
_S0_FST_ACT	=	0x00ff
_S0_IF_OV	=	0x00fe
_S0_IF_FIRST	=	0x00fd
_S0_IF_BYTE	=	0x00fc
_S0_FREE	=	0x00fb
_S0_T_FIFO	=	0x00fa
_S0_R_FIFO	=	0x00f8
_U1SM0	=	0x00c7
_U1SMOD	=	0x00c5
_U1REN	=	0x00c4
_U1TB8	=	0x00c3
_U1RB8	=	0x00c2
_U1TI	=	0x00c1
_U1RI	=	0x00c0
_CMPO	=	0x0087
_CMP_IF	=	0x0086
_ADC_IF	=	0x0085
_ADC_START	=	0x0084
_CMP_CHAN	=	0x0083
_ADC_CHAN1	=	0x0081
_ADC_CHAN0	=	0x0080
_U_IS_NAK	=	0x00df
_U_TOG_OK	=	0x00de
_U_SIE_FREE	=	0x00dd
_UIF_FIFO_OV	=	0x00dc
_UIF_HST_SOF	=	0x00db
_UIF_SUSPEND	=	0x00da
_UIF_TRANSFER	=	0x00d9
_UIF_DETECT	=	0x00d8
_UIF_BUS_RST	=	0x00d8
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; overlayable bit register bank
;--------------------------------------------------------
	.area BIT_BANK	(REL,OVR,DATA)
bits:
	.ds 1
	b0 = bits[0]
	b1 = bits[1]
	b2 = bits[2]
	b3 = bits[3]
	b4 = bits[4]
	b5 = bits[5]
	b6 = bits[6]
	b7 = bits[7]
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_SetupLen::
	.ds 2
_SetupReq::
	.ds 1
_Count::
	.ds 1
_UsbConfig::
	.ds 1
_pDescr::
	.ds 3
_SetupReqBuf::
	.ds 8
_Config_Uart1_uart1_buad_65536_47:
	.ds 4
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
_Receive_Uart_Buf::
	.ds 64
_Uart_Input_Point::
	.ds 1
_Uart_Output_Point::
	.ds 1
_UartByteCount::
	.ds 1
_USBByteCount::
	.ds 1
_USBBufOutPoint::
	.ds 1
_UpPoint2_Busy::
	.ds 1
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
_Ep0Buffer	=	0x0000
_Ep1Buffer	=	0x0040
_Ep2Buffer	=	0x0080
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
_LineCoding::
	.ds 7
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	reti
	.ds	7
	ljmp	_DeviceInterrupt
	.ds	5
	reti
	.ds	7
	ljmp	_Uart1_ISR
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
;	main.c:74: volatile __idata uint8_t Uart_Input_Point = 0;   //循环缓冲区写入指针，总线复位需要初始化为0
	mov	r0,#_Uart_Input_Point
	mov	@r0,#0x00
;	main.c:75: volatile __idata uint8_t Uart_Output_Point = 0;  //循环缓冲区取出指针，总线复位需要初始化为0
	mov	r0,#_Uart_Output_Point
	mov	@r0,#0x00
;	main.c:76: volatile __idata uint8_t UartByteCount = 0;      //当前缓冲区剩余待取字节数
	mov	r0,#_UartByteCount
	mov	@r0,#0x00
;	main.c:79: volatile __idata uint8_t USBByteCount = 0;      //代表USB端点接收到的数据
	mov	r0,#_USBByteCount
	mov	@r0,#0x00
;	main.c:80: volatile __idata uint8_t USBBufOutPoint = 0;    //取数据指针
	mov	r0,#_USBBufOutPoint
	mov	@r0,#0x00
;	main.c:82: volatile __idata uint8_t UpPoint2_Busy  = 0;   //上传端点是否忙标志
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x00
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'USBDeviceCfg'
;------------------------------------------------------------
;	main.c:92: void USBDeviceCfg()
;	-----------------------------------------
;	 function USBDeviceCfg
;	-----------------------------------------
_USBDeviceCfg:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
;	main.c:94: USB_CTRL = 0x00;                                                           //清空USB控制寄存器
	mov	_USB_CTRL,#0x00
;	main.c:95: USB_CTRL &= ~bUC_HOST_MODE;                                                //该位为选择设备模式
	anl	_USB_CTRL,#0x7f
;	main.c:96: USB_CTRL |=  bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;                    //USB设备和内部上拉使能,在中断期间中断标志未清除前自动返回NAK
	orl	_USB_CTRL,#0x29
;	main.c:97: USB_DEV_AD = 0x00;                                                         //设备地址初始化
	mov	_USB_DEV_AD,#0x00
;	main.c:100: USB_CTRL &= ~bUC_LOW_SPEED;
	anl	_USB_CTRL,#0xbf
;	main.c:101: UDEV_CTRL &= ~bUD_LOW_SPEED;                                             //选择全速12M模式，默认方式
	anl	_UDEV_CTRL,#0xfb
;	main.c:102: UDEV_CTRL = bUD_PD_DIS;  // 禁止DP/DM下拉电阻
	mov	_UDEV_CTRL,#0x80
;	main.c:103: UDEV_CTRL |= bUD_PORT_EN;                                                  //使能物理端口
	orl	_UDEV_CTRL,#0x01
;	main.c:104: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'USBDeviceIntCfg'
;------------------------------------------------------------
;	main.c:112: void USBDeviceIntCfg()
;	-----------------------------------------
;	 function USBDeviceIntCfg
;	-----------------------------------------
_USBDeviceIntCfg:
;	main.c:114: USB_INT_EN |= bUIE_SUSPEND;                                               //使能设备挂起中断
	orl	_USB_INT_EN,#0x04
;	main.c:115: USB_INT_EN |= bUIE_TRANSFER;                                              //使能USB传输完成中断
	orl	_USB_INT_EN,#0x02
;	main.c:116: USB_INT_EN |= bUIE_BUS_RST;                                               //使能设备模式USB总线复位中断
	orl	_USB_INT_EN,#0x01
;	main.c:117: USB_INT_FG |= 0x1F;                                                       //清中断标志
	orl	_USB_INT_FG,#0x1f
;	main.c:118: IE_USB = 1;                                                               //使能USB中断
;	assignBit
	setb	_IE_USB
;	main.c:119: EA = 1;                                                                   //允许单片机中断
;	assignBit
	setb	_EA
;	main.c:120: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'USBDeviceEndPointCfg'
;------------------------------------------------------------
;	main.c:128: void USBDeviceEndPointCfg()
;	-----------------------------------------
;	 function USBDeviceEndPointCfg
;	-----------------------------------------
_USBDeviceEndPointCfg:
;	main.c:131: UEP1_DMA = (uint16_t) Ep1Buffer;                                                      //端点1 发送数据传输地址
	mov	((_UEP1_DMA >> 0) & 0xFF),#_Ep1Buffer
	mov	((_UEP1_DMA >> 8) & 0xFF),#(_Ep1Buffer >> 8)
;	main.c:132: UEP2_DMA = (uint16_t) Ep2Buffer;                                                      //端点2 IN数据传输地址
	mov	((_UEP2_DMA >> 0) & 0xFF),#_Ep2Buffer
	mov	((_UEP2_DMA >> 8) & 0xFF),#(_Ep2Buffer >> 8)
;	main.c:133: UEP2_3_MOD = 0xCC;                                                         //端点2/3 单缓冲收发使能
	mov	_UEP2_3_MOD,#0xcc
;	main.c:134: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;                 //端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
	mov	_UEP2_CTRL,#0x12
;	main.c:136: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;                                 //端点1自动翻转同步标志位，IN事务返回NAK
	mov	_UEP1_CTRL,#0x12
;	main.c:137: UEP0_DMA = (uint16_t) Ep0Buffer;                                                      //端点0数据传输地址
	mov	((_UEP0_DMA >> 0) & 0xFF),#_Ep0Buffer
	mov	((_UEP0_DMA >> 8) & 0xFF),#(_Ep0Buffer >> 8)
;	main.c:138: UEP4_1_MOD = 0X40;                                                         //端点1上传缓冲区；端点0单64字节收发缓冲区
	mov	_UEP4_1_MOD,#0x40
;	main.c:139: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;                                 //手动翻转，OUT事务返回ACK，IN事务返回NAK
	mov	_UEP0_CTRL,#0x02
;	main.c:140: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Config_Uart1'
;------------------------------------------------------------
;cfg_uart                  Allocated to registers r5 r6 r7 
;uart1_buad                Allocated with name '_Config_Uart1_uart1_buad_65536_47'
;------------------------------------------------------------
;	main.c:148: void Config_Uart1(uint8_t *cfg_uart)
;	-----------------------------------------
;	 function Config_Uart1
;	-----------------------------------------
_Config_Uart1:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
;	main.c:150: uint32_t uart1_buad = 0;
	clr	a
	mov	_Config_Uart1_uart1_buad_65536_47,a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 1),a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 2),a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 3),a
;	main.c:151: *((uint8_t *)&uart1_buad) = cfg_uart[0];
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r4,a
	mov	_Config_Uart1_uart1_buad_65536_47,r4
;	main.c:152: *((uint8_t *)&uart1_buad+1) = cfg_uart[1];
	mov	a,#0x01
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r2,a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 0x0001),r2
;	main.c:153: *((uint8_t *)&uart1_buad+2) = cfg_uart[2];
	mov	a,#0x02
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r2,a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 0x0002),r2
;	main.c:154: *((uint8_t *)&uart1_buad+3) = cfg_uart[3];
	mov	a,#0x03
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r5,a
	mov	(_Config_Uart1_uart1_buad_65536_47 + 0x0003),r5
;	main.c:155: SBAUD1 = 256 - FREQ_SYS/16/uart1_buad; //  SBAUD1 = 256 - Fsys / 16 / baud rate
	mov	__divulong_PARM_2,_Config_Uart1_uart1_buad_65536_47
	mov	(__divulong_PARM_2 + 1),(_Config_Uart1_uart1_buad_65536_47 + 1)
	mov	(__divulong_PARM_2 + 2),(_Config_Uart1_uart1_buad_65536_47 + 2)
	mov	(__divulong_PARM_2 + 3),(_Config_Uart1_uart1_buad_65536_47 + 3)
	mov	dptr,#0x4240
	mov	b,#0x0f
	clr	a
	lcall	__divulong
	mov	r4,dpl
	clr	c
	clr	a
	subb	a,r4
	mov	_SBAUD1,a
;	main.c:156: IE_UART1 = 1;
;	assignBit
	setb	_IE_UART1
;	main.c:157: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'DeviceInterrupt'
;------------------------------------------------------------
;len                       Allocated to registers r4 r5 
;------------------------------------------------------------
;	main.c:162: void DeviceInterrupt(void) __interrupt (INT_NO_USB)                       //USB中断服务程序,使用寄存器组1
;	-----------------------------------------
;	 function DeviceInterrupt
;	-----------------------------------------
_DeviceInterrupt:
	push	bits
	push	acc
	push	b
	push	dpl
	push	dph
	push	(0+7)
	push	(0+6)
	push	(0+5)
	push	(0+4)
	push	(0+3)
	push	(0+2)
	push	(0+1)
	push	(0+0)
	push	psw
	mov	psw,#0x00
;	main.c:165: if(UIF_TRANSFER)                                                            //USB传输完成标志
	jb	_UIF_TRANSFER,00524$
	ljmp	00215$
00524$:
;	main.c:167: switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
	mov	a,#0x3f
	anl	a,_USB_INT_ST
	mov	r7,a
	mov	r6,a
	cjne	r6,#0x00,00525$
	ljmp	00206$
00525$:
	cjne	r7,#0x02,00526$
	sjmp	00103$
00526$:
	cjne	r7,#0x20,00527$
	ljmp	00201$
00527$:
	cjne	r7,#0x21,00528$
	sjmp	00101$
00528$:
	cjne	r7,#0x22,00529$
	sjmp	00102$
00529$:
	cjne	r7,#0x30,00530$
	sjmp	00106$
00530$:
	ljmp	00213$
;	main.c:169: case UIS_TOKEN_IN | 1:                                                  //endpoint 1# 端点中断上传
00101$:
;	main.c:170: UEP1_T_LEN = 0;
	mov	_UEP1_T_LEN,#0x00
;	main.c:171: UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
	mov	a,#0xfc
	anl	a,_UEP1_CTRL
	orl	a,#0x02
	mov	_UEP1_CTRL,a
;	main.c:172: break;
	ljmp	00213$
;	main.c:173: case UIS_TOKEN_IN | 2:                                                  //endpoint 2# 端点批量上传
00102$:
;	main.c:175: UEP2_T_LEN = 0;                                                    //预使用发送长度一定要清空
	mov	_UEP2_T_LEN,#0x00
;	main.c:176: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
	mov	a,#0xfc
	anl	a,_UEP2_CTRL
	orl	a,#0x02
	mov	_UEP2_CTRL,a
;	main.c:177: UpPoint2_Busy = 0;                                                  //清除忙标志
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x00
;	main.c:179: break;
	ljmp	00213$
;	main.c:180: case UIS_TOKEN_OUT | 2:                                                 //endpoint 3# 端点批量下传
00103$:
;	main.c:181: if ( U_TOG_OK )                                                     // 不同步的数据包将丢弃
	jb	_U_TOG_OK,00531$
	ljmp	00213$
00531$:
;	main.c:183: USBByteCount = USB_RX_LEN;
	mov	r0,#_USBByteCount
	mov	@r0,_USB_RX_LEN
;	main.c:184: USBBufOutPoint = 0;                                             //取数据指针复位
	mov	r0,#_USBBufOutPoint
	mov	@r0,#0x00
;	main.c:185: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_NAK;       //收到一包数据就NAK，主函数处理完，由主函数修改响应方式
	mov	a,#0xf3
	anl	a,_UEP2_CTRL
	orl	a,#0x08
	mov	_UEP2_CTRL,a
;	main.c:187: break;
	ljmp	00213$
;	main.c:188: case UIS_TOKEN_SETUP | 0:                                                //SETUP事务
00106$:
;	main.c:189: len = USB_RX_LEN;
	mov	r6,_USB_RX_LEN
	mov	r7,#0x00
;	main.c:190: if(len == (sizeof(USB_SETUP_REQ)))
	cjne	r6,#0x08,00532$
	cjne	r7,#0x00,00532$
	sjmp	00533$
00532$:
	ljmp	00193$
00533$:
;	main.c:192: SetupLen = ((uint16_t)UsbSetupBuf->wLengthH<<8) | (UsbSetupBuf->wLengthL);
	mov	dptr,#(_Ep0Buffer + 0x0007)
	movx	a,@dptr
	mov	r6,a
	mov	r7,#0x00
	mov	dptr,#(_Ep0Buffer + 0x0006)
	movx	a,@dptr
	mov	r4,#0x00
	orl	a,r7
	mov	_SetupLen,a
	mov	a,r4
	orl	a,r6
	mov	(_SetupLen + 1),a
;	main.c:193: len = 0;                                                      // 默认为成功并且上传0长度
	mov	r6,#0x00
	mov	r7,#0x00
;	main.c:194: SetupReq = UsbSetupBuf->bRequest;
	mov	dptr,#(_Ep0Buffer + 0x0001)
	movx	a,@dptr
	mov	_SetupReq,a
;	main.c:195: if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )//非标准请求
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	a,#0x60
	jz	00190$
;	main.c:197: switch( SetupReq )
	mov	a,#0x20
	cjne	a,_SetupReq,00535$
	ljmp	00194$
00535$:
	mov	a,#0x21
	cjne	a,_SetupReq,00536$
	sjmp	00107$
00536$:
	mov	a,#0x22
	cjne	a,_SetupReq,00537$
	ljmp	00194$
00537$:
;	main.c:199: case GET_LINE_CODING:   //0x21  currently configured
	sjmp	00110$
00107$:
;	main.c:200: pDescr = LineCoding;
	mov	_pDescr,#_LineCoding
	mov	(_pDescr + 1),#(_LineCoding >> 8)
	mov	(_pDescr + 2),#0x00
;	main.c:202: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;  // 本次传输长度
	clr	c
	mov	a,_SetupLen
	subb	a,#0x08
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00228$
	mov	r4,#0x08
	mov	r5,#0x00
	sjmp	00229$
00228$:
	mov	r4,_SetupLen
	mov	r5,(_SetupLen + 1)
00229$:
	mov	ar6,r4
	mov	ar7,r5
;	main.c:203: memcpy(Ep0Buffer,pDescr,len);
	mov	___memcpy_PARM_2,_pDescr
	mov	(___memcpy_PARM_2 + 1),(_pDescr + 1)
	mov	(___memcpy_PARM_2 + 2),(_pDescr + 2)
	mov	___memcpy_PARM_3,r6
	mov	(___memcpy_PARM_3 + 1),r7
	mov	dptr,#_Ep0Buffer
	mov	b,#0x00
	push	ar7
	push	ar6
	lcall	___memcpy
	pop	ar6
	pop	ar7
;	main.c:204: SetupLen -= len;
	mov	a,_SetupLen
	clr	c
	subb	a,r6
	mov	_SetupLen,a
	mov	a,(_SetupLen + 1)
	subb	a,r7
	mov	(_SetupLen + 1),a
;	main.c:205: pDescr += len;
	mov	a,r6
	add	a,_pDescr
	mov	_pDescr,a
	mov	a,r7
	addc	a,(_pDescr + 1)
	mov	(_pDescr + 1),a
;	main.c:206: break;
	ljmp	00194$
;	main.c:211: default:
00110$:
;	main.c:212: len = 0xFF;  								 					                 /*命令不支持*/
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:214: }
	ljmp	00194$
00190$:
;	main.c:218: switch(SetupReq)                                             //请求码
	mov	a,_SetupReq
	add	a,#0xff - 0x0a
	jnc	00539$
	ljmp	00187$
00539$:
	mov	a,_SetupReq
	add	a,#(00540$-3-.)
	movc	a,@a+pc
	mov	dpl,a
	mov	a,_SetupReq
	add	a,#(00541$-3-.)
	movc	a,@a+pc
	mov	dph,a
	clr	a
	jmp	@a+dptr
00540$:
	.db	00183$
	.db	00135$
	.db	00187$
	.db	00156$
	.db	00187$
	.db	00129$
	.db	00112$
	.db	00187$
	.db	00130$
	.db	00133$
	.db	00194$
00541$:
	.db	00183$>>8
	.db	00135$>>8
	.db	00187$>>8
	.db	00156$>>8
	.db	00187$>>8
	.db	00129$>>8
	.db	00112$>>8
	.db	00187$>>8
	.db	00130$>>8
	.db	00133$>>8
	.db	00194$>>8
;	main.c:220: case USB_GET_DESCRIPTOR:
00112$:
;	main.c:221: switch(UsbSetupBuf->wValueH)
	mov	dptr,#(_Ep0Buffer + 0x0003)
	movx	a,@dptr
	mov	r5,a
	cjne	r5,#0x01,00542$
	sjmp	00113$
00542$:
	cjne	r5,#0x02,00543$
	sjmp	00114$
00543$:
;	main.c:223: case 1:                                                       //设备描述符
	cjne	r5,#0x03,00125$
	sjmp	00115$
00113$:
;	main.c:224: pDescr = DevDesc;                                         //把设备描述符送到要发送的缓冲区
	mov	_pDescr,#_DevDesc
	mov	(_pDescr + 1),#(_DevDesc >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:225: len = sizeof(DevDesc);
	mov	r4,#0x12
	mov	r5,#0x00
;	main.c:226: break;
;	main.c:227: case 2:                                                        //配置描述符
	sjmp	00126$
00114$:
;	main.c:228: pDescr = CfgDesc;                                          //把设备描述符送到要发送的缓冲区
	mov	_pDescr,#_CfgDesc
	mov	(_pDescr + 1),#(_CfgDesc >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:229: len = sizeof(CfgDesc);
	mov	r4,#0x43
	mov	r5,#0x00
;	main.c:230: break;
;	main.c:231: case 3:
	sjmp	00126$
00115$:
;	main.c:232: if(UsbSetupBuf->wValueL == 0)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	jnz	00123$
;	main.c:234: pDescr = LangDes;
	mov	_pDescr,#_LangDes
	mov	(_pDescr + 1),#(_LangDes >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:235: len = sizeof(LangDes);
	mov	r4,#0x04
	mov	r5,#0x00
	sjmp	00126$
00123$:
;	main.c:237: else if(UsbSetupBuf->wValueL == 1)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	cjne	r3,#0x01,00120$
;	main.c:239: pDescr = Manuf_Des;
	mov	_pDescr,#_Manuf_Des
	mov	(_pDescr + 1),#(_Manuf_Des >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:240: len = sizeof(Manuf_Des);
	mov	r4,#0x0a
	mov	r5,#0x00
	sjmp	00126$
00120$:
;	main.c:242: else if(UsbSetupBuf->wValueL == 2)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	cjne	r3,#0x02,00117$
;	main.c:244: pDescr = Prod_Des;
	mov	_pDescr,#_Prod_Des
	mov	(_pDescr + 1),#(_Prod_Des >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:245: len = sizeof(Prod_Des);
	mov	r4,#0x14
	mov	r5,#0x00
	sjmp	00126$
00117$:
;	main.c:249: pDescr = SerDes;
	mov	_pDescr,#_SerDes
	mov	(_pDescr + 1),#(_SerDes >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:250: len = sizeof(SerDes);
	mov	r4,#0x14
	mov	r5,#0x00
;	main.c:252: break;
;	main.c:253: default:
	sjmp	00126$
00125$:
;	main.c:254: len = 0xff;                                                //不支持的命令或者出错
	mov	r4,#0xff
	mov	r5,#0x00
;	main.c:256: }
00126$:
;	main.c:257: if ( SetupLen > len )
	clr	c
	mov	a,r4
	subb	a,_SetupLen
	mov	a,r5
	subb	a,(_SetupLen + 1)
	jnc	00128$
;	main.c:259: SetupLen = len;    //限制总长度
	mov	_SetupLen,r4
	mov	(_SetupLen + 1),r5
00128$:
;	main.c:261: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;                            //本次传输长度
	clr	c
	mov	a,_SetupLen
	subb	a,#0x08
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00230$
	mov	r4,#0x08
	mov	r5,#0x00
	sjmp	00231$
00230$:
	mov	r4,_SetupLen
	mov	r5,(_SetupLen + 1)
00231$:
	mov	ar6,r4
	mov	ar7,r5
;	main.c:262: memcpy(Ep0Buffer,pDescr,len);                                  //加载上传数据
	mov	___memcpy_PARM_2,_pDescr
	mov	(___memcpy_PARM_2 + 1),(_pDescr + 1)
	mov	(___memcpy_PARM_2 + 2),(_pDescr + 2)
	mov	___memcpy_PARM_3,r6
	mov	(___memcpy_PARM_3 + 1),r7
	mov	dptr,#_Ep0Buffer
	mov	b,#0x00
	push	ar7
	push	ar6
	lcall	___memcpy
	pop	ar6
	pop	ar7
;	main.c:263: SetupLen -= len;
	mov	a,_SetupLen
	clr	c
	subb	a,r6
	mov	_SetupLen,a
	mov	a,(_SetupLen + 1)
	subb	a,r7
	mov	(_SetupLen + 1),a
;	main.c:264: pDescr += len;
	mov	a,r6
	add	a,_pDescr
	mov	_pDescr,a
	mov	a,r7
	addc	a,(_pDescr + 1)
	mov	(_pDescr + 1),a
;	main.c:265: break;
	ljmp	00194$
;	main.c:266: case USB_SET_ADDRESS:
00129$:
;	main.c:267: SetupLen = UsbSetupBuf->wValueL;                              //暂存USB设备地址
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r5,a
	mov	_SetupLen,r5
	mov	(_SetupLen + 1),#0x00
;	main.c:268: break;
	ljmp	00194$
;	main.c:269: case USB_GET_CONFIGURATION:
00130$:
;	main.c:270: Ep0Buffer[0] = UsbConfig;
	mov	dptr,#_Ep0Buffer
	mov	a,_UsbConfig
	movx	@dptr,a
;	main.c:271: if ( SetupLen >= 1 )
	clr	c
	mov	a,_SetupLen
	subb	a,#0x01
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jnc	00552$
	ljmp	00194$
00552$:
;	main.c:273: len = 1;
	mov	r6,#0x01
	mov	r7,#0x00
;	main.c:275: break;
	ljmp	00194$
;	main.c:276: case USB_SET_CONFIGURATION:
00133$:
;	main.c:277: UsbConfig = UsbSetupBuf->wValueL;
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	_UsbConfig,a
;	main.c:278: break;
	ljmp	00194$
;	main.c:281: case USB_CLEAR_FEATURE:                                            //Clear Feature
00135$:
;	main.c:282: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )                  /* 清除设备 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	anl	a,#0x1f
	jnz	00154$
;	main.c:284: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
	mov	dptr,#(_Ep0Buffer + 0x0003)
	movx	a,@dptr
	mov	r4,a
	mov	r5,#0x00
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	mov	r2,#0x00
	orl	ar5,a
	mov	a,r2
	orl	ar4,a
	cjne	r5,#0x01,00140$
	cjne	r4,#0x00,00140$
;	main.c:286: if( CfgDesc[ 7 ] & 0x20 )
	mov	dptr,#(_CfgDesc + 0x0007)
	clr	a
	movc	a,@a+dptr
	mov	r5,a
	jnb	acc.5,00557$
	ljmp	00194$
00557$:
;	main.c:292: len = 0xFF;                                        /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00194$
00140$:
;	main.c:297: len = 0xFF;                                            /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00194$
00154$:
;	main.c:300: else if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	ar5,#0x1f
	mov	r4,#0x00
	cjne	r5,#0x02,00151$
	cjne	r4,#0x00,00151$
;	main.c:302: switch( UsbSetupBuf->wIndexL )
	mov	dptr,#(_Ep0Buffer + 0x0004)
	movx	a,@dptr
	mov	r5,a
	cjne	r5,#0x01,00560$
	sjmp	00147$
00560$:
	cjne	r5,#0x02,00561$
	sjmp	00145$
00561$:
	cjne	r5,#0x03,00562$
	sjmp	00143$
00562$:
	cjne	r5,#0x81,00563$
	sjmp	00146$
00563$:
	cjne	r5,#0x82,00564$
	sjmp	00144$
00564$:
	cjne	r5,#0x83,00148$
;	main.c:305: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP3_CTRL
	orl	a,#0x02
	mov	_UEP3_CTRL,a
;	main.c:306: break;
	ljmp	00194$
;	main.c:307: case 0x03:
00143$:
;	main.c:308: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP3_CTRL,#0x73
;	main.c:309: break;
	ljmp	00194$
;	main.c:310: case 0x82:
00144$:
;	main.c:311: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP2_CTRL
	orl	a,#0x02
	mov	_UEP2_CTRL,a
;	main.c:312: break;
	ljmp	00194$
;	main.c:313: case 0x02:
00145$:
;	main.c:314: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP2_CTRL,#0x73
;	main.c:315: break;
	ljmp	00194$
;	main.c:316: case 0x81:
00146$:
;	main.c:317: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP1_CTRL
	orl	a,#0x02
	mov	_UEP1_CTRL,a
;	main.c:318: break;
	ljmp	00194$
;	main.c:319: case 0x01:
00147$:
;	main.c:320: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP1_CTRL,#0x73
;	main.c:321: break;
	ljmp	00194$
;	main.c:322: default:
00148$:
;	main.c:323: len = 0xFF;                                         // 不支持的端点
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:325: }
	ljmp	00194$
00151$:
;	main.c:329: len = 0xFF;                                                // 不是端点不支持
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:331: break;
	ljmp	00194$
;	main.c:332: case USB_SET_FEATURE:                                          /* Set Feature */
00156$:
;	main.c:333: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )                  /* 设置设备 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	anl	a,#0x1f
	jnz	00181$
;	main.c:335: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
	mov	dptr,#(_Ep0Buffer + 0x0003)
	movx	a,@dptr
	mov	r4,a
	mov	r5,#0x00
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	mov	r2,#0x00
	orl	ar5,a
	mov	a,r2
	orl	ar4,a
	cjne	r5,#0x01,00164$
	cjne	r4,#0x00,00164$
;	main.c:337: if( CfgDesc[ 7 ] & 0x20 )
	mov	dptr,#(_CfgDesc + 0x0007)
	clr	a
	movc	a,@a+dptr
	mov	r5,a
	jnb	acc.5,00161$
;	main.c:343: while ( XBUS_AUX & bUART0_TX )
00157$:
	mov	a,_XBUS_AUX
	jb	acc.7,00157$
;	main.c:347: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:348: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:349: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;                      //USB或者RXD0/1有信号时可被唤醒
	mov	_WAKE_CTRL,#0xc1
;	main.c:350: PCON |= PD;                                                                 //睡眠
	orl	_PCON,#0x02
;	main.c:351: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:352: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:353: WAKE_CTRL = 0x00;
	mov	_WAKE_CTRL,#0x00
	ljmp	00194$
00161$:
;	main.c:357: len = 0xFF;                                        /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00194$
00164$:
;	main.c:362: len = 0xFF;                                            /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00194$
00181$:
;	main.c:365: else if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_ENDP )             /* 设置端点 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	ar5,#0x1f
	mov	r4,#0x00
	cjne	r5,#0x02,00573$
	cjne	r4,#0x00,00573$
	sjmp	00574$
00573$:
	ljmp	00178$
00574$:
;	main.c:367: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
	mov	dptr,#(_Ep0Buffer + 0x0003)
	movx	a,@dptr
	mov	r4,a
	mov	r5,#0x00
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	mov	r2,#0x00
	orl	ar5,a
	mov	a,r2
	orl	ar4,a
	mov	a,r5
	orl	a,r4
	jz	00575$
	ljmp	00175$
00575$:
;	main.c:369: switch( ( ( uint16_t )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
	mov	dptr,#(_Ep0Buffer + 0x0005)
	movx	a,@dptr
	mov	r4,a
	mov	r5,#0x00
	mov	dptr,#(_Ep0Buffer + 0x0004)
	movx	a,@dptr
	mov	r3,a
	mov	r2,#0x00
	orl	ar5,a
	mov	a,r2
	orl	ar4,a
	cjne	r5,#0x01,00576$
	cjne	r4,#0x00,00576$
	sjmp	00171$
00576$:
	cjne	r5,#0x02,00577$
	cjne	r4,#0x00,00577$
	sjmp	00169$
00577$:
	cjne	r5,#0x03,00578$
	cjne	r4,#0x00,00578$
	sjmp	00167$
00578$:
	cjne	r5,#0x81,00579$
	cjne	r4,#0x00,00579$
	sjmp	00170$
00579$:
	cjne	r5,#0x82,00580$
	cjne	r4,#0x00,00580$
	sjmp	00168$
00580$:
	cjne	r5,#0x83,00172$
	cjne	r4,#0x00,00172$
;	main.c:372: UEP3_CTRL = UEP3_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点3 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP3_CTRL
	orl	a,#0x03
	mov	_UEP3_CTRL,a
;	main.c:373: break;
;	main.c:374: case 0x03:
	sjmp	00194$
00167$:
;	main.c:375: UEP3_CTRL = UEP3_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点3 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP3_CTRL
	orl	a,#0x0c
	mov	_UEP3_CTRL,a
;	main.c:376: break;
;	main.c:377: case 0x82:
	sjmp	00194$
00168$:
;	main.c:378: UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP2_CTRL
	orl	a,#0x03
	mov	_UEP2_CTRL,a
;	main.c:379: break;
;	main.c:380: case 0x02:
	sjmp	00194$
00169$:
;	main.c:381: UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP2_CTRL
	orl	a,#0x0c
	mov	_UEP2_CTRL,a
;	main.c:382: break;
;	main.c:383: case 0x81:
	sjmp	00194$
00170$:
;	main.c:384: UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP1_CTRL
	orl	a,#0x03
	mov	_UEP1_CTRL,a
;	main.c:385: break;
;	main.c:386: case 0x01:
	sjmp	00194$
00171$:
;	main.c:387: UEP1_CTRL = UEP1_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点1 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP1_CTRL
	orl	a,#0x0c
	mov	_UEP1_CTRL,a
;	main.c:388: default:
00172$:
;	main.c:389: len = 0xFF;                                    /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:391: }
	sjmp	00194$
00175$:
;	main.c:395: len = 0xFF;                                      /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	sjmp	00194$
00178$:
;	main.c:400: len = 0xFF;                                          /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:402: break;
;	main.c:403: case USB_GET_STATUS:
	sjmp	00194$
00183$:
;	main.c:404: Ep0Buffer[0] = 0x00;
	mov	dptr,#_Ep0Buffer
	clr	a
	movx	@dptr,a
;	main.c:405: Ep0Buffer[1] = 0x00;
	mov	dptr,#(_Ep0Buffer + 0x0001)
	movx	@dptr,a
;	main.c:406: if ( SetupLen >= 2 )
	clr	c
	mov	a,_SetupLen
	subb	a,#0x02
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00185$
;	main.c:408: len = 2;
	mov	r6,#0x02
	mov	r7,#0x00
	sjmp	00194$
00185$:
;	main.c:412: len = SetupLen;
	mov	r6,_SetupLen
	mov	r7,(_SetupLen + 1)
;	main.c:414: break;
;	main.c:415: default:
	sjmp	00194$
00187$:
;	main.c:416: len = 0xff;                                                    //操作失败
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:418: }
	sjmp	00194$
00193$:
;	main.c:423: len = 0xff;                                                         //包长度错误
	mov	r6,#0xff
	mov	r7,#0x00
00194$:
;	main.c:425: if(len == 0xff)
	cjne	r6,#0xff,00199$
	cjne	r7,#0x00,00199$
;	main.c:427: SetupReq = 0xFF;
	mov	_SetupReq,#0xff
;	main.c:428: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
	mov	_UEP0_CTRL,#0xcf
	ljmp	00213$
00199$:
;	main.c:430: else if(len <= DEFAULT_ENDP0_SIZE)                                                       //上传数据或者状态阶段返回0长度包
	clr	c
	mov	a,#0x08
	subb	a,r6
	clr	a
	subb	a,r7
	jc	00196$
;	main.c:432: UEP0_T_LEN = len;
	mov	_UEP0_T_LEN,r6
;	main.c:433: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
	mov	_UEP0_CTRL,#0xc0
	ljmp	00213$
00196$:
;	main.c:437: UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
	mov	_UEP0_T_LEN,#0x00
;	main.c:438: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
	mov	_UEP0_CTRL,#0xc0
;	main.c:440: break;
	ljmp	00213$
;	main.c:441: case UIS_TOKEN_IN | 0:                                                      //endpoint0 IN
00201$:
;	main.c:442: switch(SetupReq)
	mov	a,#0x05
	cjne	a,_SetupReq,00587$
	sjmp	00203$
00587$:
	mov	a,#0x06
	cjne	a,_SetupReq,00204$
;	main.c:445: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;                                 //本次传输长度
	clr	c
	mov	a,_SetupLen
	subb	a,#0x08
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00232$
	mov	r6,#0x08
	mov	r7,#0x00
	sjmp	00233$
00232$:
	mov	r6,_SetupLen
	mov	r7,(_SetupLen + 1)
00233$:
;	main.c:446: memcpy( Ep0Buffer, pDescr, len );                                   //加载上传数据
	mov	___memcpy_PARM_2,_pDescr
	mov	(___memcpy_PARM_2 + 1),(_pDescr + 1)
	mov	(___memcpy_PARM_2 + 2),(_pDescr + 2)
	mov	___memcpy_PARM_3,r6
	mov	(___memcpy_PARM_3 + 1),r7
	mov	dptr,#_Ep0Buffer
	mov	b,#0x00
	push	ar7
	push	ar6
	lcall	___memcpy
	pop	ar6
	pop	ar7
;	main.c:447: SetupLen -= len;
	mov	a,_SetupLen
	clr	c
	subb	a,r6
	mov	_SetupLen,a
	mov	a,(_SetupLen + 1)
	subb	a,r7
	mov	(_SetupLen + 1),a
;	main.c:448: pDescr += len;
	mov	a,r6
	add	a,_pDescr
	mov	_pDescr,a
	mov	a,r7
	addc	a,(_pDescr + 1)
	mov	(_pDescr + 1),a
;	main.c:449: UEP0_T_LEN = len;
	mov	_UEP0_T_LEN,r6
;	main.c:450: UEP0_CTRL ^= bUEP_T_TOG;                                             //同步标志位翻转
	mov	r6,_UEP0_CTRL
	mov	r7,#0x00
	xrl	ar6,#0x40
	mov	_UEP0_CTRL,r6
;	main.c:451: break;
;	main.c:452: case USB_SET_ADDRESS:
	sjmp	00213$
00203$:
;	main.c:453: USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
	mov	a,_USB_DEV_AD
	anl	a,#0x80
	mov	r7,a
	mov	a,_SetupLen
	mov	r6,a
	orl	a,r7
	mov	_USB_DEV_AD,a
;	main.c:454: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:455: break;
;	main.c:456: default:
	sjmp	00213$
00204$:
;	main.c:457: UEP0_T_LEN = 0;                                                      //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
	mov	_UEP0_T_LEN,#0x00
;	main.c:458: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:461: break;
;	main.c:462: case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
	sjmp	00213$
00206$:
;	main.c:463: if(SetupReq ==SET_LINE_CODING)  //设置串口属性
	mov	a,#0x20
	cjne	a,_SetupReq,00210$
;	main.c:465: if( U_TOG_OK )
	jnb	_U_TOG_OK,00213$
;	main.c:467: memcpy(LineCoding,UsbSetupBuf,USB_RX_LEN);
	mov	___memcpy_PARM_2,#_Ep0Buffer
	mov	(___memcpy_PARM_2 + 1),#(_Ep0Buffer >> 8)
	mov	(___memcpy_PARM_2 + 2),#0x00
	mov	___memcpy_PARM_3,_USB_RX_LEN
	mov	(___memcpy_PARM_3 + 1),#0x00
	mov	dptr,#_LineCoding
	mov	b,#0x00
	lcall	___memcpy
;	main.c:468: Config_Uart1(LineCoding);
	mov	dptr,#_LineCoding
	mov	b,#0x00
	lcall	_Config_Uart1
;	main.c:469: UEP0_T_LEN = 0;
	mov	_UEP0_T_LEN,#0x00
;	main.c:470: UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;  // 准备上传0包
	mov	_UEP0_CTRL,_UEP0_CTRL
	sjmp	00213$
00210$:
;	main.c:475: UEP0_T_LEN = 0;
	mov	_UEP0_T_LEN,#0x00
;	main.c:476: UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_NAK;  //状态阶段，对IN响应NAK
	orl	_UEP0_CTRL,#0x02
;	main.c:484: }
00213$:
;	main.c:485: UIF_TRANSFER = 0;                                                           //写0清空中断
;	assignBit
	clr	_UIF_TRANSFER
00215$:
;	main.c:487: if(UIF_BUS_RST)                                                                 //设备模式USB总线复位中断
	jnb	_UIF_BUS_RST,00217$
;	main.c:492: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:493: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
	mov	_UEP1_CTRL,#0x12
;	main.c:494: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
	mov	_UEP2_CTRL,#0x12
;	main.c:495: USB_DEV_AD = 0x00;
	mov	_USB_DEV_AD,#0x00
;	main.c:496: UIF_SUSPEND = 0;
;	assignBit
	clr	_UIF_SUSPEND
;	main.c:497: UIF_TRANSFER = 0;
;	assignBit
	clr	_UIF_TRANSFER
;	main.c:498: UIF_BUS_RST = 0;                                                             //清中断标志
;	assignBit
	clr	_UIF_BUS_RST
;	main.c:499: Uart_Input_Point = 0;   //循环缓冲区输入指针
	mov	r0,#_Uart_Input_Point
	mov	@r0,#0x00
;	main.c:500: Uart_Output_Point = 0;  //循环缓冲区读出指针
	mov	r0,#_Uart_Output_Point
	mov	@r0,#0x00
;	main.c:501: UartByteCount = 0;      //当前缓冲区剩余待取字节数
	mov	r0,#_UartByteCount
	mov	@r0,#0x00
;	main.c:502: USBByteCount = 0;       //USB端点收到的长度
	mov	r0,#_USBByteCount
	mov	@r0,#0x00
;	main.c:503: UsbConfig = 0;          //清除配置值
	mov	_UsbConfig,#0x00
;	main.c:504: UpPoint2_Busy = 0;
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x00
00217$:
;	main.c:506: if (UIF_SUSPEND)                                                                 //USB总线挂起/唤醒完成
;	main.c:508: UIF_SUSPEND = 0;
;	assignBit
	jbc	_UIF_SUSPEND,00595$
	sjmp	00224$
00595$:
;	main.c:509: if ( USB_MIS_ST & bUMS_SUSPEND )                                             //挂起
	mov	a,_USB_MIS_ST
	jnb	acc.2,00226$
;	main.c:514: while ( XBUS_AUX & bUART0_TX )
00218$:
	mov	a,_XBUS_AUX
	jb	acc.7,00218$
;	main.c:518: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:519: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:520: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;                      //USB或者RXD0/1有信号时可被唤醒
	mov	_WAKE_CTRL,#0xc1
;	main.c:521: PCON |= PD;                                                                 //睡眠
	orl	_PCON,#0x02
;	main.c:522: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:523: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:524: WAKE_CTRL = 0x00;
	mov	_WAKE_CTRL,#0x00
	sjmp	00226$
00224$:
;	main.c:528: USB_INT_FG = 0xFF;                                                             //清中断标志
	mov	_USB_INT_FG,#0xff
00226$:
;	main.c:531: }
	pop	psw
	pop	(0+0)
	pop	(0+1)
	pop	(0+2)
	pop	(0+3)
	pop	(0+4)
	pop	(0+5)
	pop	(0+6)
	pop	(0+7)
	pop	dph
	pop	dpl
	pop	b
	pop	acc
	pop	bits
	reti
;------------------------------------------------------------
;Allocation info for local variables in function 'Uart1_ISR'
;------------------------------------------------------------
;	main.c:536: void Uart1_ISR(void) __interrupt (INT_NO_UART1)
;	-----------------------------------------
;	 function Uart1_ISR
;	-----------------------------------------
_Uart1_ISR:
	push	acc
	push	ar7
	push	ar0
	push	psw
	mov	psw,#0x00
;	main.c:538: if(U1RI)   //收到数据
	jnb	_U1RI,00105$
;	main.c:540: Receive_Uart_Buf[Uart_Input_Point++] = SBUF1;
	mov	r0,#_Uart_Input_Point
	mov	ar7,@r0
	mov	r0,#_Uart_Input_Point
	mov	a,r7
	inc	a
	mov	@r0,a
	mov	a,r7
	add	a,#_Receive_Uart_Buf
	mov	r0,a
	mov	@r0,_SBUF1
;	main.c:541: UartByteCount++;                    //当前缓冲区剩余待取字节数
	mov	r0,#_UartByteCount
	mov	a,@r0
	mov	r0,#_UartByteCount
	inc	a
	mov	@r0,a
;	main.c:542: if(Uart_Input_Point>=UART_REV_LEN)
	mov	r0,#_Uart_Input_Point
	cjne	@r0,#0x40,00116$
00116$:
	jc	00102$
;	main.c:543: Uart_Input_Point = 0;           //写入指针
	mov	r0,#_Uart_Input_Point
	mov	@r0,#0x00
00102$:
;	main.c:544: U1RI =0;
;	assignBit
	clr	_U1RI
00105$:
;	main.c:547: }
	pop	psw
	pop	ar0
	pop	ar7
	pop	acc
	reti
;	eliminated unneeded push/pop ar1
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;length                    Allocated to registers r6 
;Uart_Timeout              Allocated to registers r7 
;------------------------------------------------------------
;	main.c:549: main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c:552: uint8_t Uart_Timeout = 0;
	mov	r7,#0x00
;	main.c:553: CfgFsys( );                                                           //CH559时钟选择配置
	push	ar7
	lcall	_CfgFsys
;	main.c:554: mDelaymS(5);                                                          //修改主频等待内部晶振稳定,必加
	mov	dptr,#0x0005
	lcall	_mDelaymS
;	main.c:555: mInitSTDIO( );                                                        //串口0,可以用于调试
	lcall	_mInitSTDIO
;	main.c:556: UART1Setup( );                                                        //用于CDC
	lcall	_UART1Setup
;	main.c:561: USBDeviceCfg();
	lcall	_USBDeviceCfg
;	main.c:562: USBDeviceEndPointCfg();                                               //端点配置
	lcall	_USBDeviceEndPointCfg
;	main.c:563: USBDeviceIntCfg();                                                    //中断初始化
	lcall	_USBDeviceIntCfg
	pop	ar7
;	main.c:564: UEP0_T_LEN = 0;
	mov	_UEP0_T_LEN,#0x00
;	main.c:565: UEP1_T_LEN = 0;                                                       //预使用发送长度一定要清空
	mov	_UEP1_T_LEN,#0x00
;	main.c:566: UEP2_T_LEN = 0;                                                       //预使用发送长度一定要清空
	mov	_UEP2_T_LEN,#0x00
;	main.c:568: while(1)
00121$:
;	main.c:570: if(UsbConfig)
	mov	a,_UsbConfig
	jz	00121$
;	main.c:572: if(USBByteCount)   //USB接收端点有数据
	mov	r0,#_USBByteCount
	mov	a,@r0
	jz	00104$
;	main.c:574: CH554UART1SendByte(Ep2Buffer[USBBufOutPoint++]);
	mov	r0,#_USBBufOutPoint
	mov	ar6,@r0
	mov	r0,#_USBBufOutPoint
	mov	a,r6
	inc	a
	mov	@r0,a
	mov	a,r6
	add	a,#_Ep2Buffer
	mov	dpl,a
	clr	a
	addc	a,#(_Ep2Buffer >> 8)
	mov	dph,a
	movx	a,@dptr
	mov	dpl,a
	push	ar7
	lcall	_CH554UART1SendByte
	pop	ar7
;	main.c:575: USBByteCount--;
	mov	r0,#_USBByteCount
	mov	a,@r0
	mov	r0,#_USBByteCount
	dec	a
	mov	@r0,a
;	main.c:576: if(USBByteCount==0)
	mov	r0,#_USBByteCount
	mov	a,@r0
	jnz	00104$
;	main.c:577: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;
	anl	_UEP2_CTRL,#0xf3
00104$:
;	main.c:580: if(UartByteCount)
	mov	r0,#_UartByteCount
	mov	a,@r0
	jz	00106$
;	main.c:581: Uart_Timeout++;
	inc	r7
00106$:
;	main.c:582: if(!UpPoint2_Busy)   //端点不繁忙（空闲后的第一包数据，只用作触发上传）
	mov	r0,#_UpPoint2_Busy
	mov	a,@r0
	jnz	00121$
;	main.c:584: length = UartByteCount;
	mov	r0,#_UartByteCount
;	main.c:585: if(length>0)
	mov	a,@r0
	mov	r6,a
	jz	00121$
;	main.c:587: if(length>39 || Uart_Timeout>100)
	mov	a,r6
	add	a,#0xff - 0x27
	jc	00111$
	mov	a,r7
	add	a,#0xff - 0x64
	jnc	00121$
00111$:
;	main.c:589: Uart_Timeout = 0;
	mov	r7,#0x00
;	main.c:590: if(Uart_Output_Point+length>UART_REV_LEN)
	mov	r0,#_Uart_Output_Point
	mov	ar4,@r0
	mov	r5,#0x00
	mov	ar2,r6
	mov	r3,#0x00
	mov	a,r2
	add	a,r4
	mov	r4,a
	mov	a,r3
	addc	a,r5
	mov	r5,a
	clr	c
	mov	a,#0x40
	subb	a,r4
	mov	a,#(0x00 ^ 0x80)
	mov	b,r5
	xrl	b,#0x80
	subb	a,b
	jnc	00108$
;	main.c:591: length = UART_REV_LEN-Uart_Output_Point;
	mov	r0,#_Uart_Output_Point
	mov	ar5,@r0
	mov	a,#0x40
	clr	c
	subb	a,r5
	mov	r6,a
00108$:
;	main.c:592: UartByteCount -= length;
	mov	r0,#_UartByteCount
	mov	a,@r0
	clr	c
	subb	a,r6
	mov	@r0,a
;	main.c:594: memcpy(Ep2Buffer+MAX_PACKET_SIZE,&Receive_Uart_Buf[Uart_Output_Point],length);
	mov	r0,#_Uart_Output_Point
	mov	a,@r0
	add	a,#_Receive_Uart_Buf
	mov	r5,a
	mov	___memcpy_PARM_2,r5
	mov	(___memcpy_PARM_2 + 1),#0x00
	mov	(___memcpy_PARM_2 + 2),#0x40
	mov	___memcpy_PARM_3,r6
	mov	(___memcpy_PARM_3 + 1),#0x00
	mov	dptr,#(_Ep2Buffer + 0x0040)
	mov	b,#0x00
	push	ar7
	push	ar6
	lcall	___memcpy
	pop	ar6
	pop	ar7
;	main.c:595: Uart_Output_Point+=length;
	mov	r0,#_Uart_Output_Point
	mov	a,r6
	add	a,@r0
	mov	@r0,a
;	main.c:596: if(Uart_Output_Point>=UART_REV_LEN)
	mov	r0,#_Uart_Output_Point
	cjne	@r0,#0x40,00174$
00174$:
	jc	00110$
;	main.c:597: Uart_Output_Point = 0;
	mov	r0,#_Uart_Output_Point
	mov	@r0,#0x00
00110$:
;	main.c:598: UEP2_T_LEN = length;                                                    //预使用发送长度一定要清空
	mov	_UEP2_T_LEN,r6
;	main.c:599: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;            //应答ACK
	anl	_UEP2_CTRL,#0xfc
;	main.c:600: UpPoint2_Busy = 1;
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x01
;	main.c:606: }
	ljmp	00121$
	.area CSEG    (CODE)
	.area CONST   (CODE)
_DevDesc:
	.db #0x12	; 18
	.db #0x01	; 1
	.db #0x10	; 16
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x08	; 8
	.db #0x86	; 134
	.db #0x1a	; 26
	.db #0x22	; 34
	.db #0x57	; 87	'W'
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x03	; 3
	.db #0x01	; 1
_CfgDesc:
	.db #0x09	; 9
	.db #0x02	; 2
	.db #0x43	; 67	'C'
	.db #0x00	; 0
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0xa0	; 160
	.db #0x32	; 50	'2'
	.db #0x09	; 9
	.db #0x04	; 4
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x05	; 5
	.db #0x24	; 36
	.db #0x00	; 0
	.db #0x10	; 16
	.db #0x01	; 1
	.db #0x05	; 5
	.db #0x24	; 36
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x04	; 4
	.db #0x24	; 36
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x05	; 5
	.db #0x24	; 36
	.db #0x06	; 6
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x07	; 7
	.db #0x05	; 5
	.db #0x81	; 129
	.db #0x03	; 3
	.db #0x08	; 8
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x09	; 9
	.db #0x04	; 4
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x02	; 2
	.db #0x0a	; 10
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x07	; 7
	.db #0x05	; 5
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x40	; 64
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x07	; 7
	.db #0x05	; 5
	.db #0x82	; 130
	.db #0x02	; 2
	.db #0x40	; 64
	.db #0x00	; 0
	.db #0x00	; 0
_LangDes:
	.db #0x04	; 4
	.db #0x03	; 3
	.db #0x09	; 9
	.db #0x04	; 4
_SerDes:
	.db #0x14	; 20
	.db #0x03	; 3
	.db #0x32	; 50	'2'
	.db #0x00	; 0
	.db #0x30	; 48	'0'
	.db #0x00	; 0
	.db #0x31	; 49	'1'
	.db #0x00	; 0
	.db #0x37	; 55	'7'
	.db #0x00	; 0
	.db #0x2d	; 45
	.db #0x00	; 0
	.db #0x32	; 50	'2'
	.db #0x00	; 0
	.db #0x2d	; 45
	.db #0x00	; 0
	.db #0x32	; 50	'2'
	.db #0x00	; 0
	.db #0x35	; 53	'5'
	.db #0x00	; 0
_Prod_Des:
	.db #0x14	; 20
	.db #0x03	; 3
	.db #0x43	; 67	'C'
	.db #0x00	; 0
	.db #0x48	; 72	'H'
	.db #0x00	; 0
	.db #0x35	; 53	'5'
	.db #0x00	; 0
	.db #0x35	; 53	'5'
	.db #0x00	; 0
	.db #0x34	; 52	'4'
	.db #0x00	; 0
	.db #0x5f	; 95
	.db #0x00	; 0
	.db #0x43	; 67	'C'
	.db #0x00	; 0
	.db #0x44	; 68	'D'
	.db #0x00	; 0
	.db #0x43	; 67	'C'
	.db #0x00	; 0
_Manuf_Des:
	.db #0x0a	; 10
	.db #0x03	; 3
	.db #0x5f	; 95
	.db #0x6c	; 108	'l'
	.db #0xcf	; 207
	.db #0x82	; 130
	.db #0x81	; 129
	.db #0x6c	; 108	'l'
	.db #0x52	; 82	'R'
	.db #0x60	; 96
	.area XINIT   (CODE)
__xinit__LineCoding:
	.db #0x00	; 0
	.db #0xe1	; 225
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x08	; 8
	.area CABS    (ABS,CODE)
