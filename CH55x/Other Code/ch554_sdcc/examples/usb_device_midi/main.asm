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
	.globl _DeviceInterrupt
	.globl _USBDeviceEndPointCfg
	.globl _USBDeviceIntCfg
	.globl _USBDeviceCfg
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
	.globl _Ep2Buffer
	.globl _Ep1Buffer
	.globl _Ep0Buffer
	.globl _UpPoint2_Busy
	.globl _USBBufOutPoint
	.globl _USBByteCount
	.globl _MidiByteCount
	.globl _Midi_Output_Point
	.globl _Midi_Input_Point
	.globl _Receive_Midi_Buf
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
_Receive_Midi_Buf::
	.ds 64
_Midi_Input_Point::
	.ds 1
_Midi_Output_Point::
	.ds 1
_MidiByteCount::
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
;	main.c:83: volatile __idata uint8_t Midi_Input_Point = 0;   //循环缓冲区写入指针，总线复位需要初始化为0
	mov	r0,#_Midi_Input_Point
	mov	@r0,#0x00
;	main.c:84: volatile __idata uint8_t Midi_Output_Point = 0;  //循环缓冲区取出指针，总线复位需要初始化为0
	mov	r0,#_Midi_Output_Point
	mov	@r0,#0x00
;	main.c:85: volatile __idata uint8_t MidiByteCount = 0;	  //当前缓冲区剩余待取字节数
	mov	r0,#_MidiByteCount
	mov	@r0,#0x00
;	main.c:88: volatile __idata uint8_t USBByteCount = 0;	  //代表USB端点接收到的数据
	mov	r0,#_USBByteCount
	mov	@r0,#0x00
;	main.c:89: volatile __idata uint8_t USBBufOutPoint = 0;	//取数据指针
	mov	r0,#_USBBufOutPoint
	mov	@r0,#0x00
;	main.c:91: volatile __idata uint8_t UpPoint2_Busy  = 0;   //上传端点是否忙标志
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
;	main.c:101: void USBDeviceCfg()
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
;	main.c:103: USB_CTRL = 0x00;														   //清空USB控制寄存器
	mov	_USB_CTRL,#0x00
;	main.c:104: USB_CTRL &= ~bUC_HOST_MODE;												//该位为选择设备模式
	anl	_USB_CTRL,#0x7f
;	main.c:105: USB_CTRL |=  bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;					//USB设备和内部上拉使能,在中断期间中断标志未清除前自动返回NAK
	orl	_USB_CTRL,#0x29
;	main.c:106: USB_DEV_AD = 0x00;														 //设备地址初始化
	mov	_USB_DEV_AD,#0x00
;	main.c:109: USB_CTRL &= ~bUC_LOW_SPEED;
	anl	_USB_CTRL,#0xbf
;	main.c:110: UDEV_CTRL &= ~bUD_LOW_SPEED;											 //选择全速12M模式，默认方式
	anl	_UDEV_CTRL,#0xfb
;	main.c:111: UDEV_CTRL = bUD_PD_DIS;  // 禁止DP/DM下拉电阻
	mov	_UDEV_CTRL,#0x80
;	main.c:112: UDEV_CTRL |= bUD_PORT_EN;												  //使能物理端口
	orl	_UDEV_CTRL,#0x01
;	main.c:113: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'USBDeviceIntCfg'
;------------------------------------------------------------
;	main.c:121: void USBDeviceIntCfg()
;	-----------------------------------------
;	 function USBDeviceIntCfg
;	-----------------------------------------
_USBDeviceIntCfg:
;	main.c:123: USB_INT_EN |= bUIE_SUSPEND;											   //使能设备挂起中断
	orl	_USB_INT_EN,#0x04
;	main.c:124: USB_INT_EN |= bUIE_TRANSFER;											  //使能USB传输完成中断
	orl	_USB_INT_EN,#0x02
;	main.c:125: USB_INT_EN |= bUIE_BUS_RST;											   //使能设备模式USB总线复位中断
	orl	_USB_INT_EN,#0x01
;	main.c:126: USB_INT_FG |= 0x1F;													   //清中断标志
	orl	_USB_INT_FG,#0x1f
;	main.c:127: IE_USB = 1;															   //使能USB中断
;	assignBit
	setb	_IE_USB
;	main.c:128: EA = 1;																   //允许单片机中断
;	assignBit
	setb	_EA
;	main.c:129: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'USBDeviceEndPointCfg'
;------------------------------------------------------------
;	main.c:137: void USBDeviceEndPointCfg()
;	-----------------------------------------
;	 function USBDeviceEndPointCfg
;	-----------------------------------------
_USBDeviceEndPointCfg:
;	main.c:139: UEP1_DMA = (uint16_t) Ep1Buffer;													  //端点1 发送数据传输地址
	mov	((_UEP1_DMA >> 0) & 0xFF),#_Ep1Buffer
	mov	((_UEP1_DMA >> 8) & 0xFF),#(_Ep1Buffer >> 8)
