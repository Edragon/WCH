                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.9.0 #11195 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mmcs51 --model-small
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _Manuf_Des
                                     12 	.globl _Prod_Des
                                     13 	.globl _SerDes
                                     14 	.globl _LangDes
                                     15 	.globl _CfgDesc
                                     16 	.globl _DevDesc
                                     17 	.globl _main
                                     18 	.globl _DeviceInterrupt
                                     19 	.globl _USBDeviceEndPointCfg
                                     20 	.globl _USBDeviceIntCfg
                                     21 	.globl _USBDeviceCfg
                                     22 	.globl _UART1Setup
                                     23 	.globl _mInitSTDIO
                                     24 	.globl _mDelaymS
                                     25 	.globl _CfgFsys
                                     26 	.globl ___memcpy
                                     27 	.globl _UIF_BUS_RST
                                     28 	.globl _UIF_DETECT
                                     29 	.globl _UIF_TRANSFER
                                     30 	.globl _UIF_SUSPEND
                                     31 	.globl _UIF_HST_SOF
                                     32 	.globl _UIF_FIFO_OV
                                     33 	.globl _U_SIE_FREE
                                     34 	.globl _U_TOG_OK
                                     35 	.globl _U_IS_NAK
                                     36 	.globl _ADC_CHAN0
                                     37 	.globl _ADC_CHAN1
                                     38 	.globl _CMP_CHAN
                                     39 	.globl _ADC_START
                                     40 	.globl _ADC_IF
                                     41 	.globl _CMP_IF
                                     42 	.globl _CMPO
                                     43 	.globl _U1RI
                                     44 	.globl _U1TI
                                     45 	.globl _U1RB8
                                     46 	.globl _U1TB8
                                     47 	.globl _U1REN
                                     48 	.globl _U1SMOD
                                     49 	.globl _U1SM0
                                     50 	.globl _S0_R_FIFO
                                     51 	.globl _S0_T_FIFO
                                     52 	.globl _S0_FREE
                                     53 	.globl _S0_IF_BYTE
                                     54 	.globl _S0_IF_FIRST
                                     55 	.globl _S0_IF_OV
                                     56 	.globl _S0_FST_ACT
                                     57 	.globl _CP_RL2
                                     58 	.globl _C_T2
                                     59 	.globl _TR2
                                     60 	.globl _EXEN2
                                     61 	.globl _TCLK
                                     62 	.globl _RCLK
                                     63 	.globl _EXF2
                                     64 	.globl _CAP1F
                                     65 	.globl _TF2
                                     66 	.globl _RI
                                     67 	.globl _TI
                                     68 	.globl _RB8
                                     69 	.globl _TB8
                                     70 	.globl _REN
                                     71 	.globl _SM2
                                     72 	.globl _SM1
                                     73 	.globl _SM0
                                     74 	.globl _IT0
                                     75 	.globl _IE0
                                     76 	.globl _IT1
                                     77 	.globl _IE1
                                     78 	.globl _TR0
                                     79 	.globl _TF0
                                     80 	.globl _TR1
                                     81 	.globl _TF1
                                     82 	.globl _RXD
                                     83 	.globl _PWM1_
                                     84 	.globl _TXD
                                     85 	.globl _PWM2_
                                     86 	.globl _AIN3
                                     87 	.globl _VBUS1
                                     88 	.globl _INT0
                                     89 	.globl _TXD1_
                                     90 	.globl _INT1
                                     91 	.globl _T0
                                     92 	.globl _RXD1_
                                     93 	.globl _PWM2
                                     94 	.globl _T1
                                     95 	.globl _UDP
                                     96 	.globl _UDM
                                     97 	.globl _TIN0
                                     98 	.globl _CAP1
                                     99 	.globl _T2
                                    100 	.globl _AIN0
                                    101 	.globl _VBUS2
                                    102 	.globl _TIN1
                                    103 	.globl _CAP2
                                    104 	.globl _T2EX
                                    105 	.globl _RXD_
                                    106 	.globl _TXD_
                                    107 	.globl _AIN1
                                    108 	.globl _UCC1
                                    109 	.globl _TIN2
                                    110 	.globl _SCS
                                    111 	.globl _CAP1_
                                    112 	.globl _T2_
                                    113 	.globl _AIN2
                                    114 	.globl _UCC2
                                    115 	.globl _TIN3
                                    116 	.globl _PWM1
                                    117 	.globl _MOSI
                                    118 	.globl _TIN4
                                    119 	.globl _RXD1
                                    120 	.globl _MISO
                                    121 	.globl _TIN5
                                    122 	.globl _TXD1
                                    123 	.globl _SCK
                                    124 	.globl _IE_SPI0
                                    125 	.globl _IE_TKEY
                                    126 	.globl _IE_USB
                                    127 	.globl _IE_ADC
                                    128 	.globl _IE_UART1
                                    129 	.globl _IE_PWMX
                                    130 	.globl _IE_GPIO
                                    131 	.globl _IE_WDOG
                                    132 	.globl _PX0
                                    133 	.globl _PT0
                                    134 	.globl _PX1
                                    135 	.globl _PT1
                                    136 	.globl _PS
                                    137 	.globl _PT2
                                    138 	.globl _PL_FLAG
                                    139 	.globl _PH_FLAG
                                    140 	.globl _EX0
                                    141 	.globl _ET0
                                    142 	.globl _EX1
                                    143 	.globl _ET1
                                    144 	.globl _ES
                                    145 	.globl _ET2
                                    146 	.globl _E_DIS
                                    147 	.globl _EA
                                    148 	.globl _P
                                    149 	.globl _F1
                                    150 	.globl _OV
                                    151 	.globl _RS0
                                    152 	.globl _RS1
                                    153 	.globl _F0
                                    154 	.globl _AC
                                    155 	.globl _CY
                                    156 	.globl _UEP1_DMA_H
                                    157 	.globl _UEP1_DMA_L
                                    158 	.globl _UEP1_DMA
                                    159 	.globl _UEP0_DMA_H
                                    160 	.globl _UEP0_DMA_L
                                    161 	.globl _UEP0_DMA
                                    162 	.globl _UEP2_3_MOD
                                    163 	.globl _UEP4_1_MOD
                                    164 	.globl _UEP3_DMA_H
                                    165 	.globl _UEP3_DMA_L
                                    166 	.globl _UEP3_DMA
                                    167 	.globl _UEP2_DMA_H
                                    168 	.globl _UEP2_DMA_L
                                    169 	.globl _UEP2_DMA
                                    170 	.globl _USB_DEV_AD
                                    171 	.globl _USB_CTRL
                                    172 	.globl _USB_INT_EN
                                    173 	.globl _UEP4_T_LEN
                                    174 	.globl _UEP4_CTRL
                                    175 	.globl _UEP0_T_LEN
                                    176 	.globl _UEP0_CTRL
                                    177 	.globl _USB_RX_LEN
                                    178 	.globl _USB_MIS_ST
                                    179 	.globl _USB_INT_ST
                                    180 	.globl _USB_INT_FG
                                    181 	.globl _UEP3_T_LEN
                                    182 	.globl _UEP3_CTRL
                                    183 	.globl _UEP2_T_LEN
                                    184 	.globl _UEP2_CTRL
                                    185 	.globl _UEP1_T_LEN
                                    186 	.globl _UEP1_CTRL
                                    187 	.globl _UDEV_CTRL
                                    188 	.globl _USB_C_CTRL
                                    189 	.globl _TKEY_DATH
                                    190 	.globl _TKEY_DATL
                                    191 	.globl _TKEY_DAT
                                    192 	.globl _TKEY_CTRL
                                    193 	.globl _ADC_DATA
                                    194 	.globl _ADC_CFG
                                    195 	.globl _ADC_CTRL
                                    196 	.globl _SBAUD1
                                    197 	.globl _SBUF1
                                    198 	.globl _SCON1
                                    199 	.globl _SPI0_SETUP
                                    200 	.globl _SPI0_CK_SE
                                    201 	.globl _SPI0_CTRL
                                    202 	.globl _SPI0_DATA
                                    203 	.globl _SPI0_STAT
                                    204 	.globl _PWM_CK_SE
                                    205 	.globl _PWM_CTRL
                                    206 	.globl _PWM_DATA1
                                    207 	.globl _PWM_DATA2
                                    208 	.globl _T2CAP1H
                                    209 	.globl _T2CAP1L
                                    210 	.globl _T2CAP1
                                    211 	.globl _TH2
                                    212 	.globl _TL2
                                    213 	.globl _T2COUNT
                                    214 	.globl _RCAP2H
                                    215 	.globl _RCAP2L
                                    216 	.globl _RCAP2
                                    217 	.globl _T2MOD
                                    218 	.globl _T2CON
                                    219 	.globl _SBUF
                                    220 	.globl _SCON
                                    221 	.globl _TH1
                                    222 	.globl _TH0
                                    223 	.globl _TL1
                                    224 	.globl _TL0
                                    225 	.globl _TMOD
                                    226 	.globl _TCON
                                    227 	.globl _XBUS_AUX
                                    228 	.globl _PIN_FUNC
                                    229 	.globl _P3_DIR_PU
                                    230 	.globl _P3_MOD_OC
                                    231 	.globl _P3
                                    232 	.globl _P2
                                    233 	.globl _P1_DIR_PU
                                    234 	.globl _P1_MOD_OC
                                    235 	.globl _P1
                                    236 	.globl _ROM_CTRL
                                    237 	.globl _ROM_DATA_H
                                    238 	.globl _ROM_DATA_L
                                    239 	.globl _ROM_DATA
                                    240 	.globl _ROM_ADDR_H
                                    241 	.globl _ROM_ADDR_L
                                    242 	.globl _ROM_ADDR
                                    243 	.globl _GPIO_IE
                                    244 	.globl _IP_EX
                                    245 	.globl _IE_EX
                                    246 	.globl _IP
                                    247 	.globl _IE
                                    248 	.globl _WDOG_COUNT
                                    249 	.globl _RESET_KEEP
                                    250 	.globl _WAKE_CTRL
                                    251 	.globl _CLOCK_CFG
                                    252 	.globl _PCON
                                    253 	.globl _GLOBAL_CFG
                                    254 	.globl _SAFE_MOD
                                    255 	.globl _DPH
                                    256 	.globl _DPL
                                    257 	.globl _SP
                                    258 	.globl _B
                                    259 	.globl _ACC
                                    260 	.globl _PSW
                                    261 	.globl _Ep2Buffer
                                    262 	.globl _Ep1Buffer
                                    263 	.globl _Ep0Buffer
                                    264 	.globl _UpPoint2_Busy
                                    265 	.globl _USBBufOutPoint
                                    266 	.globl _USBByteCount
                                    267 	.globl _MidiByteCount
                                    268 	.globl _Midi_Output_Point
                                    269 	.globl _Midi_Input_Point
                                    270 	.globl _Receive_Midi_Buf
                                    271 	.globl _SetupReqBuf
                                    272 	.globl _pDescr
                                    273 	.globl _UsbConfig
                                    274 	.globl _Count
                                    275 	.globl _SetupReq
                                    276 	.globl _SetupLen
                                    277 ;--------------------------------------------------------
                                    278 ; special function registers
                                    279 ;--------------------------------------------------------
                                    280 	.area RSEG    (ABS,DATA)
      000000                        281 	.org 0x0000
                           0000D0   282 _PSW	=	0x00d0
                           0000E0   283 _ACC	=	0x00e0
                           0000F0   284 _B	=	0x00f0
                           000081   285 _SP	=	0x0081
                           000082   286 _DPL	=	0x0082
                           000083   287 _DPH	=	0x0083
                           0000A1   288 _SAFE_MOD	=	0x00a1
                           0000B1   289 _GLOBAL_CFG	=	0x00b1
                           000087   290 _PCON	=	0x0087
                           0000B9   291 _CLOCK_CFG	=	0x00b9
                           0000A9   292 _WAKE_CTRL	=	0x00a9
                           0000FE   293 _RESET_KEEP	=	0x00fe
                           0000FF   294 _WDOG_COUNT	=	0x00ff
                           0000A8   295 _IE	=	0x00a8
                           0000B8   296 _IP	=	0x00b8
                           0000E8   297 _IE_EX	=	0x00e8
                           0000E9   298 _IP_EX	=	0x00e9
                           0000C7   299 _GPIO_IE	=	0x00c7
                           008584   300 _ROM_ADDR	=	0x8584
                           000084   301 _ROM_ADDR_L	=	0x0084
                           000085   302 _ROM_ADDR_H	=	0x0085
                           008F8E   303 _ROM_DATA	=	0x8f8e
                           00008E   304 _ROM_DATA_L	=	0x008e
                           00008F   305 _ROM_DATA_H	=	0x008f
                           000086   306 _ROM_CTRL	=	0x0086
                           000090   307 _P1	=	0x0090
                           000092   308 _P1_MOD_OC	=	0x0092
                           000093   309 _P1_DIR_PU	=	0x0093
                           0000A0   310 _P2	=	0x00a0
                           0000B0   311 _P3	=	0x00b0
                           000096   312 _P3_MOD_OC	=	0x0096
                           000097   313 _P3_DIR_PU	=	0x0097
                           0000C6   314 _PIN_FUNC	=	0x00c6
                           0000A2   315 _XBUS_AUX	=	0x00a2
                           000088   316 _TCON	=	0x0088
                           000089   317 _TMOD	=	0x0089
                           00008A   318 _TL0	=	0x008a
                           00008B   319 _TL1	=	0x008b
                           00008C   320 _TH0	=	0x008c
                           00008D   321 _TH1	=	0x008d
                           000098   322 _SCON	=	0x0098
                           000099   323 _SBUF	=	0x0099
                           0000C8   324 _T2CON	=	0x00c8
                           0000C9   325 _T2MOD	=	0x00c9
                           00CBCA   326 _RCAP2	=	0xcbca
                           0000CA   327 _RCAP2L	=	0x00ca
                           0000CB   328 _RCAP2H	=	0x00cb
                           00CDCC   329 _T2COUNT	=	0xcdcc
                           0000CC   330 _TL2	=	0x00cc
                           0000CD   331 _TH2	=	0x00cd
                           00CFCE   332 _T2CAP1	=	0xcfce
                           0000CE   333 _T2CAP1L	=	0x00ce
                           0000CF   334 _T2CAP1H	=	0x00cf
                           00009B   335 _PWM_DATA2	=	0x009b
                           00009C   336 _PWM_DATA1	=	0x009c
                           00009D   337 _PWM_CTRL	=	0x009d
                           00009E   338 _PWM_CK_SE	=	0x009e
                           0000F8   339 _SPI0_STAT	=	0x00f8
                           0000F9   340 _SPI0_DATA	=	0x00f9
                           0000FA   341 _SPI0_CTRL	=	0x00fa
                           0000FB   342 _SPI0_CK_SE	=	0x00fb
                           0000FC   343 _SPI0_SETUP	=	0x00fc
                           0000C0   344 _SCON1	=	0x00c0
                           0000C1   345 _SBUF1	=	0x00c1
                           0000C2   346 _SBAUD1	=	0x00c2
                           000080   347 _ADC_CTRL	=	0x0080
                           00009A   348 _ADC_CFG	=	0x009a
                           00009F   349 _ADC_DATA	=	0x009f
                           0000C3   350 _TKEY_CTRL	=	0x00c3
                           00C5C4   351 _TKEY_DAT	=	0xc5c4
                           0000C4   352 _TKEY_DATL	=	0x00c4
                           0000C5   353 _TKEY_DATH	=	0x00c5
                           000091   354 _USB_C_CTRL	=	0x0091
                           0000D1   355 _UDEV_CTRL	=	0x00d1
                           0000D2   356 _UEP1_CTRL	=	0x00d2
                           0000D3   357 _UEP1_T_LEN	=	0x00d3
                           0000D4   358 _UEP2_CTRL	=	0x00d4
                           0000D5   359 _UEP2_T_LEN	=	0x00d5
                           0000D6   360 _UEP3_CTRL	=	0x00d6
                           0000D7   361 _UEP3_T_LEN	=	0x00d7
                           0000D8   362 _USB_INT_FG	=	0x00d8
                           0000D9   363 _USB_INT_ST	=	0x00d9
                           0000DA   364 _USB_MIS_ST	=	0x00da
                           0000DB   365 _USB_RX_LEN	=	0x00db
                           0000DC   366 _UEP0_CTRL	=	0x00dc
                           0000DD   367 _UEP0_T_LEN	=	0x00dd
                           0000DE   368 _UEP4_CTRL	=	0x00de
                           0000DF   369 _UEP4_T_LEN	=	0x00df
                           0000E1   370 _USB_INT_EN	=	0x00e1
                           0000E2   371 _USB_CTRL	=	0x00e2
                           0000E3   372 _USB_DEV_AD	=	0x00e3
                           00E5E4   373 _UEP2_DMA	=	0xe5e4
                           0000E4   374 _UEP2_DMA_L	=	0x00e4
                           0000E5   375 _UEP2_DMA_H	=	0x00e5
                           00E7E6   376 _UEP3_DMA	=	0xe7e6
                           0000E6   377 _UEP3_DMA_L	=	0x00e6
                           0000E7   378 _UEP3_DMA_H	=	0x00e7
                           0000EA   379 _UEP4_1_MOD	=	0x00ea
                           0000EB   380 _UEP2_3_MOD	=	0x00eb
                           00EDEC   381 _UEP0_DMA	=	0xedec
                           0000EC   382 _UEP0_DMA_L	=	0x00ec
                           0000ED   383 _UEP0_DMA_H	=	0x00ed
                           00EFEE   384 _UEP1_DMA	=	0xefee
                           0000EE   385 _UEP1_DMA_L	=	0x00ee
                           0000EF   386 _UEP1_DMA_H	=	0x00ef
                                    387 ;--------------------------------------------------------
                                    388 ; special function bits
                                    389 ;--------------------------------------------------------
                                    390 	.area RSEG    (ABS,DATA)
      000000                        391 	.org 0x0000
                           0000D7   392 _CY	=	0x00d7
                           0000D6   393 _AC	=	0x00d6
                           0000D5   394 _F0	=	0x00d5
                           0000D4   395 _RS1	=	0x00d4
                           0000D3   396 _RS0	=	0x00d3
                           0000D2   397 _OV	=	0x00d2
                           0000D1   398 _F1	=	0x00d1
                           0000D0   399 _P	=	0x00d0
                           0000AF   400 _EA	=	0x00af
                           0000AE   401 _E_DIS	=	0x00ae
                           0000AD   402 _ET2	=	0x00ad
                           0000AC   403 _ES	=	0x00ac
                           0000AB   404 _ET1	=	0x00ab
                           0000AA   405 _EX1	=	0x00aa
                           0000A9   406 _ET0	=	0x00a9
                           0000A8   407 _EX0	=	0x00a8
                           0000BF   408 _PH_FLAG	=	0x00bf
                           0000BE   409 _PL_FLAG	=	0x00be
                           0000BD   410 _PT2	=	0x00bd
                           0000BC   411 _PS	=	0x00bc
                           0000BB   412 _PT1	=	0x00bb
                           0000BA   413 _PX1	=	0x00ba
                           0000B9   414 _PT0	=	0x00b9
                           0000B8   415 _PX0	=	0x00b8
                           0000EF   416 _IE_WDOG	=	0x00ef
                           0000EE   417 _IE_GPIO	=	0x00ee
                           0000ED   418 _IE_PWMX	=	0x00ed
                           0000EC   419 _IE_UART1	=	0x00ec
                           0000EB   420 _IE_ADC	=	0x00eb
                           0000EA   421 _IE_USB	=	0x00ea
                           0000E9   422 _IE_TKEY	=	0x00e9
                           0000E8   423 _IE_SPI0	=	0x00e8
                           000097   424 _SCK	=	0x0097
                           000097   425 _TXD1	=	0x0097
                           000097   426 _TIN5	=	0x0097
                           000096   427 _MISO	=	0x0096
                           000096   428 _RXD1	=	0x0096
                           000096   429 _TIN4	=	0x0096
                           000095   430 _MOSI	=	0x0095
                           000095   431 _PWM1	=	0x0095
                           000095   432 _TIN3	=	0x0095
                           000095   433 _UCC2	=	0x0095
                           000095   434 _AIN2	=	0x0095
                           000094   435 _T2_	=	0x0094
                           000094   436 _CAP1_	=	0x0094
                           000094   437 _SCS	=	0x0094
                           000094   438 _TIN2	=	0x0094
                           000094   439 _UCC1	=	0x0094
                           000094   440 _AIN1	=	0x0094
                           000093   441 _TXD_	=	0x0093
                           000092   442 _RXD_	=	0x0092
                           000091   443 _T2EX	=	0x0091
                           000091   444 _CAP2	=	0x0091
                           000091   445 _TIN1	=	0x0091
                           000091   446 _VBUS2	=	0x0091
                           000091   447 _AIN0	=	0x0091
                           000090   448 _T2	=	0x0090
                           000090   449 _CAP1	=	0x0090
                           000090   450 _TIN0	=	0x0090
                           0000B7   451 _UDM	=	0x00b7
                           0000B6   452 _UDP	=	0x00b6
                           0000B5   453 _T1	=	0x00b5
                           0000B4   454 _PWM2	=	0x00b4
                           0000B4   455 _RXD1_	=	0x00b4
                           0000B4   456 _T0	=	0x00b4
                           0000B3   457 _INT1	=	0x00b3
                           0000B2   458 _TXD1_	=	0x00b2
                           0000B2   459 _INT0	=	0x00b2
                           0000B2   460 _VBUS1	=	0x00b2
                           0000B2   461 _AIN3	=	0x00b2
                           0000B1   462 _PWM2_	=	0x00b1
                           0000B1   463 _TXD	=	0x00b1
                           0000B0   464 _PWM1_	=	0x00b0
                           0000B0   465 _RXD	=	0x00b0
                           00008F   466 _TF1	=	0x008f
                           00008E   467 _TR1	=	0x008e
                           00008D   468 _TF0	=	0x008d
                           00008C   469 _TR0	=	0x008c
                           00008B   470 _IE1	=	0x008b
                           00008A   471 _IT1	=	0x008a
                           000089   472 _IE0	=	0x0089
                           000088   473 _IT0	=	0x0088
                           00009F   474 _SM0	=	0x009f
                           00009E   475 _SM1	=	0x009e
                           00009D   476 _SM2	=	0x009d
                           00009C   477 _REN	=	0x009c
                           00009B   478 _TB8	=	0x009b
                           00009A   479 _RB8	=	0x009a
                           000099   480 _TI	=	0x0099
                           000098   481 _RI	=	0x0098
                           0000CF   482 _TF2	=	0x00cf
                           0000CF   483 _CAP1F	=	0x00cf
                           0000CE   484 _EXF2	=	0x00ce
                           0000CD   485 _RCLK	=	0x00cd
                           0000CC   486 _TCLK	=	0x00cc
                           0000CB   487 _EXEN2	=	0x00cb
                           0000CA   488 _TR2	=	0x00ca
                           0000C9   489 _C_T2	=	0x00c9
                           0000C8   490 _CP_RL2	=	0x00c8
                           0000FF   491 _S0_FST_ACT	=	0x00ff
                           0000FE   492 _S0_IF_OV	=	0x00fe
                           0000FD   493 _S0_IF_FIRST	=	0x00fd
                           0000FC   494 _S0_IF_BYTE	=	0x00fc
                           0000FB   495 _S0_FREE	=	0x00fb
                           0000FA   496 _S0_T_FIFO	=	0x00fa
                           0000F8   497 _S0_R_FIFO	=	0x00f8
                           0000C7   498 _U1SM0	=	0x00c7
                           0000C5   499 _U1SMOD	=	0x00c5
                           0000C4   500 _U1REN	=	0x00c4
                           0000C3   501 _U1TB8	=	0x00c3
                           0000C2   502 _U1RB8	=	0x00c2
                           0000C1   503 _U1TI	=	0x00c1
                           0000C0   504 _U1RI	=	0x00c0
                           000087   505 _CMPO	=	0x0087
                           000086   506 _CMP_IF	=	0x0086
                           000085   507 _ADC_IF	=	0x0085
                           000084   508 _ADC_START	=	0x0084
                           000083   509 _CMP_CHAN	=	0x0083
                           000081   510 _ADC_CHAN1	=	0x0081
                           000080   511 _ADC_CHAN0	=	0x0080
                           0000DF   512 _U_IS_NAK	=	0x00df
                           0000DE   513 _U_TOG_OK	=	0x00de
                           0000DD   514 _U_SIE_FREE	=	0x00dd
                           0000DC   515 _UIF_FIFO_OV	=	0x00dc
                           0000DB   516 _UIF_HST_SOF	=	0x00db
                           0000DA   517 _UIF_SUSPEND	=	0x00da
                           0000D9   518 _UIF_TRANSFER	=	0x00d9
                           0000D8   519 _UIF_DETECT	=	0x00d8
                           0000D8   520 _UIF_BUS_RST	=	0x00d8
                                    521 ;--------------------------------------------------------
                                    522 ; overlayable register banks
                                    523 ;--------------------------------------------------------
                                    524 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        525 	.ds 8
                                    526 ;--------------------------------------------------------
                                    527 ; overlayable bit register bank
                                    528 ;--------------------------------------------------------
                                    529 	.area BIT_BANK	(REL,OVR,DATA)
      000020                        530 bits:
      000020                        531 	.ds 1
                           008000   532 	b0 = bits[0]
                           008100   533 	b1 = bits[1]
                           008200   534 	b2 = bits[2]
                           008300   535 	b3 = bits[3]
                           008400   536 	b4 = bits[4]
                           008500   537 	b5 = bits[5]
                           008600   538 	b6 = bits[6]
                           008700   539 	b7 = bits[7]
                                    540 ;--------------------------------------------------------
                                    541 ; internal ram data
                                    542 ;--------------------------------------------------------
                                    543 	.area DSEG    (DATA)
      000008                        544 _SetupLen::
      000008                        545 	.ds 2
      00000A                        546 _SetupReq::
      00000A                        547 	.ds 1
      00000B                        548 _Count::
      00000B                        549 	.ds 1
      00000C                        550 _UsbConfig::
      00000C                        551 	.ds 1
      00000D                        552 _pDescr::
      00000D                        553 	.ds 3
      000010                        554 _SetupReqBuf::
      000010                        555 	.ds 8
                                    556 ;--------------------------------------------------------
                                    557 ; overlayable items in internal ram 
                                    558 ;--------------------------------------------------------
                                    559 ;--------------------------------------------------------
                                    560 ; Stack segment in internal ram 
                                    561 ;--------------------------------------------------------
                                    562 	.area	SSEG
      000072                        563 __start__stack:
      000072                        564 	.ds	1
                                    565 
                                    566 ;--------------------------------------------------------
                                    567 ; indirectly addressable internal ram data
                                    568 ;--------------------------------------------------------
                                    569 	.area ISEG    (DATA)
      00002C                        570 _Receive_Midi_Buf::
      00002C                        571 	.ds 64
      00006C                        572 _Midi_Input_Point::
      00006C                        573 	.ds 1
      00006D                        574 _Midi_Output_Point::
      00006D                        575 	.ds 1
      00006E                        576 _MidiByteCount::
      00006E                        577 	.ds 1
      00006F                        578 _USBByteCount::
      00006F                        579 	.ds 1
      000070                        580 _USBBufOutPoint::
      000070                        581 	.ds 1
      000071                        582 _UpPoint2_Busy::
      000071                        583 	.ds 1
                                    584 ;--------------------------------------------------------
                                    585 ; absolute internal ram data
                                    586 ;--------------------------------------------------------
                                    587 	.area IABS    (ABS,DATA)
                                    588 	.area IABS    (ABS,DATA)
                                    589 ;--------------------------------------------------------
                                    590 ; bit data
                                    591 ;--------------------------------------------------------
                                    592 	.area BSEG    (BIT)
                                    593 ;--------------------------------------------------------
                                    594 ; paged external ram data
                                    595 ;--------------------------------------------------------
                                    596 	.area PSEG    (PAG,XDATA)
                                    597 ;--------------------------------------------------------
                                    598 ; external ram data
                                    599 ;--------------------------------------------------------
                                    600 	.area XSEG    (XDATA)
                           000000   601 _Ep0Buffer	=	0x0000
                           000040   602 _Ep1Buffer	=	0x0040
                           000080   603 _Ep2Buffer	=	0x0080
                                    604 ;--------------------------------------------------------
                                    605 ; absolute external ram data
                                    606 ;--------------------------------------------------------
                                    607 	.area XABS    (ABS,XDATA)
                                    608 ;--------------------------------------------------------
                                    609 ; external initialized ram data
                                    610 ;--------------------------------------------------------
                                    611 	.area XISEG   (XDATA)
                                    612 	.area HOME    (CODE)
                                    613 	.area GSINIT0 (CODE)
                                    614 	.area GSINIT1 (CODE)
                                    615 	.area GSINIT2 (CODE)
                                    616 	.area GSINIT3 (CODE)
                                    617 	.area GSINIT4 (CODE)
                                    618 	.area GSINIT5 (CODE)
                                    619 	.area GSINIT  (CODE)
                                    620 	.area GSFINAL (CODE)
                                    621 	.area CSEG    (CODE)
                                    622 ;--------------------------------------------------------
                                    623 ; interrupt vector 
                                    624 ;--------------------------------------------------------
                                    625 	.area HOME    (CODE)
      000000                        626 __interrupt_vect:
      000000 02 00 49         [24]  627 	ljmp	__sdcc_gsinit_startup
      000003 32               [24]  628 	reti
      000004                        629 	.ds	7
      00000B 32               [24]  630 	reti
      00000C                        631 	.ds	7
      000013 32               [24]  632 	reti
      000014                        633 	.ds	7
      00001B 32               [24]  634 	reti
      00001C                        635 	.ds	7
      000023 32               [24]  636 	reti
      000024                        637 	.ds	7
      00002B 32               [24]  638 	reti
      00002C                        639 	.ds	7
      000033 32               [24]  640 	reti
      000034                        641 	.ds	7
      00003B 32               [24]  642 	reti
      00003C                        643 	.ds	7
      000043 02 01 09         [24]  644 	ljmp	_DeviceInterrupt
                                    645 ;--------------------------------------------------------
                                    646 ; global & static initialisations
                                    647 ;--------------------------------------------------------
                                    648 	.area HOME    (CODE)
                                    649 	.area GSINIT  (CODE)
                                    650 	.area GSFINAL (CODE)
                                    651 	.area GSINIT  (CODE)
                                    652 	.globl __sdcc_gsinit_startup
                                    653 	.globl __sdcc_program_startup
                                    654 	.globl __start__stack
                                    655 	.globl __mcs51_genXINIT
                                    656 	.globl __mcs51_genXRAMCLEAR
                                    657 	.globl __mcs51_genRAMCLEAR
                                    658 ;	main.c:83: volatile __idata uint8_t Midi_Input_Point = 0;   //循环缓冲区写入指针，总线复位需要初始化为0
      0000A2 78 6C            [12]  659 	mov	r0,#_Midi_Input_Point
      0000A4 76 00            [12]  660 	mov	@r0,#0x00
                                    661 ;	main.c:84: volatile __idata uint8_t Midi_Output_Point = 0;  //循环缓冲区取出指针，总线复位需要初始化为0
      0000A6 78 6D            [12]  662 	mov	r0,#_Midi_Output_Point
      0000A8 76 00            [12]  663 	mov	@r0,#0x00
                                    664 ;	main.c:85: volatile __idata uint8_t MidiByteCount = 0;	  //当前缓冲区剩余待取字节数
      0000AA 78 6E            [12]  665 	mov	r0,#_MidiByteCount
      0000AC 76 00            [12]  666 	mov	@r0,#0x00
                                    667 ;	main.c:88: volatile __idata uint8_t USBByteCount = 0;	  //代表USB端点接收到的数据
      0000AE 78 6F            [12]  668 	mov	r0,#_USBByteCount
      0000B0 76 00            [12]  669 	mov	@r0,#0x00
                                    670 ;	main.c:89: volatile __idata uint8_t USBBufOutPoint = 0;	//取数据指针
      0000B2 78 70            [12]  671 	mov	r0,#_USBBufOutPoint
      0000B4 76 00            [12]  672 	mov	@r0,#0x00
                                    673 ;	main.c:91: volatile __idata uint8_t UpPoint2_Busy  = 0;   //上传端点是否忙标志
      0000B6 78 71            [12]  674 	mov	r0,#_UpPoint2_Busy
      0000B8 76 00            [12]  675 	mov	@r0,#0x00
                                    676 	.area GSFINAL (CODE)
      0000BA 02 00 46         [24]  677 	ljmp	__sdcc_program_startup
                                    678 ;--------------------------------------------------------
                                    679 ; Home
                                    680 ;--------------------------------------------------------
                                    681 	.area HOME    (CODE)
                                    682 	.area HOME    (CODE)
      000046                        683 __sdcc_program_startup:
      000046 02 06 14         [24]  684 	ljmp	_main
                                    685 ;	return from main will return to caller
                                    686 ;--------------------------------------------------------
                                    687 ; code
                                    688 ;--------------------------------------------------------
                                    689 	.area CSEG    (CODE)
                                    690 ;------------------------------------------------------------
                                    691 ;Allocation info for local variables in function 'USBDeviceCfg'
                                    692 ;------------------------------------------------------------
                                    693 ;	main.c:101: void USBDeviceCfg()
                                    694 ;	-----------------------------------------
                                    695 ;	 function USBDeviceCfg
                                    696 ;	-----------------------------------------
      0000BD                        697 _USBDeviceCfg:
                           000007   698 	ar7 = 0x07
                           000006   699 	ar6 = 0x06
                           000005   700 	ar5 = 0x05
                           000004   701 	ar4 = 0x04
                           000003   702 	ar3 = 0x03
                           000002   703 	ar2 = 0x02
                           000001   704 	ar1 = 0x01
                           000000   705 	ar0 = 0x00
                                    706 ;	main.c:103: USB_CTRL = 0x00;														   //清空USB控制寄存器
      0000BD 75 E2 00         [24]  707 	mov	_USB_CTRL,#0x00
                                    708 ;	main.c:104: USB_CTRL &= ~bUC_HOST_MODE;												//该位为选择设备模式
      0000C0 53 E2 7F         [24]  709 	anl	_USB_CTRL,#0x7f
                                    710 ;	main.c:105: USB_CTRL |=  bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;					//USB设备和内部上拉使能,在中断期间中断标志未清除前自动返回NAK
      0000C3 43 E2 29         [24]  711 	orl	_USB_CTRL,#0x29
                                    712 ;	main.c:106: USB_DEV_AD = 0x00;														 //设备地址初始化
      0000C6 75 E3 00         [24]  713 	mov	_USB_DEV_AD,#0x00
                                    714 ;	main.c:109: USB_CTRL &= ~bUC_LOW_SPEED;
      0000C9 53 E2 BF         [24]  715 	anl	_USB_CTRL,#0xbf
                                    716 ;	main.c:110: UDEV_CTRL &= ~bUD_LOW_SPEED;											 //选择全速12M模式，默认方式
      0000CC 53 D1 FB         [24]  717 	anl	_UDEV_CTRL,#0xfb
                                    718 ;	main.c:111: UDEV_CTRL = bUD_PD_DIS;  // 禁止DP/DM下拉电阻
      0000CF 75 D1 80         [24]  719 	mov	_UDEV_CTRL,#0x80
                                    720 ;	main.c:112: UDEV_CTRL |= bUD_PORT_EN;												  //使能物理端口
      0000D2 43 D1 01         [24]  721 	orl	_UDEV_CTRL,#0x01
                                    722 ;	main.c:113: }
      0000D5 22               [24]  723 	ret
                                    724 ;------------------------------------------------------------
                                    725 ;Allocation info for local variables in function 'USBDeviceIntCfg'
                                    726 ;------------------------------------------------------------
                                    727 ;	main.c:121: void USBDeviceIntCfg()
                                    728 ;	-----------------------------------------
                                    729 ;	 function USBDeviceIntCfg
                                    730 ;	-----------------------------------------
      0000D6                        731 _USBDeviceIntCfg:
                                    732 ;	main.c:123: USB_INT_EN |= bUIE_SUSPEND;											   //使能设备挂起中断
      0000D6 43 E1 04         [24]  733 	orl	_USB_INT_EN,#0x04
                                    734 ;	main.c:124: USB_INT_EN |= bUIE_TRANSFER;											  //使能USB传输完成中断
      0000D9 43 E1 02         [24]  735 	orl	_USB_INT_EN,#0x02
                                    736 ;	main.c:125: USB_INT_EN |= bUIE_BUS_RST;											   //使能设备模式USB总线复位中断
      0000DC 43 E1 01         [24]  737 	orl	_USB_INT_EN,#0x01
                                    738 ;	main.c:126: USB_INT_FG |= 0x1F;													   //清中断标志
      0000DF 43 D8 1F         [24]  739 	orl	_USB_INT_FG,#0x1f
                                    740 ;	main.c:127: IE_USB = 1;															   //使能USB中断
                                    741 ;	assignBit
      0000E2 D2 EA            [12]  742 	setb	_IE_USB
                                    743 ;	main.c:128: EA = 1;																   //允许单片机中断
                                    744 ;	assignBit
      0000E4 D2 AF            [12]  745 	setb	_EA
                                    746 ;	main.c:129: }
      0000E6 22               [24]  747 	ret
                                    748 ;------------------------------------------------------------
                                    749 ;Allocation info for local variables in function 'USBDeviceEndPointCfg'
                                    750 ;------------------------------------------------------------
                                    751 ;	main.c:137: void USBDeviceEndPointCfg()
                                    752 ;	-----------------------------------------
                                    753 ;	 function USBDeviceEndPointCfg
                                    754 ;	-----------------------------------------
      0000E7                        755 _USBDeviceEndPointCfg:
                                    756 ;	main.c:139: UEP1_DMA = (uint16_t) Ep1Buffer;													  //端点1 发送数据传输地址
      0000E7 75 EE 40         [24]  757 	mov	((_UEP1_DMA >> 0) & 0xFF),#_Ep1Buffer
      0000EA 75 EF 00         [24]  758 	mov	((_UEP1_DMA >> 8) & 0xFF),#(_Ep1Buffer >> 8)
                                    759 ;	main.c:140: UEP2_DMA = (uint16_t) Ep2Buffer;													  //端点2 IN数据传输地址
      0000ED 75 E4 80         [24]  760 	mov	((_UEP2_DMA >> 0) & 0xFF),#_Ep2Buffer
      0000F0 75 E5 00         [24]  761 	mov	((_UEP2_DMA >> 8) & 0xFF),#(_Ep2Buffer >> 8)
                                    762 ;	main.c:141: UEP2_3_MOD = 0xCC;														 //端点2/3 单缓冲收发使能
      0000F3 75 EB CC         [24]  763 	mov	_UEP2_3_MOD,#0xcc
                                    764 ;	main.c:142: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;		//端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK
      0000F6 75 D4 12         [24]  765 	mov	_UEP2_CTRL,#0x12
                                    766 ;	main.c:144: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;				//端点1自动翻转同步标志位，IN事务返回NAK
      0000F9 75 D2 12         [24]  767 	mov	_UEP1_CTRL,#0x12
                                    768 ;	main.c:145: UEP0_DMA = (uint16_t) Ep0Buffer;													  //端点0数据传输地址
      0000FC 75 EC 00         [24]  769 	mov	((_UEP0_DMA >> 0) & 0xFF),#_Ep0Buffer
      0000FF 75 ED 00         [24]  770 	mov	((_UEP0_DMA >> 8) & 0xFF),#(_Ep0Buffer >> 8)
                                    771 ;	main.c:146: UEP4_1_MOD = 0X40;														 //端点1上传缓冲区；端点0单64字节收发缓冲区
      000102 75 EA 40         [24]  772 	mov	_UEP4_1_MOD,#0x40
                                    773 ;	main.c:147: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;				//手动翻转，OUT事务返回ACK，IN事务返回NAK
      000105 75 DC 02         [24]  774 	mov	_UEP0_CTRL,#0x02
                                    775 ;	main.c:148: }
      000108 22               [24]  776 	ret
                                    777 ;------------------------------------------------------------
                                    778 ;Allocation info for local variables in function 'DeviceInterrupt'
                                    779 ;------------------------------------------------------------
                                    780 ;len                       Allocated to registers r4 r5 
                                    781 ;------------------------------------------------------------
                                    782 ;	main.c:172: void DeviceInterrupt(void) __interrupt (INT_NO_USB)					   //USB中断服务程序,使用寄存器组1
                                    783 ;	-----------------------------------------
                                    784 ;	 function DeviceInterrupt
                                    785 ;	-----------------------------------------
      000109                        786 _DeviceInterrupt:
      000109 C0 20            [24]  787 	push	bits
      00010B C0 E0            [24]  788 	push	acc
      00010D C0 F0            [24]  789 	push	b
      00010F C0 82            [24]  790 	push	dpl
      000111 C0 83            [24]  791 	push	dph
      000113 C0 07            [24]  792 	push	(0+7)
      000115 C0 06            [24]  793 	push	(0+6)
      000117 C0 05            [24]  794 	push	(0+5)
      000119 C0 04            [24]  795 	push	(0+4)
      00011B C0 03            [24]  796 	push	(0+3)
      00011D C0 02            [24]  797 	push	(0+2)
      00011F C0 01            [24]  798 	push	(0+1)
      000121 C0 00            [24]  799 	push	(0+0)
      000123 C0 D0            [24]  800 	push	psw
      000125 75 D0 00         [24]  801 	mov	psw,#0x00
                                    802 ;	main.c:175: if(UIF_TRANSFER)															//USB传输完成标志
      000128 20 D9 03         [24]  803 	jb	_UIF_TRANSFER,00484$
      00012B 02 05 A2         [24]  804 	ljmp	00207$
      00012E                        805 00484$:
                                    806 ;	main.c:177: switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
      00012E 74 3F            [12]  807 	mov	a,#0x3f
      000130 55 D9            [12]  808 	anl	a,_USB_INT_ST
      000132 FF               [12]  809 	mov	r7,a
      000133 FE               [12]  810 	mov	r6,a
      000134 BE 00 03         [24]  811 	cjne	r6,#0x00,00485$
      000137 02 05 9A         [24]  812 	ljmp	00203$
      00013A                        813 00485$:
      00013A BF 02 02         [24]  814 	cjne	r7,#0x02,00486$
      00013D 80 38            [24]  815 	sjmp	00103$
      00013F                        816 00486$:
      00013F BF 20 03         [24]  817 	cjne	r7,#0x20,00487$
      000142 02 05 21         [24]  818 	ljmp	00198$
      000145                        819 00487$:
      000145 BF 21 02         [24]  820 	cjne	r7,#0x21,00488$
      000148 80 0D            [24]  821 	sjmp	00101$
      00014A                        822 00488$:
      00014A BF 22 02         [24]  823 	cjne	r7,#0x22,00489$
      00014D 80 16            [24]  824 	sjmp	00102$
      00014F                        825 00489$:
      00014F BF 30 02         [24]  826 	cjne	r7,#0x30,00490$
      000152 80 3C            [24]  827 	sjmp	00106$
      000154                        828 00490$:
      000154 02 05 A0         [24]  829 	ljmp	00205$
                                    830 ;	main.c:179: case UIS_TOKEN_IN | 1:												  //endpoint 1# 端点中断上传
      000157                        831 00101$:
                                    832 ;	main.c:180: UEP1_T_LEN = 0;
      000157 75 D3 00         [24]  833 	mov	_UEP1_T_LEN,#0x00
                                    834 ;	main.c:181: UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;		   //默认应答NAK
      00015A 74 FC            [12]  835 	mov	a,#0xfc
      00015C 55 D2            [12]  836 	anl	a,_UEP1_CTRL
      00015E 44 02            [12]  837 	orl	a,#0x02
      000160 F5 D2            [12]  838 	mov	_UEP1_CTRL,a
                                    839 ;	main.c:182: break;
      000162 02 05 A0         [24]  840 	ljmp	00205$
                                    841 ;	main.c:183: case UIS_TOKEN_IN | 2:												  //endpoint 2# 端点批量上传
      000165                        842 00102$:
                                    843 ;	main.c:185: UEP2_T_LEN = 0;													//预使用发送长度一定要清空
      000165 75 D5 00         [24]  844 	mov	_UEP2_T_LEN,#0x00
                                    845 ;	main.c:186: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;		   //默认应答NAK
      000168 74 FC            [12]  846 	mov	a,#0xfc
      00016A 55 D4            [12]  847 	anl	a,_UEP2_CTRL
      00016C 44 02            [12]  848 	orl	a,#0x02
      00016E F5 D4            [12]  849 	mov	_UEP2_CTRL,a
                                    850 ;	main.c:187: UpPoint2_Busy = 0;												  //清除忙标志
      000170 78 71            [12]  851 	mov	r0,#_UpPoint2_Busy
      000172 76 00            [12]  852 	mov	@r0,#0x00
                                    853 ;	main.c:189: break;
      000174 02 05 A0         [24]  854 	ljmp	00205$
                                    855 ;	main.c:190: case UIS_TOKEN_OUT | 2:											 //endpoint 3# 端点批量下传
      000177                        856 00103$:
                                    857 ;	main.c:191: if ( U_TOG_OK )													 // 不同步的数据包将丢弃
      000177 20 DE 03         [24]  858 	jb	_U_TOG_OK,00491$
      00017A 02 05 A0         [24]  859 	ljmp	00205$
      00017D                        860 00491$:
                                    861 ;	main.c:193: USBByteCount = USB_RX_LEN;
      00017D 78 6F            [12]  862 	mov	r0,#_USBByteCount
      00017F A6 DB            [24]  863 	mov	@r0,_USB_RX_LEN
                                    864 ;	main.c:194: USBBufOutPoint = 0;											 //取数据指针复位
      000181 78 70            [12]  865 	mov	r0,#_USBBufOutPoint
      000183 76 00            [12]  866 	mov	@r0,#0x00
                                    867 ;	main.c:195: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_NAK;	   //收到一包数据就NAK，主函数处理完，由主函数修改响应方式
      000185 74 F3            [12]  868 	mov	a,#0xf3
      000187 55 D4            [12]  869 	anl	a,_UEP2_CTRL
      000189 44 08            [12]  870 	orl	a,#0x08
      00018B F5 D4            [12]  871 	mov	_UEP2_CTRL,a
                                    872 ;	main.c:197: break;
      00018D 02 05 A0         [24]  873 	ljmp	00205$
                                    874 ;	main.c:198: case UIS_TOKEN_SETUP | 0:												//SETUP事务
      000190                        875 00106$:
                                    876 ;	main.c:199: len = USB_RX_LEN;
      000190 AE DB            [24]  877 	mov	r6,_USB_RX_LEN
      000192 7F 00            [12]  878 	mov	r7,#0x00
                                    879 ;	main.c:200: if(len == (sizeof(USB_SETUP_REQ)))
      000194 BE 08 05         [24]  880 	cjne	r6,#0x08,00492$
      000197 BF 00 02         [24]  881 	cjne	r7,#0x00,00492$
      00019A 80 03            [24]  882 	sjmp	00493$
      00019C                        883 00492$:
      00019C 02 04 F5         [24]  884 	ljmp	00190$
      00019F                        885 00493$:
                                    886 ;	main.c:202: SetupLen = ((uint16_t)UsbSetupBuf->wLengthH<<8) | (UsbSetupBuf->wLengthL);
      00019F 90 00 07         [24]  887 	mov	dptr,#(_Ep0Buffer + 0x0007)
      0001A2 E0               [24]  888 	movx	a,@dptr
      0001A3 FE               [12]  889 	mov	r6,a
      0001A4 7F 00            [12]  890 	mov	r7,#0x00
      0001A6 90 00 06         [24]  891 	mov	dptr,#(_Ep0Buffer + 0x0006)
      0001A9 E0               [24]  892 	movx	a,@dptr
      0001AA 7C 00            [12]  893 	mov	r4,#0x00
      0001AC 4F               [12]  894 	orl	a,r7
      0001AD F5 08            [12]  895 	mov	_SetupLen,a
      0001AF EC               [12]  896 	mov	a,r4
      0001B0 4E               [12]  897 	orl	a,r6
      0001B1 F5 09            [12]  898 	mov	(_SetupLen + 1),a
                                    899 ;	main.c:203: len = 0;													  // 默认为成功并且上传0长度
      0001B3 7E 00            [12]  900 	mov	r6,#0x00
      0001B5 7F 00            [12]  901 	mov	r7,#0x00
                                    902 ;	main.c:204: SetupReq = UsbSetupBuf->bRequest;
      0001B7 90 00 01         [24]  903 	mov	dptr,#(_Ep0Buffer + 0x0001)
      0001BA E0               [24]  904 	movx	a,@dptr
      0001BB F5 0A            [12]  905 	mov	_SetupReq,a
                                    906 ;	main.c:205: if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )//非标准请求
      0001BD 90 00 00         [24]  907 	mov	dptr,#_Ep0Buffer
      0001C0 E0               [24]  908 	movx	a,@dptr
      0001C1 FD               [12]  909 	mov	r5,a
      0001C2 54 60            [12]  910 	anl	a,#0x60
      0001C4 60 07            [24]  911 	jz	00187$
                                    912 ;	main.c:211: len = 0xFF;  								 									 /*命令不支持*/
      0001C6 7E FF            [12]  913 	mov	r6,#0xff
      0001C8 7F 00            [12]  914 	mov	r7,#0x00
                                    915 ;	main.c:213: }
      0001CA 02 04 F9         [24]  916 	ljmp	00191$
      0001CD                        917 00187$:
                                    918 ;	main.c:217: switch(SetupReq)											 //请求码
      0001CD E5 0A            [12]  919 	mov	a,_SetupReq
      0001CF 24 F5            [12]  920 	add	a,#0xff - 0x0a
      0001D1 50 03            [24]  921 	jnc	00495$
      0001D3 02 04 EF         [24]  922 	ljmp	00184$
      0001D6                        923 00495$:
      0001D6 E5 0A            [12]  924 	mov	a,_SetupReq
      0001D8 24 0B            [12]  925 	add	a,#(00496$-3-.)
      0001DA 83               [24]  926 	movc	a,@a+pc
      0001DB F5 82            [12]  927 	mov	dpl,a
      0001DD E5 0A            [12]  928 	mov	a,_SetupReq
      0001DF 24 0F            [12]  929 	add	a,#(00497$-3-.)
      0001E1 83               [24]  930 	movc	a,@a+pc
      0001E2 F5 83            [12]  931 	mov	dph,a
      0001E4 E4               [12]  932 	clr	a
      0001E5 73               [24]  933 	jmp	@a+dptr
      0001E6                        934 00496$:
      0001E6 CF                     935 	.db	00180$
      0001E7 11                     936 	.db	00132$
      0001E8 EF                     937 	.db	00184$
      0001E9 BE                     938 	.db	00153$
      0001EA EF                     939 	.db	00184$
      0001EB E0                     940 	.db	00126$
      0001EC FC                     941 	.db	00109$
      0001ED EF                     942 	.db	00184$
      0001EE ED                     943 	.db	00127$
      0001EF 08                     944 	.db	00130$
      0001F0 F9                     945 	.db	00191$
      0001F1                        946 00497$:
      0001F1 04                     947 	.db	00180$>>8
      0001F2 03                     948 	.db	00132$>>8
      0001F3 04                     949 	.db	00184$>>8
      0001F4 03                     950 	.db	00153$>>8
      0001F5 04                     951 	.db	00184$>>8
      0001F6 02                     952 	.db	00126$>>8
      0001F7 01                     953 	.db	00109$>>8
      0001F8 04                     954 	.db	00184$>>8
      0001F9 02                     955 	.db	00127$>>8
      0001FA 03                     956 	.db	00130$>>8
      0001FB 04                     957 	.db	00191$>>8
                                    958 ;	main.c:219: case USB_GET_DESCRIPTOR:
      0001FC                        959 00109$:
                                    960 ;	main.c:220: switch(UsbSetupBuf->wValueH)
      0001FC 90 00 03         [24]  961 	mov	dptr,#(_Ep0Buffer + 0x0003)
      0001FF E0               [24]  962 	movx	a,@dptr
      000200 FD               [12]  963 	mov	r5,a
      000201 BD 01 02         [24]  964 	cjne	r5,#0x01,00498$
      000204 80 0A            [24]  965 	sjmp	00110$
      000206                        966 00498$:
      000206 BD 02 02         [24]  967 	cjne	r5,#0x02,00499$
      000209 80 14            [24]  968 	sjmp	00111$
      00020B                        969 00499$:
                                    970 ;	main.c:222: case 1:													   //设备描述符
      00020B BD 03 72         [24]  971 	cjne	r5,#0x03,00122$
      00020E 80 1E            [24]  972 	sjmp	00112$
      000210                        973 00110$:
                                    974 ;	main.c:223: pDescr = DevDesc;										 //把设备描述符送到要发送的缓冲区
      000210 75 0D 59         [24]  975 	mov	_pDescr,#_DevDesc
      000213 75 0E 09         [24]  976 	mov	(_pDescr + 1),#(_DevDesc >> 8)
      000216 75 0F 80         [24]  977 	mov	(_pDescr + 2),#0x80
                                    978 ;	main.c:224: len = sizeof(DevDesc);
      000219 7C 12            [12]  979 	mov	r4,#0x12
      00021B 7D 00            [12]  980 	mov	r5,#0x00
                                    981 ;	main.c:225: break;
                                    982 ;	main.c:226: case 2:														//配置描述符
      00021D 80 65            [24]  983 	sjmp	00123$
      00021F                        984 00111$:
                                    985 ;	main.c:227: pDescr = CfgDesc;										  //把设备描述符送到要发送的缓冲区
      00021F 75 0D 6B         [24]  986 	mov	_pDescr,#_CfgDesc
      000222 75 0E 09         [24]  987 	mov	(_pDescr + 1),#(_CfgDesc >> 8)
      000225 75 0F 80         [24]  988 	mov	(_pDescr + 2),#0x80
                                    989 ;	main.c:228: len = sizeof(CfgDesc);
      000228 7C 61            [12]  990 	mov	r4,#0x61
      00022A 7D 00            [12]  991 	mov	r5,#0x00
                                    992 ;	main.c:229: break;
                                    993 ;	main.c:230: case 3:
      00022C 80 56            [24]  994 	sjmp	00123$
      00022E                        995 00112$:
                                    996 ;	main.c:231: if(UsbSetupBuf->wValueL == 0)
      00022E 90 00 02         [24]  997 	mov	dptr,#(_Ep0Buffer + 0x0002)
      000231 E0               [24]  998 	movx	a,@dptr
      000232 70 0F            [24]  999 	jnz	00120$
                                   1000 ;	main.c:233: pDescr = LangDes;
      000234 75 0D CC         [24] 1001 	mov	_pDescr,#_LangDes
      000237 75 0E 09         [24] 1002 	mov	(_pDescr + 1),#(_LangDes >> 8)
      00023A 75 0F 80         [24] 1003 	mov	(_pDescr + 2),#0x80
                                   1004 ;	main.c:234: len = sizeof(LangDes);
      00023D 7C 04            [12] 1005 	mov	r4,#0x04
      00023F 7D 00            [12] 1006 	mov	r5,#0x00
      000241 80 41            [24] 1007 	sjmp	00123$
      000243                       1008 00120$:
                                   1009 ;	main.c:236: else if(UsbSetupBuf->wValueL == 1)
      000243 90 00 02         [24] 1010 	mov	dptr,#(_Ep0Buffer + 0x0002)
      000246 E0               [24] 1011 	movx	a,@dptr
      000247 FB               [12] 1012 	mov	r3,a
      000248 BB 01 0F         [24] 1013 	cjne	r3,#0x01,00117$
                                   1014 ;	main.c:238: pDescr = Manuf_Des;
      00024B 75 0D F8         [24] 1015 	mov	_pDescr,#_Manuf_Des
      00024E 75 0E 09         [24] 1016 	mov	(_pDescr + 1),#(_Manuf_Des >> 8)
      000251 75 0F 80         [24] 1017 	mov	(_pDescr + 2),#0x80
                                   1018 ;	main.c:239: len = sizeof(Manuf_Des);
      000254 7C 0A            [12] 1019 	mov	r4,#0x0a
      000256 7D 00            [12] 1020 	mov	r5,#0x00
      000258 80 2A            [24] 1021 	sjmp	00123$
      00025A                       1022 00117$:
                                   1023 ;	main.c:241: else if(UsbSetupBuf->wValueL == 2)
      00025A 90 00 02         [24] 1024 	mov	dptr,#(_Ep0Buffer + 0x0002)
      00025D E0               [24] 1025 	movx	a,@dptr
      00025E FB               [12] 1026 	mov	r3,a
      00025F BB 02 0F         [24] 1027 	cjne	r3,#0x02,00114$
                                   1028 ;	main.c:243: pDescr = Prod_Des;
      000262 75 0D E4         [24] 1029 	mov	_pDescr,#_Prod_Des
      000265 75 0E 09         [24] 1030 	mov	(_pDescr + 1),#(_Prod_Des >> 8)
      000268 75 0F 80         [24] 1031 	mov	(_pDescr + 2),#0x80
                                   1032 ;	main.c:244: len = sizeof(Prod_Des);
      00026B 7C 14            [12] 1033 	mov	r4,#0x14
      00026D 7D 00            [12] 1034 	mov	r5,#0x00
      00026F 80 13            [24] 1035 	sjmp	00123$
      000271                       1036 00114$:
                                   1037 ;	main.c:248: pDescr = SerDes;
      000271 75 0D D0         [24] 1038 	mov	_pDescr,#_SerDes
      000274 75 0E 09         [24] 1039 	mov	(_pDescr + 1),#(_SerDes >> 8)
      000277 75 0F 80         [24] 1040 	mov	(_pDescr + 2),#0x80
                                   1041 ;	main.c:249: len = sizeof(SerDes);
      00027A 7C 14            [12] 1042 	mov	r4,#0x14
      00027C 7D 00            [12] 1043 	mov	r5,#0x00
                                   1044 ;	main.c:251: break;
                                   1045 ;	main.c:252: default:
      00027E 80 04            [24] 1046 	sjmp	00123$
      000280                       1047 00122$:
                                   1048 ;	main.c:253: len = 0xff;												//不支持的命令或者出错
      000280 7C FF            [12] 1049 	mov	r4,#0xff
      000282 7D 00            [12] 1050 	mov	r5,#0x00
                                   1051 ;	main.c:255: }
      000284                       1052 00123$:
                                   1053 ;	main.c:256: if ( SetupLen > len )
      000284 C3               [12] 1054 	clr	c
      000285 EC               [12] 1055 	mov	a,r4
      000286 95 08            [12] 1056 	subb	a,_SetupLen
      000288 ED               [12] 1057 	mov	a,r5
      000289 95 09            [12] 1058 	subb	a,(_SetupLen + 1)
      00028B 50 04            [24] 1059 	jnc	00125$
                                   1060 ;	main.c:258: SetupLen = len;	//限制总长度
      00028D 8C 08            [24] 1061 	mov	_SetupLen,r4
      00028F 8D 09            [24] 1062 	mov	(_SetupLen + 1),r5
      000291                       1063 00125$:
                                   1064 ;	main.c:260: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;							//本次传输长度
      000291 C3               [12] 1065 	clr	c
      000292 E5 08            [12] 1066 	mov	a,_SetupLen
      000294 94 08            [12] 1067 	subb	a,#0x08
      000296 E5 09            [12] 1068 	mov	a,(_SetupLen + 1)
      000298 94 00            [12] 1069 	subb	a,#0x00
      00029A 40 06            [24] 1070 	jc	00220$
      00029C 7C 08            [12] 1071 	mov	r4,#0x08
      00029E 7D 00            [12] 1072 	mov	r5,#0x00
      0002A0 80 04            [24] 1073 	sjmp	00221$
      0002A2                       1074 00220$:
      0002A2 AC 08            [24] 1075 	mov	r4,_SetupLen
      0002A4 AD 09            [24] 1076 	mov	r5,(_SetupLen + 1)
      0002A6                       1077 00221$:
      0002A6 8C 06            [24] 1078 	mov	ar6,r4
      0002A8 8D 07            [24] 1079 	mov	ar7,r5
                                   1080 ;	main.c:261: memcpy(Ep0Buffer,pDescr,len);								  //加载上传数据
      0002AA 85 0D 21         [24] 1081 	mov	___memcpy_PARM_2,_pDescr
      0002AD 85 0E 22         [24] 1082 	mov	(___memcpy_PARM_2 + 1),(_pDescr + 1)
      0002B0 85 0F 23         [24] 1083 	mov	(___memcpy_PARM_2 + 2),(_pDescr + 2)
      0002B3 8E 24            [24] 1084 	mov	___memcpy_PARM_3,r6
      0002B5 8F 25            [24] 1085 	mov	(___memcpy_PARM_3 + 1),r7
      0002B7 90 00 00         [24] 1086 	mov	dptr,#_Ep0Buffer
      0002BA 75 F0 00         [24] 1087 	mov	b,#0x00
      0002BD C0 07            [24] 1088 	push	ar7
      0002BF C0 06            [24] 1089 	push	ar6
      0002C1 12 08 C9         [24] 1090 	lcall	___memcpy
      0002C4 D0 06            [24] 1091 	pop	ar6
      0002C6 D0 07            [24] 1092 	pop	ar7
                                   1093 ;	main.c:262: SetupLen -= len;
      0002C8 E5 08            [12] 1094 	mov	a,_SetupLen
      0002CA C3               [12] 1095 	clr	c
      0002CB 9E               [12] 1096 	subb	a,r6
      0002CC F5 08            [12] 1097 	mov	_SetupLen,a
      0002CE E5 09            [12] 1098 	mov	a,(_SetupLen + 1)
      0002D0 9F               [12] 1099 	subb	a,r7
      0002D1 F5 09            [12] 1100 	mov	(_SetupLen + 1),a
                                   1101 ;	main.c:263: pDescr += len;
      0002D3 EE               [12] 1102 	mov	a,r6
      0002D4 25 0D            [12] 1103 	add	a,_pDescr
      0002D6 F5 0D            [12] 1104 	mov	_pDescr,a
      0002D8 EF               [12] 1105 	mov	a,r7
      0002D9 35 0E            [12] 1106 	addc	a,(_pDescr + 1)
      0002DB F5 0E            [12] 1107 	mov	(_pDescr + 1),a
                                   1108 ;	main.c:264: break;
      0002DD 02 04 F9         [24] 1109 	ljmp	00191$
                                   1110 ;	main.c:265: case USB_SET_ADDRESS:
      0002E0                       1111 00126$:
                                   1112 ;	main.c:266: SetupLen = UsbSetupBuf->wValueL;							  //暂存USB设备地址
      0002E0 90 00 02         [24] 1113 	mov	dptr,#(_Ep0Buffer + 0x0002)
      0002E3 E0               [24] 1114 	movx	a,@dptr
      0002E4 FD               [12] 1115 	mov	r5,a
      0002E5 8D 08            [24] 1116 	mov	_SetupLen,r5
      0002E7 75 09 00         [24] 1117 	mov	(_SetupLen + 1),#0x00
                                   1118 ;	main.c:267: break;
      0002EA 02 04 F9         [24] 1119 	ljmp	00191$
                                   1120 ;	main.c:268: case USB_GET_CONFIGURATION:
      0002ED                       1121 00127$:
                                   1122 ;	main.c:269: Ep0Buffer[0] = UsbConfig;
      0002ED 90 00 00         [24] 1123 	mov	dptr,#_Ep0Buffer
      0002F0 E5 0C            [12] 1124 	mov	a,_UsbConfig
      0002F2 F0               [24] 1125 	movx	@dptr,a
                                   1126 ;	main.c:270: if ( SetupLen >= 1 )
      0002F3 C3               [12] 1127 	clr	c
      0002F4 E5 08            [12] 1128 	mov	a,_SetupLen
      0002F6 94 01            [12] 1129 	subb	a,#0x01
      0002F8 E5 09            [12] 1130 	mov	a,(_SetupLen + 1)
      0002FA 94 00            [12] 1131 	subb	a,#0x00
      0002FC 50 03            [24] 1132 	jnc	00508$
      0002FE 02 04 F9         [24] 1133 	ljmp	00191$
      000301                       1134 00508$:
                                   1135 ;	main.c:272: len = 1;
      000301 7E 01            [12] 1136 	mov	r6,#0x01
      000303 7F 00            [12] 1137 	mov	r7,#0x00
                                   1138 ;	main.c:274: break;
      000305 02 04 F9         [24] 1139 	ljmp	00191$
                                   1140 ;	main.c:275: case USB_SET_CONFIGURATION:
      000308                       1141 00130$:
                                   1142 ;	main.c:276: UsbConfig = UsbSetupBuf->wValueL;
      000308 90 00 02         [24] 1143 	mov	dptr,#(_Ep0Buffer + 0x0002)
      00030B E0               [24] 1144 	movx	a,@dptr
      00030C F5 0C            [12] 1145 	mov	_UsbConfig,a
                                   1146 ;	main.c:277: break;
      00030E 02 04 F9         [24] 1147 	ljmp	00191$
                                   1148 ;	main.c:280: case USB_CLEAR_FEATURE:											//Clear Feature
      000311                       1149 00132$:
                                   1150 ;	main.c:281: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )				  /* 清除设备 */
      000311 90 00 00         [24] 1151 	mov	dptr,#_Ep0Buffer
      000314 E0               [24] 1152 	movx	a,@dptr
      000315 54 1F            [12] 1153 	anl	a,#0x1f
      000317 70 33            [24] 1154 	jnz	00151$
                                   1155 ;	main.c:283: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
      000319 90 00 03         [24] 1156 	mov	dptr,#(_Ep0Buffer + 0x0003)
      00031C E0               [24] 1157 	movx	a,@dptr
      00031D FC               [12] 1158 	mov	r4,a
      00031E 7D 00            [12] 1159 	mov	r5,#0x00
      000320 90 00 02         [24] 1160 	mov	dptr,#(_Ep0Buffer + 0x0002)
      000323 E0               [24] 1161 	movx	a,@dptr
      000324 FB               [12] 1162 	mov	r3,a
      000325 7A 00            [12] 1163 	mov	r2,#0x00
      000327 42 05            [12] 1164 	orl	ar5,a
      000329 EA               [12] 1165 	mov	a,r2
      00032A 42 04            [12] 1166 	orl	ar4,a
      00032C BD 01 16         [24] 1167 	cjne	r5,#0x01,00137$
      00032F BC 00 13         [24] 1168 	cjne	r4,#0x00,00137$
                                   1169 ;	main.c:285: if( CfgDesc[ 7 ] & 0x20 )
      000332 90 09 72         [24] 1170 	mov	dptr,#(_CfgDesc + 0x0007)
      000335 E4               [12] 1171 	clr	a
      000336 93               [24] 1172 	movc	a,@a+dptr
      000337 FD               [12] 1173 	mov	r5,a
      000338 30 E5 03         [24] 1174 	jnb	acc.5,00513$
      00033B 02 04 F9         [24] 1175 	ljmp	00191$
      00033E                       1176 00513$:
                                   1177 ;	main.c:291: len = 0xFF;										/* 操作失败 */
      00033E 7E FF            [12] 1178 	mov	r6,#0xff
      000340 7F 00            [12] 1179 	mov	r7,#0x00
      000342 02 04 F9         [24] 1180 	ljmp	00191$
      000345                       1181 00137$:
                                   1182 ;	main.c:296: len = 0xFF;											/* 操作失败 */
      000345 7E FF            [12] 1183 	mov	r6,#0xff
      000347 7F 00            [12] 1184 	mov	r7,#0x00
      000349 02 04 F9         [24] 1185 	ljmp	00191$
      00034C                       1186 00151$:
                                   1187 ;	main.c:299: else if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
      00034C 90 00 00         [24] 1188 	mov	dptr,#_Ep0Buffer
      00034F E0               [24] 1189 	movx	a,@dptr
      000350 FD               [12] 1190 	mov	r5,a
      000351 53 05 1F         [24] 1191 	anl	ar5,#0x1f
      000354 7C 00            [12] 1192 	mov	r4,#0x00
      000356 BD 02 5E         [24] 1193 	cjne	r5,#0x02,00148$
      000359 BC 00 5B         [24] 1194 	cjne	r4,#0x00,00148$
                                   1195 ;	main.c:301: switch( UsbSetupBuf->wIndexL )
      00035C 90 00 04         [24] 1196 	mov	dptr,#(_Ep0Buffer + 0x0004)
      00035F E0               [24] 1197 	movx	a,@dptr
      000360 FD               [12] 1198 	mov	r5,a
      000361 BD 01 02         [24] 1199 	cjne	r5,#0x01,00516$
      000364 80 44            [24] 1200 	sjmp	00144$
      000366                       1201 00516$:
      000366 BD 02 02         [24] 1202 	cjne	r5,#0x02,00517$
      000369 80 2E            [24] 1203 	sjmp	00142$
      00036B                       1204 00517$:
      00036B BD 03 02         [24] 1205 	cjne	r5,#0x03,00518$
      00036E 80 18            [24] 1206 	sjmp	00140$
      000370                       1207 00518$:
      000370 BD 81 02         [24] 1208 	cjne	r5,#0x81,00519$
      000373 80 2A            [24] 1209 	sjmp	00143$
      000375                       1210 00519$:
      000375 BD 82 02         [24] 1211 	cjne	r5,#0x82,00520$
      000378 80 14            [24] 1212 	sjmp	00141$
      00037A                       1213 00520$:
      00037A BD 83 33         [24] 1214 	cjne	r5,#0x83,00145$
                                   1215 ;	main.c:304: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
      00037D 74 BC            [12] 1216 	mov	a,#0xbc
      00037F 55 D6            [12] 1217 	anl	a,_UEP3_CTRL
      000381 44 02            [12] 1218 	orl	a,#0x02
      000383 F5 D6            [12] 1219 	mov	_UEP3_CTRL,a
                                   1220 ;	main.c:305: break;
      000385 02 04 F9         [24] 1221 	ljmp	00191$
                                   1222 ;	main.c:306: case 0x03:
      000388                       1223 00140$:
                                   1224 ;	main.c:307: UEP3_CTRL = UEP3_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
      000388 53 D6 73         [24] 1225 	anl	_UEP3_CTRL,#0x73
                                   1226 ;	main.c:308: break;
      00038B 02 04 F9         [24] 1227 	ljmp	00191$
                                   1228 ;	main.c:309: case 0x82:
      00038E                       1229 00141$:
                                   1230 ;	main.c:310: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
      00038E 74 BC            [12] 1231 	mov	a,#0xbc
      000390 55 D4            [12] 1232 	anl	a,_UEP2_CTRL
      000392 44 02            [12] 1233 	orl	a,#0x02
      000394 F5 D4            [12] 1234 	mov	_UEP2_CTRL,a
                                   1235 ;	main.c:311: break;
      000396 02 04 F9         [24] 1236 	ljmp	00191$
                                   1237 ;	main.c:312: case 0x02:
      000399                       1238 00142$:
                                   1239 ;	main.c:313: UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
      000399 53 D4 73         [24] 1240 	anl	_UEP2_CTRL,#0x73
                                   1241 ;	main.c:314: break;
      00039C 02 04 F9         [24] 1242 	ljmp	00191$
                                   1243 ;	main.c:315: case 0x81:
      00039F                       1244 00143$:
                                   1245 ;	main.c:316: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
      00039F 74 BC            [12] 1246 	mov	a,#0xbc
      0003A1 55 D2            [12] 1247 	anl	a,_UEP1_CTRL
      0003A3 44 02            [12] 1248 	orl	a,#0x02
      0003A5 F5 D2            [12] 1249 	mov	_UEP1_CTRL,a
                                   1250 ;	main.c:317: break;
      0003A7 02 04 F9         [24] 1251 	ljmp	00191$
                                   1252 ;	main.c:318: case 0x01:
      0003AA                       1253 00144$:
                                   1254 ;	main.c:319: UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
      0003AA 53 D2 73         [24] 1255 	anl	_UEP1_CTRL,#0x73
                                   1256 ;	main.c:320: break;
      0003AD 02 04 F9         [24] 1257 	ljmp	00191$
                                   1258 ;	main.c:321: default:
      0003B0                       1259 00145$:
                                   1260 ;	main.c:322: len = 0xFF;										 // 不支持的端点
      0003B0 7E FF            [12] 1261 	mov	r6,#0xff
      0003B2 7F 00            [12] 1262 	mov	r7,#0x00
                                   1263 ;	main.c:324: }
      0003B4 02 04 F9         [24] 1264 	ljmp	00191$
      0003B7                       1265 00148$:
                                   1266 ;	main.c:328: len = 0xFF;												// 不是端点不支持
      0003B7 7E FF            [12] 1267 	mov	r6,#0xff
      0003B9 7F 00            [12] 1268 	mov	r7,#0x00
                                   1269 ;	main.c:330: break;
      0003BB 02 04 F9         [24] 1270 	ljmp	00191$
                                   1271 ;	main.c:331: case USB_SET_FEATURE:										  /* Set Feature */
      0003BE                       1272 00153$:
                                   1273 ;	main.c:332: if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_DEVICE )				  /* 设置设备 */
      0003BE 90 00 00         [24] 1274 	mov	dptr,#_Ep0Buffer
      0003C1 E0               [24] 1275 	movx	a,@dptr
      0003C2 54 1F            [12] 1276 	anl	a,#0x1f
      0003C4 70 4D            [24] 1277 	jnz	00178$
                                   1278 ;	main.c:334: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
      0003C6 90 00 03         [24] 1279 	mov	dptr,#(_Ep0Buffer + 0x0003)
      0003C9 E0               [24] 1280 	movx	a,@dptr
      0003CA FC               [12] 1281 	mov	r4,a
      0003CB 7D 00            [12] 1282 	mov	r5,#0x00
      0003CD 90 00 02         [24] 1283 	mov	dptr,#(_Ep0Buffer + 0x0002)
      0003D0 E0               [24] 1284 	movx	a,@dptr
      0003D1 FB               [12] 1285 	mov	r3,a
      0003D2 7A 00            [12] 1286 	mov	r2,#0x00
      0003D4 42 05            [12] 1287 	orl	ar5,a
      0003D6 EA               [12] 1288 	mov	a,r2
      0003D7 42 04            [12] 1289 	orl	ar4,a
      0003D9 BD 01 30         [24] 1290 	cjne	r5,#0x01,00161$
      0003DC BC 00 2D         [24] 1291 	cjne	r4,#0x00,00161$
                                   1292 ;	main.c:336: if( CfgDesc[ 7 ] & 0x20 )
      0003DF 90 09 72         [24] 1293 	mov	dptr,#(_CfgDesc + 0x0007)
      0003E2 E4               [12] 1294 	clr	a
      0003E3 93               [24] 1295 	movc	a,@a+dptr
      0003E4 FD               [12] 1296 	mov	r5,a
      0003E5 30 E5 1D         [24] 1297 	jnb	acc.5,00158$
                                   1298 ;	main.c:342: while ( XBUS_AUX & bUART0_TX )
      0003E8                       1299 00154$:
      0003E8 E5 A2            [12] 1300 	mov	a,_XBUS_AUX
      0003EA 20 E7 FB         [24] 1301 	jb	acc.7,00154$
                                   1302 ;	main.c:346: SAFE_MOD = 0x55;
      0003ED 75 A1 55         [24] 1303 	mov	_SAFE_MOD,#0x55
                                   1304 ;	main.c:347: SAFE_MOD = 0xAA;
      0003F0 75 A1 AA         [24] 1305 	mov	_SAFE_MOD,#0xaa
                                   1306 ;	main.c:348: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;					  //USB或者RXD0/1有信号时可被唤醒
      0003F3 75 A9 C1         [24] 1307 	mov	_WAKE_CTRL,#0xc1
                                   1308 ;	main.c:349: PCON |= PD;																 //睡眠
      0003F6 43 87 02         [24] 1309 	orl	_PCON,#0x02
                                   1310 ;	main.c:350: SAFE_MOD = 0x55;
      0003F9 75 A1 55         [24] 1311 	mov	_SAFE_MOD,#0x55
                                   1312 ;	main.c:351: SAFE_MOD = 0xAA;
      0003FC 75 A1 AA         [24] 1313 	mov	_SAFE_MOD,#0xaa
                                   1314 ;	main.c:352: WAKE_CTRL = 0x00;
      0003FF 75 A9 00         [24] 1315 	mov	_WAKE_CTRL,#0x00
      000402 02 04 F9         [24] 1316 	ljmp	00191$
      000405                       1317 00158$:
                                   1318 ;	main.c:356: len = 0xFF;										/* 操作失败 */
      000405 7E FF            [12] 1319 	mov	r6,#0xff
      000407 7F 00            [12] 1320 	mov	r7,#0x00
      000409 02 04 F9         [24] 1321 	ljmp	00191$
      00040C                       1322 00161$:
                                   1323 ;	main.c:361: len = 0xFF;											/* 操作失败 */
      00040C 7E FF            [12] 1324 	mov	r6,#0xff
      00040E 7F 00            [12] 1325 	mov	r7,#0x00
      000410 02 04 F9         [24] 1326 	ljmp	00191$
      000413                       1327 00178$:
                                   1328 ;	main.c:364: else if( ( UsbSetupBuf->bRequestType & 0x1F ) == USB_REQ_RECIP_ENDP )			 /* 设置端点 */
      000413 90 00 00         [24] 1329 	mov	dptr,#_Ep0Buffer
      000416 E0               [24] 1330 	movx	a,@dptr
      000417 FD               [12] 1331 	mov	r5,a
      000418 53 05 1F         [24] 1332 	anl	ar5,#0x1f
      00041B 7C 00            [12] 1333 	mov	r4,#0x00
      00041D BD 02 05         [24] 1334 	cjne	r5,#0x02,00529$
      000420 BC 00 02         [24] 1335 	cjne	r4,#0x00,00529$
      000423 80 03            [24] 1336 	sjmp	00530$
      000425                       1337 00529$:
      000425 02 04 C9         [24] 1338 	ljmp	00175$
      000428                       1339 00530$:
                                   1340 ;	main.c:366: if( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
      000428 90 00 03         [24] 1341 	mov	dptr,#(_Ep0Buffer + 0x0003)
      00042B E0               [24] 1342 	movx	a,@dptr
      00042C FC               [12] 1343 	mov	r4,a
      00042D 7D 00            [12] 1344 	mov	r5,#0x00
      00042F 90 00 02         [24] 1345 	mov	dptr,#(_Ep0Buffer + 0x0002)
      000432 E0               [24] 1346 	movx	a,@dptr
      000433 FB               [12] 1347 	mov	r3,a
      000434 7A 00            [12] 1348 	mov	r2,#0x00
      000436 42 05            [12] 1349 	orl	ar5,a
      000438 EA               [12] 1350 	mov	a,r2
      000439 42 04            [12] 1351 	orl	ar4,a
      00043B ED               [12] 1352 	mov	a,r5
      00043C 4C               [12] 1353 	orl	a,r4
      00043D 60 03            [24] 1354 	jz	00531$
      00043F 02 04 C3         [24] 1355 	ljmp	00172$
      000442                       1356 00531$:
                                   1357 ;	main.c:368: switch( ( ( uint16_t )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
      000442 90 00 05         [24] 1358 	mov	dptr,#(_Ep0Buffer + 0x0005)
      000445 E0               [24] 1359 	movx	a,@dptr
      000446 FC               [12] 1360 	mov	r4,a
      000447 7D 00            [12] 1361 	mov	r5,#0x00
      000449 90 00 04         [24] 1362 	mov	dptr,#(_Ep0Buffer + 0x0004)
      00044C E0               [24] 1363 	movx	a,@dptr
      00044D FB               [12] 1364 	mov	r3,a
      00044E 7A 00            [12] 1365 	mov	r2,#0x00
      000450 42 05            [12] 1366 	orl	ar5,a
      000452 EA               [12] 1367 	mov	a,r2
      000453 42 04            [12] 1368 	orl	ar4,a
      000455 BD 01 05         [24] 1369 	cjne	r5,#0x01,00532$
      000458 BC 00 02         [24] 1370 	cjne	r4,#0x00,00532$
      00045B 80 58            [24] 1371 	sjmp	00168$
      00045D                       1372 00532$:
      00045D BD 02 05         [24] 1373 	cjne	r5,#0x02,00533$
      000460 BC 00 02         [24] 1374 	cjne	r4,#0x00,00533$
      000463 80 3C            [24] 1375 	sjmp	00166$
      000465                       1376 00533$:
      000465 BD 03 05         [24] 1377 	cjne	r5,#0x03,00534$
      000468 BC 00 02         [24] 1378 	cjne	r4,#0x00,00534$
      00046B 80 20            [24] 1379 	sjmp	00164$
      00046D                       1380 00534$:
      00046D BD 81 05         [24] 1381 	cjne	r5,#0x81,00535$
      000470 BC 00 02         [24] 1382 	cjne	r4,#0x00,00535$
      000473 80 36            [24] 1383 	sjmp	00167$
      000475                       1384 00535$:
      000475 BD 82 05         [24] 1385 	cjne	r5,#0x82,00536$
      000478 BC 00 02         [24] 1386 	cjne	r4,#0x00,00536$
      00047B 80 1A            [24] 1387 	sjmp	00165$
      00047D                       1388 00536$:
      00047D BD 83 3D         [24] 1389 	cjne	r5,#0x83,00169$
      000480 BC 00 3A         [24] 1390 	cjne	r4,#0x00,00169$
                                   1391 ;	main.c:371: UEP3_CTRL = UEP3_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点3 IN STALL */
      000483 74 BF            [12] 1392 	mov	a,#0xbf
      000485 55 D6            [12] 1393 	anl	a,_UEP3_CTRL
      000487 44 03            [12] 1394 	orl	a,#0x03
      000489 F5 D6            [12] 1395 	mov	_UEP3_CTRL,a
                                   1396 ;	main.c:372: break;
                                   1397 ;	main.c:373: case 0x03:
      00048B 80 6C            [24] 1398 	sjmp	00191$
      00048D                       1399 00164$:
                                   1400 ;	main.c:374: UEP3_CTRL = UEP3_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点3 OUT Stall */
      00048D 74 7F            [12] 1401 	mov	a,#0x7f
      00048F 55 D6            [12] 1402 	anl	a,_UEP3_CTRL
      000491 44 0C            [12] 1403 	orl	a,#0x0c
      000493 F5 D6            [12] 1404 	mov	_UEP3_CTRL,a
                                   1405 ;	main.c:375: break;
                                   1406 ;	main.c:376: case 0x82:
      000495 80 62            [24] 1407 	sjmp	00191$
      000497                       1408 00165$:
                                   1409 ;	main.c:377: UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
      000497 74 BF            [12] 1410 	mov	a,#0xbf
      000499 55 D4            [12] 1411 	anl	a,_UEP2_CTRL
      00049B 44 03            [12] 1412 	orl	a,#0x03
      00049D F5 D4            [12] 1413 	mov	_UEP2_CTRL,a
                                   1414 ;	main.c:378: break;
                                   1415 ;	main.c:379: case 0x02:
      00049F 80 58            [24] 1416 	sjmp	00191$
      0004A1                       1417 00166$:
                                   1418 ;	main.c:380: UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
      0004A1 74 7F            [12] 1419 	mov	a,#0x7f
      0004A3 55 D4            [12] 1420 	anl	a,_UEP2_CTRL
      0004A5 44 0C            [12] 1421 	orl	a,#0x0c
      0004A7 F5 D4            [12] 1422 	mov	_UEP2_CTRL,a
                                   1423 ;	main.c:381: break;
                                   1424 ;	main.c:382: case 0x81:
      0004A9 80 4E            [24] 1425 	sjmp	00191$
      0004AB                       1426 00167$:
                                   1427 ;	main.c:383: UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
      0004AB 74 BF            [12] 1428 	mov	a,#0xbf
      0004AD 55 D2            [12] 1429 	anl	a,_UEP1_CTRL
      0004AF 44 03            [12] 1430 	orl	a,#0x03
      0004B1 F5 D2            [12] 1431 	mov	_UEP1_CTRL,a
                                   1432 ;	main.c:384: break;
                                   1433 ;	main.c:385: case 0x01:
      0004B3 80 44            [24] 1434 	sjmp	00191$
      0004B5                       1435 00168$:
                                   1436 ;	main.c:386: UEP1_CTRL = UEP1_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点1 OUT Stall */
      0004B5 74 7F            [12] 1437 	mov	a,#0x7f
      0004B7 55 D2            [12] 1438 	anl	a,_UEP1_CTRL
      0004B9 44 0C            [12] 1439 	orl	a,#0x0c
      0004BB F5 D2            [12] 1440 	mov	_UEP1_CTRL,a
                                   1441 ;	main.c:387: default:
      0004BD                       1442 00169$:
                                   1443 ;	main.c:388: len = 0xFF;									/* 操作失败 */
      0004BD 7E FF            [12] 1444 	mov	r6,#0xff
      0004BF 7F 00            [12] 1445 	mov	r7,#0x00
                                   1446 ;	main.c:390: }
      0004C1 80 36            [24] 1447 	sjmp	00191$
      0004C3                       1448 00172$:
                                   1449 ;	main.c:394: len = 0xFF;									  /* 操作失败 */
      0004C3 7E FF            [12] 1450 	mov	r6,#0xff
      0004C5 7F 00            [12] 1451 	mov	r7,#0x00
      0004C7 80 30            [24] 1452 	sjmp	00191$
      0004C9                       1453 00175$:
                                   1454 ;	main.c:399: len = 0xFF;										  /* 操作失败 */
      0004C9 7E FF            [12] 1455 	mov	r6,#0xff
      0004CB 7F 00            [12] 1456 	mov	r7,#0x00
                                   1457 ;	main.c:401: break;
                                   1458 ;	main.c:402: case USB_GET_STATUS:
      0004CD 80 2A            [24] 1459 	sjmp	00191$
      0004CF                       1460 00180$:
                                   1461 ;	main.c:403: Ep0Buffer[0] = 0x00;
      0004CF 90 00 00         [24] 1462 	mov	dptr,#_Ep0Buffer
      0004D2 E4               [12] 1463 	clr	a
      0004D3 F0               [24] 1464 	movx	@dptr,a
                                   1465 ;	main.c:404: Ep0Buffer[1] = 0x00;
      0004D4 90 00 01         [24] 1466 	mov	dptr,#(_Ep0Buffer + 0x0001)
      0004D7 F0               [24] 1467 	movx	@dptr,a
                                   1468 ;	main.c:405: if ( SetupLen >= 2 )
      0004D8 C3               [12] 1469 	clr	c
      0004D9 E5 08            [12] 1470 	mov	a,_SetupLen
      0004DB 94 02            [12] 1471 	subb	a,#0x02
      0004DD E5 09            [12] 1472 	mov	a,(_SetupLen + 1)
      0004DF 94 00            [12] 1473 	subb	a,#0x00
      0004E1 40 06            [24] 1474 	jc	00182$
                                   1475 ;	main.c:407: len = 2;
      0004E3 7E 02            [12] 1476 	mov	r6,#0x02
      0004E5 7F 00            [12] 1477 	mov	r7,#0x00
      0004E7 80 10            [24] 1478 	sjmp	00191$
      0004E9                       1479 00182$:
                                   1480 ;	main.c:411: len = SetupLen;
      0004E9 AE 08            [24] 1481 	mov	r6,_SetupLen
      0004EB AF 09            [24] 1482 	mov	r7,(_SetupLen + 1)
                                   1483 ;	main.c:413: break;
                                   1484 ;	main.c:414: default:
      0004ED 80 0A            [24] 1485 	sjmp	00191$
      0004EF                       1486 00184$:
                                   1487 ;	main.c:415: len = 0xff;													//操作失败
      0004EF 7E FF            [12] 1488 	mov	r6,#0xff
      0004F1 7F 00            [12] 1489 	mov	r7,#0x00
                                   1490 ;	main.c:417: }
      0004F3 80 04            [24] 1491 	sjmp	00191$
      0004F5                       1492 00190$:
                                   1493 ;	main.c:422: len = 0xff;														 //包长度错误
      0004F5 7E FF            [12] 1494 	mov	r6,#0xff
      0004F7 7F 00            [12] 1495 	mov	r7,#0x00
      0004F9                       1496 00191$:
                                   1497 ;	main.c:424: if(len == 0xff)
      0004F9 BE FF 0C         [24] 1498 	cjne	r6,#0xff,00196$
      0004FC BF 00 09         [24] 1499 	cjne	r7,#0x00,00196$
                                   1500 ;	main.c:426: SetupReq = 0xFF;
      0004FF 75 0A FF         [24] 1501 	mov	_SetupReq,#0xff
                                   1502 ;	main.c:427: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
      000502 75 DC CF         [24] 1503 	mov	_UEP0_CTRL,#0xcf
      000505 02 05 A0         [24] 1504 	ljmp	00205$
      000508                       1505 00196$:
                                   1506 ;	main.c:429: else if(len <= DEFAULT_ENDP0_SIZE)													   //上传数据或者状态阶段返回0长度包
      000508 C3               [12] 1507 	clr	c
      000509 74 08            [12] 1508 	mov	a,#0x08
      00050B 9E               [12] 1509 	subb	a,r6
      00050C E4               [12] 1510 	clr	a
      00050D 9F               [12] 1511 	subb	a,r7
      00050E 40 08            [24] 1512 	jc	00193$
                                   1513 ;	main.c:431: UEP0_T_LEN = len;
      000510 8E DD            [24] 1514 	mov	_UEP0_T_LEN,r6
                                   1515 ;	main.c:432: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
      000512 75 DC C0         [24] 1516 	mov	_UEP0_CTRL,#0xc0
      000515 02 05 A0         [24] 1517 	ljmp	00205$
      000518                       1518 00193$:
                                   1519 ;	main.c:436: UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
      000518 75 DD 00         [24] 1520 	mov	_UEP0_T_LEN,#0x00
                                   1521 ;	main.c:437: UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
      00051B 75 DC C0         [24] 1522 	mov	_UEP0_CTRL,#0xc0
                                   1523 ;	main.c:439: break;
      00051E 02 05 A0         [24] 1524 	ljmp	00205$
                                   1525 ;	main.c:440: case UIS_TOKEN_IN | 0:													  //endpoint0 IN
      000521                       1526 00198$:
                                   1527 ;	main.c:441: switch(SetupReq)
      000521 74 05            [12] 1528 	mov	a,#0x05
      000523 B5 0A 02         [24] 1529 	cjne	a,_SetupReq,00543$
      000526 80 5A            [24] 1530 	sjmp	00200$
      000528                       1531 00543$:
      000528 74 06            [12] 1532 	mov	a,#0x06
      00052A B5 0A 65         [24] 1533 	cjne	a,_SetupReq,00201$
                                   1534 ;	main.c:444: len = SetupLen >= DEFAULT_ENDP0_SIZE ? DEFAULT_ENDP0_SIZE : SetupLen;								 //本次传输长度
      00052D C3               [12] 1535 	clr	c
      00052E E5 08            [12] 1536 	mov	a,_SetupLen
      000530 94 08            [12] 1537 	subb	a,#0x08
      000532 E5 09            [12] 1538 	mov	a,(_SetupLen + 1)
      000534 94 00            [12] 1539 	subb	a,#0x00
      000536 40 06            [24] 1540 	jc	00222$
      000538 7E 08            [12] 1541 	mov	r6,#0x08
      00053A 7F 00            [12] 1542 	mov	r7,#0x00
      00053C 80 04            [24] 1543 	sjmp	00223$
      00053E                       1544 00222$:
      00053E AE 08            [24] 1545 	mov	r6,_SetupLen
      000540 AF 09            [24] 1546 	mov	r7,(_SetupLen + 1)
      000542                       1547 00223$:
                                   1548 ;	main.c:445: memcpy( Ep0Buffer, pDescr, len );								   //加载上传数据
      000542 85 0D 21         [24] 1549 	mov	___memcpy_PARM_2,_pDescr
      000545 85 0E 22         [24] 1550 	mov	(___memcpy_PARM_2 + 1),(_pDescr + 1)
      000548 85 0F 23         [24] 1551 	mov	(___memcpy_PARM_2 + 2),(_pDescr + 2)
      00054B 8E 24            [24] 1552 	mov	___memcpy_PARM_3,r6
      00054D 8F 25            [24] 1553 	mov	(___memcpy_PARM_3 + 1),r7
      00054F 90 00 00         [24] 1554 	mov	dptr,#_Ep0Buffer
      000552 75 F0 00         [24] 1555 	mov	b,#0x00
      000555 C0 07            [24] 1556 	push	ar7
      000557 C0 06            [24] 1557 	push	ar6
      000559 12 08 C9         [24] 1558 	lcall	___memcpy
      00055C D0 06            [24] 1559 	pop	ar6
      00055E D0 07            [24] 1560 	pop	ar7
                                   1561 ;	main.c:446: SetupLen -= len;
      000560 E5 08            [12] 1562 	mov	a,_SetupLen
      000562 C3               [12] 1563 	clr	c
      000563 9E               [12] 1564 	subb	a,r6
      000564 F5 08            [12] 1565 	mov	_SetupLen,a
      000566 E5 09            [12] 1566 	mov	a,(_SetupLen + 1)
      000568 9F               [12] 1567 	subb	a,r7
      000569 F5 09            [12] 1568 	mov	(_SetupLen + 1),a
                                   1569 ;	main.c:447: pDescr += len;
      00056B EE               [12] 1570 	mov	a,r6
      00056C 25 0D            [12] 1571 	add	a,_pDescr
      00056E F5 0D            [12] 1572 	mov	_pDescr,a
      000570 EF               [12] 1573 	mov	a,r7
      000571 35 0E            [12] 1574 	addc	a,(_pDescr + 1)
      000573 F5 0E            [12] 1575 	mov	(_pDescr + 1),a
                                   1576 ;	main.c:448: UEP0_T_LEN = len;
      000575 8E DD            [24] 1577 	mov	_UEP0_T_LEN,r6
                                   1578 ;	main.c:449: UEP0_CTRL ^= bUEP_T_TOG;											 //同步标志位翻转
      000577 AE DC            [24] 1579 	mov	r6,_UEP0_CTRL
      000579 7F 00            [12] 1580 	mov	r7,#0x00
      00057B 63 06 40         [24] 1581 	xrl	ar6,#0x40
      00057E 8E DC            [24] 1582 	mov	_UEP0_CTRL,r6
                                   1583 ;	main.c:450: break;
                                   1584 ;	main.c:451: case USB_SET_ADDRESS:
      000580 80 1E            [24] 1585 	sjmp	00205$
      000582                       1586 00200$:
                                   1587 ;	main.c:452: USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
      000582 E5 E3            [12] 1588 	mov	a,_USB_DEV_AD
      000584 54 80            [12] 1589 	anl	a,#0x80
      000586 FF               [12] 1590 	mov	r7,a
      000587 E5 08            [12] 1591 	mov	a,_SetupLen
      000589 FE               [12] 1592 	mov	r6,a
      00058A 4F               [12] 1593 	orl	a,r7
      00058B F5 E3            [12] 1594 	mov	_USB_DEV_AD,a
                                   1595 ;	main.c:453: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
      00058D 75 DC 02         [24] 1596 	mov	_UEP0_CTRL,#0x02
                                   1597 ;	main.c:454: break;
                                   1598 ;	main.c:455: default:
      000590 80 0E            [24] 1599 	sjmp	00205$
      000592                       1600 00201$:
                                   1601 ;	main.c:456: UEP0_T_LEN = 0;													  //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
      000592 75 DD 00         [24] 1602 	mov	_UEP0_T_LEN,#0x00
                                   1603 ;	main.c:457: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
      000595 75 DC 02         [24] 1604 	mov	_UEP0_CTRL,#0x02
                                   1605 ;	main.c:460: break;
                                   1606 ;	main.c:461: case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
      000598 80 06            [24] 1607 	sjmp	00205$
      00059A                       1608 00203$:
                                   1609 ;	main.c:474: UEP0_T_LEN = 0;
      00059A 75 DD 00         [24] 1610 	mov	_UEP0_T_LEN,#0x00
                                   1611 ;	main.c:475: UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_ACK;  //状态阶段，对IN响应NAK
      00059D 85 DC DC         [24] 1612 	mov	_UEP0_CTRL,_UEP0_CTRL
                                   1613 ;	main.c:483: }
      0005A0                       1614 00205$:
                                   1615 ;	main.c:484: UIF_TRANSFER = 0;														   //写0清空中断
                                   1616 ;	assignBit
      0005A0 C2 D9            [12] 1617 	clr	_UIF_TRANSFER
      0005A2                       1618 00207$:
                                   1619 ;	main.c:486: if(UIF_BUS_RST)																 //设备模式USB总线复位中断
      0005A2 30 D8 29         [24] 1620 	jnb	_UIF_BUS_RST,00209$
                                   1621 ;	main.c:491: UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
      0005A5 75 DC 02         [24] 1622 	mov	_UEP0_CTRL,#0x02
                                   1623 ;	main.c:492: UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
      0005A8 75 D2 12         [24] 1624 	mov	_UEP1_CTRL,#0x12
                                   1625 ;	main.c:493: UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
      0005AB 75 D4 12         [24] 1626 	mov	_UEP2_CTRL,#0x12
                                   1627 ;	main.c:494: USB_DEV_AD = 0x00;
      0005AE 75 E3 00         [24] 1628 	mov	_USB_DEV_AD,#0x00
                                   1629 ;	main.c:495: UIF_SUSPEND = 0;
                                   1630 ;	assignBit
      0005B1 C2 DA            [12] 1631 	clr	_UIF_SUSPEND
                                   1632 ;	main.c:496: UIF_TRANSFER = 0;
                                   1633 ;	assignBit
      0005B3 C2 D9            [12] 1634 	clr	_UIF_TRANSFER
                                   1635 ;	main.c:497: UIF_BUS_RST = 0;															 //清中断标志
                                   1636 ;	assignBit
      0005B5 C2 D8            [12] 1637 	clr	_UIF_BUS_RST
                                   1638 ;	main.c:498: Midi_Input_Point = 0;   //循环缓冲区输入指针
      0005B7 78 6C            [12] 1639 	mov	r0,#_Midi_Input_Point
      0005B9 76 00            [12] 1640 	mov	@r0,#0x00
                                   1641 ;	main.c:499: Midi_Output_Point = 0;  //循环缓冲区读出指针
      0005BB 78 6D            [12] 1642 	mov	r0,#_Midi_Output_Point
      0005BD 76 00            [12] 1643 	mov	@r0,#0x00
                                   1644 ;	main.c:500: MidiByteCount = 0;	  //当前缓冲区剩余待取字节数
      0005BF 78 6E            [12] 1645 	mov	r0,#_MidiByteCount
      0005C1 76 00            [12] 1646 	mov	@r0,#0x00
                                   1647 ;	main.c:501: USBByteCount = 0;	   //USB端点收到的长度
      0005C3 78 6F            [12] 1648 	mov	r0,#_USBByteCount
      0005C5 76 00            [12] 1649 	mov	@r0,#0x00
                                   1650 ;	main.c:502: UsbConfig = 0;		  //清除配置值
      0005C7 75 0C 00         [24] 1651 	mov	_UsbConfig,#0x00
                                   1652 ;	main.c:503: UpPoint2_Busy = 0;
      0005CA 78 71            [12] 1653 	mov	r0,#_UpPoint2_Busy
      0005CC 76 00            [12] 1654 	mov	@r0,#0x00
      0005CE                       1655 00209$:
                                   1656 ;	main.c:505: if (UIF_SUSPEND)																 //USB总线挂起/唤醒完成
                                   1657 ;	main.c:507: UIF_SUSPEND = 0;
                                   1658 ;	assignBit
      0005CE 10 DA 02         [24] 1659 	jbc	_UIF_SUSPEND,00548$
      0005D1 80 21            [24] 1660 	sjmp	00216$
      0005D3                       1661 00548$:
                                   1662 ;	main.c:508: if ( USB_MIS_ST & bUMS_SUSPEND )											 //挂起
      0005D3 E5 DA            [12] 1663 	mov	a,_USB_MIS_ST
      0005D5 30 E2 1F         [24] 1664 	jnb	acc.2,00218$
                                   1665 ;	main.c:513: while ( XBUS_AUX & bUART0_TX )
      0005D8                       1666 00210$:
      0005D8 E5 A2            [12] 1667 	mov	a,_XBUS_AUX
      0005DA 20 E7 FB         [24] 1668 	jb	acc.7,00210$
                                   1669 ;	main.c:517: SAFE_MOD = 0x55;
      0005DD 75 A1 55         [24] 1670 	mov	_SAFE_MOD,#0x55
                                   1671 ;	main.c:518: SAFE_MOD = 0xAA;
      0005E0 75 A1 AA         [24] 1672 	mov	_SAFE_MOD,#0xaa
                                   1673 ;	main.c:519: WAKE_CTRL = bWAK_BY_USB | bWAK_RXD0_LO | bWAK_RXD1_LO;					  //USB或者RXD0/1有信号时可被唤醒
      0005E3 75 A9 C1         [24] 1674 	mov	_WAKE_CTRL,#0xc1
                                   1675 ;	main.c:520: PCON |= PD;																 //睡眠
      0005E6 43 87 02         [24] 1676 	orl	_PCON,#0x02
                                   1677 ;	main.c:521: SAFE_MOD = 0x55;
      0005E9 75 A1 55         [24] 1678 	mov	_SAFE_MOD,#0x55
                                   1679 ;	main.c:522: SAFE_MOD = 0xAA;
      0005EC 75 A1 AA         [24] 1680 	mov	_SAFE_MOD,#0xaa
                                   1681 ;	main.c:523: WAKE_CTRL = 0x00;
      0005EF 75 A9 00         [24] 1682 	mov	_WAKE_CTRL,#0x00
      0005F2 80 03            [24] 1683 	sjmp	00218$
      0005F4                       1684 00216$:
                                   1685 ;	main.c:527: USB_INT_FG = 0xFF;															 //清中断标志
      0005F4 75 D8 FF         [24] 1686 	mov	_USB_INT_FG,#0xff
      0005F7                       1687 00218$:
                                   1688 ;	main.c:530: }
      0005F7 D0 D0            [24] 1689 	pop	psw
      0005F9 D0 00            [24] 1690 	pop	(0+0)
      0005FB D0 01            [24] 1691 	pop	(0+1)
      0005FD D0 02            [24] 1692 	pop	(0+2)
      0005FF D0 03            [24] 1693 	pop	(0+3)
      000601 D0 04            [24] 1694 	pop	(0+4)
      000603 D0 05            [24] 1695 	pop	(0+5)
      000605 D0 06            [24] 1696 	pop	(0+6)
      000607 D0 07            [24] 1697 	pop	(0+7)
      000609 D0 83            [24] 1698 	pop	dph
      00060B D0 82            [24] 1699 	pop	dpl
      00060D D0 F0            [24] 1700 	pop	b
      00060F D0 E0            [24] 1701 	pop	acc
      000611 D0 20            [24] 1702 	pop	bits
      000613 32               [24] 1703 	reti
                                   1704 ;------------------------------------------------------------
                                   1705 ;Allocation info for local variables in function 'main'
                                   1706 ;------------------------------------------------------------
                                   1707 ;length                    Allocated to registers r7 
                                   1708 ;Midi_Timeout              Allocated to registers 
                                   1709 ;------------------------------------------------------------
                                   1710 ;	main.c:551: main()
                                   1711 ;	-----------------------------------------
                                   1712 ;	 function main
                                   1713 ;	-----------------------------------------
      000614                       1714 _main:
                                   1715 ;	main.c:553: uint8_t length = 0;
      000614 7F 00            [12] 1716 	mov	r7,#0x00
                                   1717 ;	main.c:555: CfgFsys( );														   //CH559时钟选择配置
      000616 C0 07            [24] 1718 	push	ar7
      000618 12 06 9B         [24] 1719 	lcall	_CfgFsys
                                   1720 ;	main.c:556: mDelaymS(5);														  //修改主频等待内部晶振稳定,必加
      00061B 90 00 05         [24] 1721 	mov	dptr,#0x0005
      00061E 12 06 C3         [24] 1722 	lcall	_mDelaymS
                                   1723 ;	main.c:557: mInitSTDIO( );														//串口0,可以用于调试
      000621 12 06 E5         [24] 1724 	lcall	_mInitSTDIO
                                   1725 ;	main.c:558: UART1Setup( );														//用于CDC
      000624 12 07 A7         [24] 1726 	lcall	_UART1Setup
                                   1727 ;	main.c:564: USBDeviceCfg();
      000627 12 00 BD         [24] 1728 	lcall	_USBDeviceCfg
                                   1729 ;	main.c:565: USBDeviceEndPointCfg();											   //端点配置
      00062A 12 00 E7         [24] 1730 	lcall	_USBDeviceEndPointCfg
                                   1731 ;	main.c:566: USBDeviceIntCfg();													//中断初始化
      00062D 12 00 D6         [24] 1732 	lcall	_USBDeviceIntCfg
      000630 D0 07            [24] 1733 	pop	ar7
                                   1734 ;	main.c:567: UEP0_T_LEN = 0;
      000632 75 DD 00         [24] 1735 	mov	_UEP0_T_LEN,#0x00
                                   1736 ;	main.c:568: UEP1_T_LEN = 0;													   //预使用发送长度一定要清空
      000635 75 D3 00         [24] 1737 	mov	_UEP1_T_LEN,#0x00
                                   1738 ;	main.c:569: UEP2_T_LEN = 0;													   //预使用发送长度一定要清空
      000638 75 D5 00         [24] 1739 	mov	_UEP2_T_LEN,#0x00
                                   1740 ;	main.c:571: while(1)
      00063B                       1741 00112$:
                                   1742 ;	main.c:573: if(UsbConfig)
      00063B E5 0C            [12] 1743 	mov	a,_UsbConfig
      00063D 60 FC            [24] 1744 	jz	00112$
                                   1745 ;	main.c:575: if(USBByteCount)   //USB接收端点有数据
      00063F 78 6F            [12] 1746 	mov	r0,#_USBByteCount
      000641 E6               [12] 1747 	mov	a,@r0
      000642 60 24            [24] 1748 	jz	00102$
                                   1749 ;	main.c:580: memcpy(Receive_Midi_Buf, Ep2Buffer, USBByteCount);
      000644 75 21 80         [24] 1750 	mov	___memcpy_PARM_2,#_Ep2Buffer
      000647 75 22 00         [24] 1751 	mov	(___memcpy_PARM_2 + 1),#(_Ep2Buffer >> 8)
      00064A 75 23 00         [24] 1752 	mov	(___memcpy_PARM_2 + 2),#0x00
      00064D 78 6F            [12] 1753 	mov	r0,#_USBByteCount
      00064F 86 24            [24] 1754 	mov	___memcpy_PARM_3,@r0
      000651 75 25 00         [24] 1755 	mov	(___memcpy_PARM_3 + 1),#0x00
      000654 90 00 2C         [24] 1756 	mov	dptr,#_Receive_Midi_Buf
      000657 75 F0 40         [24] 1757 	mov	b,#0x40
      00065A 12 08 C9         [24] 1758 	lcall	___memcpy
                                   1759 ;	main.c:585: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;
      00065D 53 D4 F3         [24] 1760 	anl	_UEP2_CTRL,#0xf3
                                   1761 ;	main.c:586: length = USBByteCount;
      000660 78 6F            [12] 1762 	mov	r0,#_USBByteCount
      000662 86 07            [24] 1763 	mov	ar7,@r0
                                   1764 ;	main.c:587: USBByteCount = 0;
      000664 78 6F            [12] 1765 	mov	r0,#_USBByteCount
      000666 76 00            [12] 1766 	mov	@r0,#0x00
      000668                       1767 00102$:
                                   1768 ;	main.c:590: if(MidiByteCount)
      000668 78 6E            [12] 1769 	mov	r0,#_MidiByteCount
      00066A E6               [12] 1770 	mov	a,@r0
                                   1771 ;	main.c:592: if(!UpPoint2_Busy)   //端点不繁忙（空闲后的第一包数据，只用作触发上传）
      00066B 78 71            [12] 1772 	mov	r0,#_UpPoint2_Busy
      00066D E6               [12] 1773 	mov	a,@r0
      00066E 70 CB            [24] 1774 	jnz	00112$
                                   1775 ;	main.c:595: if(length>0)
      000670 EF               [12] 1776 	mov	a,r7
      000671 60 C8            [24] 1777 	jz	00112$
                                   1778 ;	main.c:607: memcpy(Ep2Buffer+MAX_PACKET_SIZE, Receive_Midi_Buf, length); //环回MIDI设备
      000673 75 21 2C         [24] 1779 	mov	___memcpy_PARM_2,#_Receive_Midi_Buf
      000676 75 22 00         [24] 1780 	mov	(___memcpy_PARM_2 + 1),#0x00
      000679 75 23 40         [24] 1781 	mov	(___memcpy_PARM_2 + 2),#0x40
      00067C 8F 24            [24] 1782 	mov	___memcpy_PARM_3,r7
      00067E 75 25 00         [24] 1783 	mov	(___memcpy_PARM_3 + 1),#0x00
      000681 90 00 C0         [24] 1784 	mov	dptr,#(_Ep2Buffer + 0x0040)
      000684 75 F0 00         [24] 1785 	mov	b,#0x00
      000687 C0 07            [24] 1786 	push	ar7
      000689 12 08 C9         [24] 1787 	lcall	___memcpy
      00068C D0 07            [24] 1788 	pop	ar7
                                   1789 ;	main.c:608: UEP2_T_LEN = length;								//预使用发送长度一定要清空
      00068E 8F D5            [24] 1790 	mov	_UEP2_T_LEN,r7
                                   1791 ;	main.c:609: UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;			//应答ACK
      000690 53 D4 FC         [24] 1792 	anl	_UEP2_CTRL,#0xfc
                                   1793 ;	main.c:610: UpPoint2_Busy = 1;
      000693 78 71            [12] 1794 	mov	r0,#_UpPoint2_Busy
      000695 76 01            [12] 1795 	mov	@r0,#0x01
                                   1796 ;	main.c:611: length = 0;
      000697 7F 00            [12] 1797 	mov	r7,#0x00
                                   1798 ;	main.c:617: }
      000699 80 A0            [24] 1799 	sjmp	00112$
                                   1800 	.area CSEG    (CODE)
                                   1801 	.area CONST   (CODE)
      000959                       1802 _DevDesc:
      000959 12                    1803 	.db #0x12	; 18
      00095A 01                    1804 	.db #0x01	; 1
      00095B 10                    1805 	.db #0x10	; 16
      00095C 01                    1806 	.db #0x01	; 1
      00095D 00                    1807 	.db #0x00	; 0
      00095E 00                    1808 	.db #0x00	; 0
      00095F 00                    1809 	.db #0x00	; 0
      000960 08                    1810 	.db #0x08	; 8
      000961 86                    1811 	.db #0x86	; 134
      000962 1A                    1812 	.db #0x1a	; 26
      000963 22                    1813 	.db #0x22	; 34
      000964 57                    1814 	.db #0x57	; 87	'W'
      000965 00                    1815 	.db #0x00	; 0
      000966 01                    1816 	.db #0x01	; 1
      000967 01                    1817 	.db #0x01	; 1
      000968 02                    1818 	.db #0x02	; 2
      000969 03                    1819 	.db #0x03	; 3
      00096A 01                    1820 	.db #0x01	; 1
      00096B                       1821 _CfgDesc:
      00096B 09                    1822 	.db #0x09	; 9
      00096C 02                    1823 	.db #0x02	; 2
      00096D 61                    1824 	.db #0x61	; 97	'a'
      00096E 00                    1825 	.db #0x00	; 0
      00096F 02                    1826 	.db #0x02	; 2
      000970 01                    1827 	.db #0x01	; 1
      000971 00                    1828 	.db #0x00	; 0
      000972 80                    1829 	.db #0x80	; 128
      000973 32                    1830 	.db #0x32	; 50	'2'
      000974 09                    1831 	.db #0x09	; 9
      000975 04                    1832 	.db #0x04	; 4
      000976 00                    1833 	.db #0x00	; 0
      000977 00                    1834 	.db #0x00	; 0
      000978 00                    1835 	.db #0x00	; 0
      000979 01                    1836 	.db #0x01	; 1
      00097A 01                    1837 	.db #0x01	; 1
      00097B 00                    1838 	.db #0x00	; 0
      00097C 00                    1839 	.db #0x00	; 0
      00097D 09                    1840 	.db #0x09	; 9
      00097E 24                    1841 	.db #0x24	; 36
      00097F 01                    1842 	.db #0x01	; 1
      000980 00                    1843 	.db #0x00	; 0
      000981 01                    1844 	.db #0x01	; 1
      000982 09                    1845 	.db #0x09	; 9
      000983 00                    1846 	.db #0x00	; 0
      000984 01                    1847 	.db #0x01	; 1
      000985 01                    1848 	.db #0x01	; 1
      000986 09                    1849 	.db #0x09	; 9
      000987 04                    1850 	.db #0x04	; 4
      000988 01                    1851 	.db #0x01	; 1
      000989 00                    1852 	.db #0x00	; 0
      00098A 02                    1853 	.db #0x02	; 2
      00098B 01                    1854 	.db #0x01	; 1
      00098C 03                    1855 	.db #0x03	; 3
      00098D 00                    1856 	.db #0x00	; 0
      00098E 00                    1857 	.db #0x00	; 0
      00098F 07                    1858 	.db #0x07	; 7
      000990 24                    1859 	.db #0x24	; 36
      000991 01                    1860 	.db #0x01	; 1
      000992 00                    1861 	.db #0x00	; 0
      000993 01                    1862 	.db #0x01	; 1
      000994 25                    1863 	.db #0x25	; 37
      000995 00                    1864 	.db #0x00	; 0
      000996 06                    1865 	.db #0x06	; 6
      000997 24                    1866 	.db #0x24	; 36
      000998 02                    1867 	.db #0x02	; 2
      000999 01                    1868 	.db #0x01	; 1
      00099A 01                    1869 	.db #0x01	; 1
      00099B 00                    1870 	.db #0x00	; 0
      00099C 06                    1871 	.db #0x06	; 6
      00099D 24                    1872 	.db #0x24	; 36
      00099E 02                    1873 	.db #0x02	; 2
      00099F 02                    1874 	.db #0x02	; 2
      0009A0 02                    1875 	.db #0x02	; 2
      0009A1 00                    1876 	.db #0x00	; 0
      0009A2 09                    1877 	.db #0x09	; 9
      0009A3 24                    1878 	.db #0x24	; 36
      0009A4 03                    1879 	.db #0x03	; 3
      0009A5 01                    1880 	.db #0x01	; 1
      0009A6 03                    1881 	.db #0x03	; 3
      0009A7 01                    1882 	.db #0x01	; 1
      0009A8 02                    1883 	.db #0x02	; 2
      0009A9 01                    1884 	.db #0x01	; 1
      0009AA 00                    1885 	.db #0x00	; 0
      0009AB 09                    1886 	.db #0x09	; 9
      0009AC 24                    1887 	.db #0x24	; 36
      0009AD 03                    1888 	.db #0x03	; 3
      0009AE 02                    1889 	.db #0x02	; 2
      0009AF 04                    1890 	.db #0x04	; 4
      0009B0 01                    1891 	.db #0x01	; 1
      0009B1 01                    1892 	.db #0x01	; 1
      0009B2 01                    1893 	.db #0x01	; 1
      0009B3 00                    1894 	.db #0x00	; 0
      0009B4 07                    1895 	.db #0x07	; 7
      0009B5 05                    1896 	.db #0x05	; 5
      0009B6 02                    1897 	.db #0x02	; 2
      0009B7 02                    1898 	.db #0x02	; 2
      0009B8 40                    1899 	.db #0x40	; 64
      0009B9 00                    1900 	.db #0x00	; 0
      0009BA 00                    1901 	.db #0x00	; 0
      0009BB 05                    1902 	.db #0x05	; 5
      0009BC 25                    1903 	.db #0x25	; 37
      0009BD 01                    1904 	.db #0x01	; 1
      0009BE 01                    1905 	.db #0x01	; 1
      0009BF 01                    1906 	.db #0x01	; 1
      0009C0 07                    1907 	.db #0x07	; 7
      0009C1 05                    1908 	.db #0x05	; 5
      0009C2 82                    1909 	.db #0x82	; 130
      0009C3 02                    1910 	.db #0x02	; 2
      0009C4 40                    1911 	.db #0x40	; 64
      0009C5 00                    1912 	.db #0x00	; 0
      0009C6 00                    1913 	.db #0x00	; 0
      0009C7 05                    1914 	.db #0x05	; 5
      0009C8 25                    1915 	.db #0x25	; 37
      0009C9 01                    1916 	.db #0x01	; 1
      0009CA 01                    1917 	.db #0x01	; 1
      0009CB 03                    1918 	.db #0x03	; 3
      0009CC                       1919 _LangDes:
      0009CC 04                    1920 	.db #0x04	; 4
      0009CD 03                    1921 	.db #0x03	; 3
      0009CE 09                    1922 	.db #0x09	; 9
      0009CF 04                    1923 	.db #0x04	; 4
      0009D0                       1924 _SerDes:
      0009D0 14                    1925 	.db #0x14	; 20
      0009D1 03                    1926 	.db #0x03	; 3
      0009D2 32                    1927 	.db #0x32	; 50	'2'
      0009D3 00                    1928 	.db #0x00	; 0
      0009D4 30                    1929 	.db #0x30	; 48	'0'
      0009D5 00                    1930 	.db #0x00	; 0
      0009D6 31                    1931 	.db #0x31	; 49	'1'
      0009D7 00                    1932 	.db #0x00	; 0
      0009D8 38                    1933 	.db #0x38	; 56	'8'
      0009D9 00                    1934 	.db #0x00	; 0
      0009DA 2D                    1935 	.db #0x2d	; 45
      0009DB 00                    1936 	.db #0x00	; 0
      0009DC 33                    1937 	.db #0x33	; 51	'3'
      0009DD 00                    1938 	.db #0x00	; 0
      0009DE 2D                    1939 	.db #0x2d	; 45
      0009DF 00                    1940 	.db #0x00	; 0
      0009E0 32                    1941 	.db #0x32	; 50	'2'
      0009E1 00                    1942 	.db #0x00	; 0
      0009E2 37                    1943 	.db #0x37	; 55	'7'
      0009E3 00                    1944 	.db #0x00	; 0
      0009E4                       1945 _Prod_Des:
      0009E4 14                    1946 	.db #0x14	; 20
      0009E5 03                    1947 	.db #0x03	; 3
      0009E6 43                    1948 	.db #0x43	; 67	'C'
      0009E7 00                    1949 	.db #0x00	; 0
      0009E8 48                    1950 	.db #0x48	; 72	'H'
      0009E9 00                    1951 	.db #0x00	; 0
      0009EA 35                    1952 	.db #0x35	; 53	'5'
      0009EB 00                    1953 	.db #0x00	; 0
      0009EC 35                    1954 	.db #0x35	; 53	'5'
      0009ED 00                    1955 	.db #0x00	; 0
      0009EE 78                    1956 	.db #0x78	; 120	'x'
      0009EF 00                    1957 	.db #0x00	; 0
      0009F0 4D                    1958 	.db #0x4d	; 77	'M'
      0009F1 00                    1959 	.db #0x00	; 0
      0009F2 49                    1960 	.db #0x49	; 73	'I'
      0009F3 00                    1961 	.db #0x00	; 0
      0009F4 44                    1962 	.db #0x44	; 68	'D'
      0009F5 00                    1963 	.db #0x00	; 0
      0009F6 49                    1964 	.db #0x49	; 73	'I'
      0009F7 00                    1965 	.db #0x00	; 0
      0009F8                       1966 _Manuf_Des:
      0009F8 0A                    1967 	.db #0x0a	; 10
      0009F9 03                    1968 	.db #0x03	; 3
      0009FA 5F                    1969 	.db #0x5f	; 95
      0009FB 6C                    1970 	.db #0x6c	; 108	'l'
      0009FC CF                    1971 	.db #0xcf	; 207
      0009FD 82                    1972 	.db #0x82	; 130
      0009FE 81                    1973 	.db #0x81	; 129
      0009FF 6C                    1974 	.db #0x6c	; 108	'l'
      000A00 52                    1975 	.db #0x52	; 82	'R'
      000A01 60                    1976 	.db #0x60	; 96
                                   1977 	.area XINIT   (CODE)
                                   1978 	.area CABS    (ABS,CODE)