;	main.c:140: UEP2_DMA = (uint16_t) Ep2Buffer;													  //端点2 IN数据传输地址
	mov	((_UEP2_DMA >> 0) & 0xFF),#_Ep2Buffer
	mov	((_UEP2_DMA >> 8) & 0xFF),#(_Ep2Buffer >> 8)
;	main.c:141: UEP2_3_MOD = 0xCC;														 //端点2/3 单缓冲收发使能
	mov	_UEP2_3_MOD,#0xcc
;	main.c:142: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;		//端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
	mov	_UEP2_CTRL,#0x12
;	main.c:144: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;				//端点1自动翻转同步标志位，IN事务返回NAK
	mov	_UEP1_CTRL,#0x12
;	main.c:145: UEP0_DMA = (uint16_t) Ep0Buffer;													  //端点0数据传输地址
	mov	((_UEP0_DMA >> 0) & 0xFF),#_Ep0Buffer
	mov	((_UEP0_DMA >> 8) & 0xFF),#(_Ep0Buffer >> 8)
;	main.c:146: UEP4_1_MOD = 0X40;														 //端点1上传缓冲区；端点0单64字节收发缓冲区
	mov	_UEP4_1_MOD,#0x40
;	main.c:147: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;				//手动翻转，OUT事务返回ACK，IN事务返回NAK
	mov	_UEP0_CTRL,#0x02
;	main.c:148: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'DeviceInterrupt'
;------------------------------------------------------------
;len                       Allocated to registers r4 r5 
;------------------------------------------------------------
;	main.c:172: void DeviceInterrupt(void) __interrupt (INT_NO_USB)					   //USB中断服务程序,使用寄存器组1
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
;	main.c:175: if(UIF_TRANSFER)															//USB传输完成标志
	jb	_UIF_TRANSFER,00484$
	ljmp	00207$
00484$:
;	main.c:177: switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
	mov	a,#0x3f
	anl	a,_USB_INT_ST
	mov	r7,a
	mov	r6,a
	cjne	r6,#0x00,00485$
	ljmp	00203$
00485$:
	cjne	r7,#0x02,00486$
	sjmp	00103$
00486$:
	cjne	r7,#0x20,00487$
	ljmp	00198$
00487$:
	cjne	r7,#0x21,00488$
	sjmp	00101$
00488$:
	cjne	r7,#0x22,00489$
	sjmp	00102$
00489$:
	cjne	r7,#0x30,00490$
	sjmp	00106$
00490$:
	ljmp	00205$
;	main.c:179: case UIS_TOKEN_IN | 1:												  //endpoint 1# 端点中断上传
00101$:
;	main.c:180: UEP1_T_LEN = 0;
	mov	_UEP1_T_LEN,#0x00
;	main.c:181: UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;		   //默认应答NAK
	mov	a,#0xfc
	anl	a,_UEP1_CTRL
	orl	a,#0x02
	mov	_UEP1_CTRL,a
;	main.c:182: break;
	ljmp	00205$
;	main.c:183: case UIS_TOKEN_IN | 2:												  //endpoint 2# 端点批量上传
00102$:
;	main.c:185: UEP2_T_LEN = 0;													//预使用发送长度一定要清空
	mov	_UEP2_T_LEN,#0x00
;	main.c:186: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;		   //默认应答NAK
	mov	a,#0xfc
	anl	a,_UEP2_CTRL
	orl	a,#0x02
	mov	_UEP2_CTRL,a
;	main.c:187: UpPoint2_Busy = 0;												  //清除忙标志
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x00
;	main.c:189: break;
	ljmp	00205$
;	main.c:190: case UIS_TOKEN_OUT | 2:											 //endpoint 3# 端点批量下传
00103$:
;	main.c:191: if ( U_TOG_OK )													 // 不同步的数据包将丢弃
	jb	_U_TOG_OK,00491$
	ljmp	00205$
00491$:
;	main.c:193: USBByteCount = USB_RX_LEN;
	mov	r0,#_USBByteCount
	mov	@r0,_USB_RX_LEN
;	main.c:194: USBBufOutPoint = 0;											 //取数据指针复位
	mov	r0,#_USBBufOutPoint
	mov	@r0,#0x00
;	main.c:195: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_NAK;	   //收到一包数据就NAK，主函数处理完，由主函数修改响应方式
	mov	a,#0xf3
	anl	a,_UEP2_CTRL
	orl	a,#0x08
	mov	_UEP2_CTRL,a
;	main.c:197: break;
	ljmp	00205$
;	main.c:198: case UIS_TOKEN_SETUP | 0:												//SETUP事务
00106$:
;	main.c:199: len = USB_RX_LEN;
	mov	r6,_USB_RX_LEN
	mov	r7,#0x00
;	main.c:200: if(len == (sizeof(USB_SETUP_REQ)))
	cjne	r6,#0x08,00492$
	cjne	r7,#0x00,00492$
	sjmp	00493$
00492$:
	ljmp	00190$
00493$:
;	main.c:202: SetupLen = ((uint16_t)UsbSetupBuf->wLengthH<<8) | (UsbSetupBuf->wLengthL);
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
;	main.c:203: len = 0;													  // 默认为成功并且上传0长度
	mov	r6,#0x00
	mov	r7,#0x00
;	main.c:204: SetupReq = UsbSetupBuf->bRequest;
	mov	dptr,#(_Ep0Buffer + 0x0001)
	movx	a,@dptr
	mov	_SetupReq,a
;	main.c:205: if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )//非标准请求
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	a,#0x60
	jz	00187$
;	main.c:211: len = 0xFF;  								 									 /*命令不支持*/
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:213: }
	ljmp	00191$
00187$:
;	main.c:217: switch(SetupReq)											 //请求码
	mov	a,_SetupReq
	add	a,#0xff - 0x0a
	jnc	00495$
	ljmp	00184$
00495$:
	mov	a,_SetupReq
	add	a,#(00496$-3-.)
	movc	a,@a+pc
	mov	dpl,a
	mov	a,_SetupReq
	add	a,#(00497$-3-.)
	movc	a,@a+pc
	mov	dph,a
	clr	a
	jmp	@a+dptr
00496$:
	.db	00180$
	.db	00132$
	.db	00184$
	.db	00153$
	.db	00184$
	.db	00126$
	.db	00109$
	.db	00184$
	.db	00127$
	.db	00130$
	.db	00191$
00497$:
	.db	00180$>>8
	.db	00132$>>8
	.db	00184$>>8
	.db	00153$>>8
	.db	00184$>>8
	.db	00126$>>8
	.db	00109$>>8
	.db	00184$>>8
	.db	00127$>>8
	.db	00130$>>8
	.db	00191$>>8
;	main.c:219: case USB_GET_DESCRIPTOR:
00109$:
;	main.c:220: switch(UsbSetupBuf->wValueH)
	mov	dptr,#(_Ep0Buffer + 0x0003)
	movx	a,@dptr
	mov	r5,a
	cjne	r5,#0x01,00498$
	sjmp	00110$
00498$:
	cjne	r5,#0x02,00499$
	sjmp	00111$
00499$:
;	main.c:222: case 1:													   //设备描述符
	cjne	r5,#0x03,00122$
	sjmp	00112$
00110$:
;	main.c:223: pDescr = DevDesc;										 //把设备描述符送到要发送的缓冲区
	mov	_pDescr,#_DevDesc
	mov	(_pDescr + 1),#(_DevDesc >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:224: len = sizeof(DevDesc);
	mov	r4,#0x12
	mov	r5,#0x00
;	main.c:225: break;
;	main.c:226: case 2:														//配置描述符
	sjmp	00123$
00111$:
;	main.c:227: pDescr = CfgDesc;										  //把设备描述符送到要发送的缓冲区
	mov	_pDescr,#_CfgDesc
	mov	(_pDescr + 1),#(_CfgDesc >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:228: len = sizeof(CfgDesc);
	mov	r4,#0x61
	mov	r5,#0x00
;	main.c:229: break;
;	main.c:230: case 3:
	sjmp	00123$
00112$:
;	main.c:231: if(UsbSetupBuf->wValueL == 0)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	jnz	00120$
;	main.c:233: pDescr = LangDes;
	mov	_pDescr,#_LangDes
	mov	(_pDescr + 1),#(_LangDes >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:234: len = sizeof(LangDes);
	mov	r4,#0x04
	mov	r5,#0x00
	sjmp	00123$
00120$:
;	main.c:236: else if(UsbSetupBuf->wValueL == 1)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	cjne	r3,#0x01,00117$
;	main.c:238: pDescr = Manuf_Des;
	mov	_pDescr,#_Manuf_Des
	mov	(_pDescr + 1),#(_Manuf_Des >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:239: len = sizeof(Manuf_Des);
	mov	r4,#0x0a
	mov	r5,#0x00
	sjmp	00123$
00117$:
;	main.c:241: else if(UsbSetupBuf->wValueL == 2)
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r3,a
	cjne	r3,#0x02,00114$
;	main.c:243: pDescr = Prod_Des;
	mov	_pDescr,#_Prod_Des
	mov	(_pDescr + 1),#(_Prod_Des >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:244: len = sizeof(Prod_Des);
	mov	r4,#0x14
	mov	r5,#0x00
	sjmp	00123$
00114$:
;	main.c:248: pDescr = SerDes;
	mov	_pDescr,#_SerDes
	mov	(_pDescr + 1),#(_SerDes >> 8)
	mov	(_pDescr + 2),#0x80
;	main.c:249: len = sizeof(SerDes);
	mov	r4,#0x14
	mov	r5,#0x00
;	main.c:251: break;
;	main.c:252: default:
	sjmp	00123$
00122$:
;	main.c:253: len = 0xff;												//不支持的命令或者出错
	mov	r4,#0xff
	mov	r5,#0x00
;	main.c:255: }
00123$:
;	main.c:256: if ( SetupLen > len )
	clr	c
	mov	a,r4
	subb	a,_SetupLen
	mov	a,r5
	subb	a,(_SetupLen + 1)
	jnc	00125$
;	main.c:258: SetupLen = len;	//限制总长度
	mov	_SetupLen,r4
	mov	(_SetupLen + 1),r5
00125$:
;	main.c:260: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;							//本次传输长度
	clr	c
	mov	a,_SetupLen
	subb	a,#0x08
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00220$
	mov	r4,#0x08
	mov	r5,#0x00
	sjmp	00221$
00220$:
	mov	r4,_SetupLen
	mov	r5,(_SetupLen + 1)
00221$:
	mov	ar6,r4
	mov	ar7,r5
;	main.c:261: memcpy(Ep0Buffer,pDescr,len);								  //加载上传数据
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
;	main.c:262: SetupLen -= len;
	mov	a,_SetupLen
	clr	c
	subb	a,r6
	mov	_SetupLen,a
	mov	a,(_SetupLen + 1)
	subb	a,r7
	mov	(_SetupLen + 1),a
;	main.c:263: pDescr += len;
	mov	a,r6
	add	a,_pDescr
	mov	_pDescr,a
	mov	a,r7
	addc	a,(_pDescr + 1)
	mov	(_pDescr + 1),a
;	main.c:264: break;
	ljmp	00191$
;	main.c:265: case USB_SET_ADDRESS:
00126$:
;	main.c:266: SetupLen = UsbSetupBuf->wValueL;							  //暂存USB设备地址
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	r5,a
	mov	_SetupLen,r5
	mov	(_SetupLen + 1),#0x00
;	main.c:267: break;
	ljmp	00191$
;	main.c:268: case USB_GET_CONFIGURATION:
00127$:
;	main.c:269: Ep0Buffer[0] = UsbConfig;
	mov	dptr,#_Ep0Buffer
	mov	a,_UsbConfig
	movx	@dptr,a
;	main.c:270: if ( SetupLen >= 1 )
	clr	c
	mov	a,_SetupLen
	subb	a,#0x01
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jnc	00508$
	ljmp	00191$
00508$:
;	main.c:272: len = 1;
	mov	r6,#0x01
	mov	r7,#0x00
;	main.c:274: break;
	ljmp	00191$
;	main.c:275: case USB_SET_CONFIGURATION:
00130$:
;	main.c:276: UsbConfig = UsbSetupBuf->wValueL;
	mov	dptr,#(_Ep0Buffer + 0x0002)
	movx	a,@dptr
	mov	_UsbConfig,a
;	main.c:277: break;
	ljmp	00191$
;	main.c:280: case USB_CLEAR_FEATURE:											//Clear Feature
00132$:
;	main.c:281: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )				  /* 清除设备 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	anl	a,#0x1f
	jnz	00151$
;	main.c:283: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
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
	cjne	r5,#0x01,00137$
	cjne	r4,#0x00,00137$
;	main.c:285: if( CfgDesc[ 7 ] & 0x20 )
	mov	dptr,#(_CfgDesc + 0x0007)
	clr	a
	movc	a,@a+dptr
	mov	r5,a
	jnb	acc.5,00513$
	ljmp	00191$
00513$:
;	main.c:291: len = 0xFF;										/* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00191$
00137$:
;	main.c:296: len = 0xFF;											/* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00191$
00151$:
;	main.c:299: else if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	ar5,#0x1f
	mov	r4,#0x00
	cjne	r5,#0x02,00148$
	cjne	r4,#0x00,00148$
;	main.c:301: switch( UsbSetupBuf->wIndexL )
	mov	dptr,#(_Ep0Buffer + 0x0004)
	movx	a,@dptr
	mov	r5,a
	cjne	r5,#0x01,00516$
	sjmp	00144$
00516$:
	cjne	r5,#0x02,00517$
	sjmp	00142$
00517$:
	cjne	r5,#0x03,00518$
	sjmp	00140$
00518$:
	cjne	r5,#0x81,00519$
	sjmp	00143$
00519$:
	cjne	r5,#0x82,00520$
	sjmp	00141$
00520$:
	cjne	r5,#0x83,00145$
;	main.c:304: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP3_CTRL
	orl	a,#0x02
	mov	_UEP3_CTRL,a
;	main.c:305: break;
	ljmp	00191$
;	main.c:306: case 0x03:
00140$:
;	main.c:307: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP3_CTRL,#0x73
;	main.c:308: break;
	ljmp	00191$
;	main.c:309: case 0x82:
00141$:
;	main.c:310: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP2_CTRL
	orl	a,#0x02
	mov	_UEP2_CTRL,a
;	main.c:311: break;
	ljmp	00191$
;	main.c:312: case 0x02:
00142$:
;	main.c:313: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP2_CTRL,#0x73
;	main.c:314: break;
	ljmp	00191$
;	main.c:315: case 0x81:
00143$:
;	main.c:316: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
	mov	a,#0xbc
	anl	a,_UEP1_CTRL
	orl	a,#0x02
	mov	_UEP1_CTRL,a
;	main.c:317: break;
	ljmp	00191$
;	main.c:318: case 0x01:
00144$:
;	main.c:319: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
	anl	_UEP1_CTRL,#0x73
;	main.c:320: break;
	ljmp	00191$
;	main.c:321: default:
00145$:
;	main.c:322: len = 0xFF;										 // 不支持的端点
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:324: }
	ljmp	00191$
00148$:
;	main.c:328: len = 0xFF;												// 不是端点不支持
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:330: break;
	ljmp	00191$
;	main.c:331: case USB_SET_FEATURE:										  /* Set Feature */
00153$:
;	main.c:332: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )				  /* 设置设备 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	anl	a,#0x1f
	jnz	00178$
;	main.c:334: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
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
	cjne	r5,#0x01,00161$
	cjne	r4,#0x00,00161$
;	main.c:336: if( CfgDesc[ 7 ] & 0x20 )
	mov	dptr,#(_CfgDesc + 0x0007)
	clr	a
	movc	a,@a+dptr
	mov	r5,a
	jnb	acc.5,00158$
;	main.c:342: while ( XBUS_AUX & bUART0_TX )
00154$:
	mov	a,_XBUS_AUX
	jb	acc.7,00154$
;	main.c:346: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:347: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:348: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;					  //USB或者RXD0/1有信号时可被唤醒
	mov	_WAKE_CTRL,#0xc1
;	main.c:349: PCON |= PD;																 //睡眠
	orl	_PCON,#0x02
;	main.c:350: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:351: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:352: WAKE_CTRL = 0x00;
	mov	_WAKE_CTRL,#0x00
	ljmp	00191$
00158$:
;	main.c:356: len = 0xFF;										/* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00191$
00161$:
;	main.c:361: len = 0xFF;											/* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	ljmp	00191$
00178$:
;	main.c:364: else if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_ENDP )			 /* 设置端点 */
	mov	dptr,#_Ep0Buffer
	movx	a,@dptr
	mov	r5,a
	anl	ar5,#0x1f
	mov	r4,#0x00
	cjne	r5,#0x02,00529$
	cjne	r4,#0x00,00529$
	sjmp	00530$
00529$:
	ljmp	00175$
00530$:
;	main.c:366: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
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
	jz	00531$
	ljmp	00172$
00531$:
;	main.c:368: switch( ( ( uint16_t )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
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
	cjne	r5,#0x01,00532$
	cjne	r4,#0x00,00532$
	sjmp	00168$
00532$:
	cjne	r5,#0x02,00533$
	cjne	r4,#0x00,00533$
	sjmp	00166$
00533$:
	cjne	r5,#0x03,00534$
	cjne	r4,#0x00,00534$
	sjmp	00164$
00534$:
	cjne	r5,#0x81,00535$
	cjne	r4,#0x00,00535$
	sjmp	00167$
00535$:
	cjne	r5,#0x82,00536$
	cjne	r4,#0x00,00536$
	sjmp	00165$
00536$:
	cjne	r5,#0x83,00169$
	cjne	r4,#0x00,00169$
;	main.c:371: UEP3_CTRL = UEP3_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点3 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP3_CTRL
	orl	a,#0x03
	mov	_UEP3_CTRL,a
;	main.c:372: break;
;	main.c:373: case 0x03:
	sjmp	00191$
00164$:
;	main.c:374: UEP3_CTRL = UEP3_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点3 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP3_CTRL
	orl	a,#0x0c
	mov	_UEP3_CTRL,a
;	main.c:375: break;
;	main.c:376: case 0x82:
	sjmp	00191$
00165$:
;	main.c:377: UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP2_CTRL
	orl	a,#0x03
	mov	_UEP2_CTRL,a
;	main.c:378: break;
;	main.c:379: case 0x02:
	sjmp	00191$
00166$:
;	main.c:380: UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP2_CTRL
	orl	a,#0x0c
	mov	_UEP2_CTRL,a
;	main.c:381: break;
;	main.c:382: case 0x81:
	sjmp	00191$
00167$:
;	main.c:383: UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
	mov	a,#0xbf
	anl	a,_UEP1_CTRL
	orl	a,#0x03
	mov	_UEP1_CTRL,a
;	main.c:384: break;
;	main.c:385: case 0x01:
	sjmp	00191$
00168$:
;	main.c:386: UEP1_CTRL = UEP1_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点1 OUT Stall */
	mov	a,#0x7f
	anl	a,_UEP1_CTRL
	orl	a,#0x0c
	mov	_UEP1_CTRL,a
;	main.c:387: default:
00169$:
;	main.c:388: len = 0xFF;									/* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:390: }
	sjmp	00191$
00172$:
;	main.c:394: len = 0xFF;									  /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
	sjmp	00191$
00175$:
;	main.c:399: len = 0xFF;										  /* 操作失败 */
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:401: break;
;	main.c:402: case USB_GET_STATUS:
	sjmp	00191$
00180$:
;	main.c:403: Ep0Buffer[0] = 0x00;
	mov	dptr,#_Ep0Buffer
	clr	a
	movx	@dptr,a
;	main.c:404: Ep0Buffer[1] = 0x00;
	mov	dptr,#(_Ep0Buffer + 0x0001)
	movx	@dptr,a
;	main.c:405: if ( SetupLen >= 2 )
	clr	c
	mov	a,_SetupLen
	subb	a,#0x02
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00182$
;	main.c:407: len = 2;
	mov	r6,#0x02
	mov	r7,#0x00
	sjmp	00191$
00182$:
;	main.c:411: len = SetupLen;
	mov	r6,_SetupLen
	mov	r7,(_SetupLen + 1)
;	main.c:413: break;
;	main.c:414: default:
	sjmp	00191$
00184$:
;	main.c:415: len = 0xff;													//操作失败
	mov	r6,#0xff
	mov	r7,#0x00
;	main.c:417: }
	sjmp	00191$
00190$:
;	main.c:422: len = 0xff;														 //包长度错误
	mov	r6,#0xff
	mov	r7,#0x00
00191$:
;	main.c:424: if(len == 0xff)
	cjne	r6,#0xff,00196$
	cjne	r7,#0x00,00196$
;	main.c:426: SetupReq = 0xFF;
	mov	_SetupReq,#0xff
;	main.c:427: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
	mov	_UEP0_CTRL,#0xcf
	ljmp	00205$
00196$:
;	main.c:429: else if(len <= DEFAULT_ENDP0_SIZE)													   //上传数据或者状态阶段返回0长度包
	clr	c
	mov	a,#0x08
	subb	a,r6
	clr	a
	subb	a,r7
	jc	00193$
;	main.c:431: UEP0_T_LEN = len;
	mov	_UEP0_T_LEN,r6
;	main.c:432: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
	mov	_UEP0_CTRL,#0xc0
	ljmp	00205$
00193$:
;	main.c:436: UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
	mov	_UEP0_T_LEN,#0x00
;	main.c:437: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
	mov	_UEP0_CTRL,#0xc0
;	main.c:439: break;
	ljmp	00205$
;	main.c:440: case UIS_TOKEN_IN | 0:													  //endpoint0 IN
00198$:
;	main.c:441: switch(SetupReq)
	mov	a,#0x05
	cjne	a,_SetupReq,00543$
	sjmp	00200$
00543$:
	mov	a,#0x06
	cjne	a,_SetupReq,00201$
;	main.c:444: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;								 //本次传输长度
	clr	c
	mov	a,_SetupLen
	subb	a,#0x08
	mov	a,(_SetupLen + 1)
	subb	a,#0x00
	jc	00222$
	mov	r6,#0x08
	mov	r7,#0x00
	sjmp	00223$
00222$:
	mov	r6,_SetupLen
	mov	r7,(_SetupLen + 1)
00223$:
;	main.c:445: memcpy( Ep0Buffer, pDescr, len );								   //加载上传数据
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
;	main.c:446: SetupLen -= len;
	mov	a,_SetupLen
	clr	c
	subb	a,r6
	mov	_SetupLen,a
	mov	a,(_SetupLen + 1)
	subb	a,r7
	mov	(_SetupLen + 1),a
;	main.c:447: pDescr += len;
	mov	a,r6
	add	a,_pDescr
	mov	_pDescr,a
	mov	a,r7
	addc	a,(_pDescr + 1)
	mov	(_pDescr + 1),a
;	main.c:448: UEP0_T_LEN = len;
	mov	_UEP0_T_LEN,r6
;	main.c:449: UEP0_CTRL ^= bUEP_T_TOG;											 //同步标志位翻转
	mov	r6,_UEP0_CTRL
	mov	r7,#0x00
	xrl	ar6,#0x40
	mov	_UEP0_CTRL,r6
;	main.c:450: break;
;	main.c:451: case USB_SET_ADDRESS:
	sjmp	00205$
00200$:
;	main.c:452: USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
	mov	a,_USB_DEV_AD
	anl	a,#0x80
	mov	r7,a
	mov	a,_SetupLen
	mov	r6,a
	orl	a,r7
	mov	_USB_DEV_AD,a
;	main.c:453: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:454: break;
;	main.c:455: default:
	sjmp	00205$
00201$:
;	main.c:456: UEP0_T_LEN = 0;													  //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
	mov	_UEP0_T_LEN,#0x00
;	main.c:457: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:460: break;
;	main.c:461: case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
	sjmp	00205$
00203$:
;	main.c:474: UEP0_T_LEN = 0;
	mov	_UEP0_T_LEN,#0x00
;	main.c:475: UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;  //状态阶段，对IN响应NAK
	mov	_UEP0_CTRL,_UEP0_CTRL
;	main.c:483: }
00205$:
;	main.c:484: UIF_TRANSFER = 0;														   //写0清空中断
;	assignBit
	clr	_UIF_TRANSFER
00207$:
;	main.c:486: if(UIF_BUS_RST)																 //设备模式USB总线复位中断
	jnb	_UIF_BUS_RST,00209$
;	main.c:491: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
	mov	_UEP0_CTRL,#0x02
;	main.c:492: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
	mov	_UEP1_CTRL,#0x12
;	main.c:493: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
	mov	_UEP2_CTRL,#0x12
;	main.c:494: USB_DEV_AD = 0x00;
	mov	_USB_DEV_AD,#0x00
;	main.c:495: UIF_SUSPEND = 0;
;	assignBit
	clr	_UIF_SUSPEND
;	main.c:496: UIF_TRANSFER = 0;
;	assignBit
	clr	_UIF_TRANSFER
;	main.c:497: UIF_BUS_RST = 0;															 //清中断标志
;	assignBit
	clr	_UIF_BUS_RST
;	main.c:498: Midi_Input_Point = 0;   //循环缓冲区输入指针
	mov	r0,#_Midi_Input_Point
	mov	@r0,#0x00
;	main.c:499: Midi_Output_Point = 0;  //循环缓冲区读出指针
	mov	r0,#_Midi_Output_Point
	mov	@r0,#0x00
;	main.c:500: MidiByteCount = 0;	  //当前缓冲区剩余待取字节数
	mov	r0,#_MidiByteCount
	mov	@r0,#0x00
;	main.c:501: USBByteCount = 0;	   //USB端点收到的长度
	mov	r0,#_USBByteCount
	mov	@r0,#0x00
;	main.c:502: UsbConfig = 0;		  //清除配置值
	mov	_UsbConfig,#0x00
;	main.c:503: UpPoint2_Busy = 0;
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x00
00209$:
;	main.c:505: if (UIF_SUSPEND)																 //USB总线挂起/唤醒完成
;	main.c:507: UIF_SUSPEND = 0;
;	assignBit
	jbc	_UIF_SUSPEND,00548$
	sjmp	00216$
00548$:
;	main.c:508: if ( USB_MIS_ST & bUMS_SUSPEND )											 //挂起
	mov	a,_USB_MIS_ST
	jnb	acc.2,00218$
;	main.c:513: while ( XBUS_AUX & bUART0_TX )
00210$:
	mov	a,_XBUS_AUX
	jb	acc.7,00210$
;	main.c:517: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:518: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:519: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;					  //USB或者RXD0/1有信号时可被唤醒
	mov	_WAKE_CTRL,#0xc1
;	main.c:520: PCON |= PD;																 //睡眠
	orl	_PCON,#0x02
;	main.c:521: SAFE_MOD = 0x55;
	mov	_SAFE_MOD,#0x55
;	main.c:522: SAFE_MOD = 0xAA;
	mov	_SAFE_MOD,#0xaa
;	main.c:523: WAKE_CTRL = 0x00;
	mov	_WAKE_CTRL,#0x00
	sjmp	00218$
00216$:
;	main.c:527: USB_INT_FG = 0xFF;															 //清中断标志
	mov	_USB_INT_FG,#0xff
00218$:
;	main.c:530: }
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
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;length                    Allocated to registers r7 
;Midi_Timeout              Allocated to registers 
;------------------------------------------------------------
;	main.c:551: main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c:553: uint8_t length = 0;
	mov	r7,#0x00
;	main.c:555: CfgFsys( );														   //CH559时钟选择配置
	push	ar7
	lcall	_CfgFsys
;	main.c:556: mDelaymS(5);														  //修改主频等待内部晶振稳定,必加
	mov	dptr,#0x0005
	lcall	_mDelaymS
;	main.c:557: mInitSTDIO( );														//串口0,可以用于调试
	lcall	_mInitSTDIO
;	main.c:558: UART1Setup( );														//用于CDC
	lcall	_UART1Setup
;	main.c:564: USBDeviceCfg();
	lcall	_USBDeviceCfg
;	main.c:565: USBDeviceEndPointCfg();											   //端点配置
	lcall	_USBDeviceEndPointCfg
;	main.c:566: USBDeviceIntCfg();													//中断初始化
	lcall	_USBDeviceIntCfg
	pop	ar7
;	main.c:567: UEP0_T_LEN = 0;
	mov	_UEP0_T_LEN,#0x00
;	main.c:568: UEP1_T_LEN = 0;													   //预使用发送长度一定要清空
	mov	_UEP1_T_LEN,#0x00
;	main.c:569: UEP2_T_LEN = 0;													   //预使用发送长度一定要清空
	mov	_UEP2_T_LEN,#0x00
;	main.c:571: while(1)
00112$:
;	main.c:573: if(UsbConfig)
	mov	a,_UsbConfig
	jz	00112$
;	main.c:575: if(USBByteCount)   //USB接收端点有数据
	mov	r0,#_USBByteCount
	mov	a,@r0
	jz	00102$
;	main.c:580: memcpy(Receive_Midi_Buf, Ep2Buffer, USBByteCount);
	mov	___memcpy_PARM_2,#_Ep2Buffer
	mov	(___memcpy_PARM_2 + 1),#(_Ep2Buffer >> 8)
	mov	(___memcpy_PARM_2 + 2),#0x00
	mov	r0,#_USBByteCount
	mov	___memcpy_PARM_3,@r0
	mov	(___memcpy_PARM_3 + 1),#0x00
	mov	dptr,#_Receive_Midi_Buf
	mov	b,#0x40
	lcall	___memcpy
;	main.c:585: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;
	anl	_UEP2_CTRL,#0xf3
;	main.c:586: length = USBByteCount;
	mov	r0,#_USBByteCount
	mov	ar7,@r0
;	main.c:587: USBByteCount = 0;
	mov	r0,#_USBByteCount
	mov	@r0,#0x00
00102$:
;	main.c:590: if(MidiByteCount)
	mov	r0,#_MidiByteCount
	mov	a,@r0
;	main.c:592: if(!UpPoint2_Busy)   //端点不繁忙（空闲后的第一包数据，只用作触发上传）
	mov	r0,#_UpPoint2_Busy
	mov	a,@r0
	jnz	00112$
;	main.c:595: if(length>0)
	mov	a,r7
	jz	00112$
;	main.c:607: memcpy(Ep2Buffer+MAX_PACKET_SIZE, Receive_Midi_Buf, length); //环回MIDI设备
	mov	___memcpy_PARM_2,#_Receive_Midi_Buf
	mov	(___memcpy_PARM_2 + 1),#0x00
	mov	(___memcpy_PARM_2 + 2),#0x40
	mov	___memcpy_PARM_3,r7
	mov	(___memcpy_PARM_3 + 1),#0x00
	mov	dptr,#(_Ep2Buffer + 0x0040)
	mov	b,#0x00
	push	ar7
	lcall	___memcpy
	pop	ar7
;	main.c:608: UEP2_T_LEN = length;								//预使用发送长度一定要清空
	mov	_UEP2_T_LEN,r7
;	main.c:609: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;			//应答ACK
	anl	_UEP2_CTRL,#0xfc
;	main.c:610: UpPoint2_Busy = 1;
	mov	r0,#_UpPoint2_Busy
	mov	@r0,#0x01
;	main.c:611: length = 0;
	mov	r7,#0x00
;	main.c:617: }
	sjmp	00112$
	.area CSEG    (CODE)
	.area CONST   (CODE)
_DevDesc:
	.db #0x12	; 18
	.db #0x01	; 1
	.db #0x10	; 16
	.db #0x01	; 1
	.db #0x00	; 0
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
	.db #0x61	; 97	'a'
	.db #0x00	; 0
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x80	; 128
	.db #0x32	; 50	'2'
	.db #0x09	; 9
	.db #0x04	; 4
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x09	; 9
	.db #0x24	; 36
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x09	; 9
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x09	; 9
	.db #0x04	; 4
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x03	; 3
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x07	; 7
	.db #0x24	; 36
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x01	; 1
	.db #0x25	; 37
	.db #0x00	; 0
	.db #0x06	; 6
	.db #0x24	; 36
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x06	; 6
	.db #0x24	; 36
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x00	; 0
	.db #0x09	; 9
	.db #0x24	; 36
	.db #0x03	; 3
	.db #0x01	; 1
	.db #0x03	; 3
	.db #0x01	; 1
	.db #0x02	; 2
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x09	; 9
	.db #0x24	; 36
	.db #0x03	; 3
	.db #0x02	; 2
	.db #0x04	; 4
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x00	; 0
	.db #0x07	; 7
	.db #0x05	; 5
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0x40	; 64
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x05	; 5
	.db #0x25	; 37
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x07	; 7
	.db #0x05	; 5
	.db #0x82	; 130
	.db #0x02	; 2
	.db #0x40	; 64
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x05	; 5
	.db #0x25	; 37
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x03	; 3
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
	.db #0x38	; 56	'8'
	.db #0x00	; 0
	.db #0x2d	; 45
	.db #0x00	; 0
	.db #0x33	; 51	'3'
	.db #0x00	; 0
	.db #0x2d	; 45
	.db #0x00	; 0
	.db #0x32	; 50	'2'
	.db #0x00	; 0
	.db #0x37	; 55	'7'
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
	.db #0x78	; 120	'x'
	.db #0x00	; 0
	.db #0x4d	; 77	'M'
	.db #0x00	; 0
	.db #0x49	; 73	'I'
	.db #0x00	; 0
	.db #0x44	; 68	'D'
	.db #0x00	; 0
	.db #0x49	; 73	'I'
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
	.area CABS    (ABS,CODE)
