/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

UsbCore.c file

作者：电脑圈圈
建立日期: 2008.06.29
修改日期: 2008.08.08（2008北京奥运会开幕式呢）
版本：V1.1
版权所有，盗版必究。
Copyright(C) 电脑圈圈 2008-2018
All rights reserved            
*******************************************************************/

#include "config.h"
#include "pdiusbd12.h"
#include "uart.h"
#include "usbcore.h"
#include "led.h"

idata uint8 Buffer[16];  //读端点0用的缓冲区

//USB设备请求的各字段
uint8  bmRequestType;
uint8  bRequest;
uint16 wValue;
uint16 wIndex;
uint16 wLength;
//当前发送数据的位置
uint8 * pSendData;
//需要发送数据的长度
uint16 SendLength;
//是否需要发送0数据包的标志。在USB控制传输的数据过程中，
//当返回的数据包字节数少于最大包长时，会认为数据过程结束。
//当请求的字节数比实际需要返回的字节数长，而实际返回的字节
//数又刚好是端点0大小的整数倍时，就需要返回一个0长度的数据包
//来结束数据过程。因此这里增加一个标志，供程序决定是否需要返回
//一个0长度的数据包。
uint8 NeedZeroPacket;

//当前的配置值。只有在设置非0配置后
uint8 ConfigValue;

//端点1缓冲是否忙的标志。当缓冲区中有数据时，该标志为真。
//当缓冲区中空闲时，该标志为假。
uint8 Ep1InIsBusy;
uint8 Ep2InIsBusy;

//USB设备描述符的定义
code uint8 DeviceDescriptor[0x12]=  //设备描述符为18字节
{
//bLength字段。设备描述符的长度为18(0x12)字节
 0x12,
 
//bDescriptorType字段。设备描述符的编号为0x01
 0x01,
 
//bcdUSB字段。这里设置版本为USB1.1，即0x0110。
//由于是小端结构，所以低字节在先，即0x10，0x01。
 0x10,
 0x01,
 
//bDeviceClass字段。我们不在设备描述符中定义设备类，
//而在接口描述符中定义设备类，所以该字段的值为0。
 0x00,
 
//bDeviceSubClass字段。bDeviceClass字段为0时，该字段也为0。
 0x00,
 
//bDeviceProtocol字段。bDeviceClass字段为0时，该字段也为0。
 0x00,
 
//bMaxPacketSize0字段。PDIUSBD12的端点0大小的16字节。
 0x10,
 
//idVender字段。厂商ID号，我们这里取0x8888，仅供实验用。
//实际产品不能随便使用厂商ID号，必须跟USB协会申请厂商ID号。
//注意小端模式，低字节在先。
 0x88,
 0x88,
 
//idProduct字段。产品ID号，由于是第八个实验，我们这里取0x0008。
//注意小端模式，低字节应该在前。
 0x08,
 0x00,
 
//bcdDevice字段。我们这个USB MIDI键盘刚开始做，就叫它1.0版吧，即0x0100。
//小端模式，低字节在先。
 0x00,
 0x01,
 
//iManufacturer字段。厂商字符串的索引值，为了方便记忆和管理，
//字符串索引就从1开始吧。
 0x01,

//iProduct字段。产品字符串的索引值。刚刚用了1，这里就取2吧。
//注意字符串索引值不要使用相同的值。
 0x02,
 
//iSerialNumber字段。设备的序列号字符串索引值。
//这里取3就可以了。
 0x03,

//bNumConfigurations字段。该设备所具有的配置数。
//我们只需要一种配置就行了，因此该值设置为1。
 0x01
};
//////////////////////////设备描述符完毕//////////////////////////////

//USB配置描述符集合的定义
//配置描述符总长度为9+9+9+9+7+6+6+9+9+7+5+7+5字节
code uint8 ConfigurationDescriptor[9+9+9+9+7+6+6+9+9+7+5+7+5]=
{
 /***************配置描述符***********************/
 //bLength字段。配置描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。配置描述符编号为0x02。
 0x02,
 
 //wTotalLength字段。配置描述符集合的总长度，
 //包括配置描述符本身、接口描述符、类描述符、端点描述符等。
 sizeof(ConfigurationDescriptor)&0xFF, //低字节
 (sizeof(ConfigurationDescriptor)>>8)&0xFF, //高字节
 
 //bNumInterfaces字段。该配置包含的接口数，有2个接口。
 0x02,
 
 //bConfiguration字段。该配置的值为1。
 0x01,
 
 //iConfigurationz字段，该配置的字符串索引。这里没有，为0。
 0x00,
 
 //bmAttributes字段，该设备的属性。由于我们的板子是总线供电的，
 //并且我们不想实现远程唤醒的功能，所以该字段的值为0x80。
 0x80,
 
 //bMaxPower字段，该设备需要的最大电流量。由于我们的板子
 //需要的电流不到100mA，因此我们这里设置为100mA。由于每单位
 //电流为2mA，所以这里设置为50(0x32)。
 0x32,
 
 /*****************音频控制接口描述符*******************/
 //bLength字段。接口描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。接口描述符的编号为0x04。
 0x04,
 
 //bInterfaceNumber字段。该接口的编号，第一个接口，编号为0。
 0x00,
 
 //bAlternateSetting字段。该接口的备用编号，为0。
 0x00,
 
 //bNumEndpoints字段。非0端点的数目。该接口没有端点
 0x00,
 
 //bInterfaceClass字段。该接口所使用的类。音频接口类的代码为0x01。
 0x01,
 
 //bInterfaceSubClass字段。该接口所使用的子类。音频控制接口的子类代码为0x01。
 0x01,
 
 //bInterfaceProtocol字段。没有使用协议。
 0x00,
 
 //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
 0x00,
 
 
 /*************类特殊音频控制接口**********/
 //bLength字段，该描述符的长度。为9字节。
 0x09,
 
 //bDescriptorType字段，描述符的类型。编号为0x24，CS_INTERFACE_DESCRIPTOR。
 0x24,
 
 //bDescriptorSubtype字段，描述符子类。编号为0x01，HEADER。
 0x01,
 
 //bcdADC字段，协议版本。这里为1.0版。
 0x00,
 0x01,
 
 //wTotalLength字段，类特殊描述符的总长度。这里为9字节。
 0x09,
 0x00,

 //bInCollection字段，流接口的数量。这里仅有一个。
 0x01,
 
 //baInterfaceNr字段，属于此接口的流接口编号。MIDI 流接口1属于此音频控制接口
 0x01,

 /*****************MIDI流接口描述符*******************/
 //bLength字段。接口描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。接口描述符的编号为0x04。
 0x04,
 
 //bInterfaceNumber字段。该接口的编号，第二个接口，编号为1。
 0x01,
 
 //bAlternateSetting字段。该接口的备用编号，为0。
 0x00,
 
 //bNumEndpoints字段。非0端点的数目。MIDI流接口使用一对批量输出/输出端点。
 0x02,
 
 //bInterfaceClass字段。该接口所使用的类。音频接口类的代码为0x01。
 0x01,
 
 //bInterfaceSubClass字段。该接口所使用的子类。MIDI流接口的子类代码为0x03。
 0x03,
 
 //bInterfaceProtocol字段。没有使用协议。
 0x00,
 
 //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
 0x00,
 
 /**********类特殊MIDI流接口描述符*****/
 /*******头描述符********/
 //bLength字段。该描述符的长度，7字节。
 0x07,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MS_HEADER
 0x01,
 
 //bcdMSC字段。该MIDI流类所使用的协议版本，为1.0
 0x00,
 0x01,
 
 //wTotalLengthz字段。整个类特殊MIDI流接口描述符总长度
 0x25,
 0x00,
 
 /**********内嵌输入插孔描述符********/
 //bLength字段。该描述符的长度，为6字节。
 0x06,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_IN_JACK
 0x02,
 
 //bJackType字段。该插孔的类型，为内嵌（EMBEDDED）
 0x01,
 
 //bJackID字段。该插孔的唯一ID，这里取值1
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,

 /**********外部输入插孔描述符********/
 //bLength字段。该描述符的长度，为6字节。
 0x06,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_IN_JACK
 0x02,
 
 //bJackType字段。该插孔的类型，为外部（EXTERNAL）
 0x02,
 
 //bJackID字段。该插孔的唯一ID，这里取值2
 0x02,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,
  
 /**********内嵌输出插孔描述符********/
 //bLength字段。该描述符的长度，为9字节。
 0x09,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_OUT_JACK
 0x03,
 
 //bJackType字段。该插孔的类型，为内嵌（EMBEDDED）
 0x01,
 
 //bJackID字段。该插孔的唯一ID，这里取值3
 0x03,
 
 //bNrInputPins字段。该输出插孔的输入引脚数。这里仅有一个。
 0x01,
 
 //baSourceID字段。连接到该插孔输入引脚的输入插孔的ID，选择为外部输入插孔2
 0x02,
 
 //BaSourcePin字段。外部输入插孔连接在该插孔的输入引脚1上
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00, 

 /**********外部输出插孔描述符********/
 //bLength字段。该描述符的长度，为9字节。
 0x09,
 
 //bDescriptorType字段。该描述符的类型，为CS_INTERFACE。
 0x24,
 
 //bDescriptorSubtype字段。描述符子类，为MIDI_OUT_JACK
 0x03,
 
 //bJackType字段。该插孔的类型，为外部（EXTERNAL）
 0x02,
 
 //bJackID字段。该插孔的唯一ID，这里取值4
 0x04,
 
 //bNrInputPins字段。该输出插孔的输入引脚数。这里仅有一个。
 0x01,
 
 //baSourceID字段。连接到该插孔输入引脚的输入插孔的ID，选择为内嵌输入插孔
 0x01,
 
 //BaSourcePin字段。内嵌输入插孔连接在该插孔的输入引脚1上
 0x01,
 
 //iJack字段。该插孔的字符串描述符索引，这里没有，为0
 0x00,
  
 /*************标准批量数据输入端点描述符****************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输入端点2。
 //D7位表示数据方向，输入端点D7为1。所以输入端点2的地址为0x82。
 0x82,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为批端点。批量端点的编号为2。其它位保留为0。
 0x02,
 
 //wMaxPacketSize字段。该端点的最大包长。端点2的最大包长为64字节。
 //注意低字节在先。
 0x40,
 0x00,
 
 //bInterval字段。端点查询的时间，此处无意义。
 0x00,
 
 /**********类特殊MIDI流批量数据端点描述符********/
 //bLength字段，该描述符的长度。为5字节。
 0x05,
 
 //bDescriptorType字段，该描述符的类型，为类特殊端点描述符（CS_ENDPOINT）
 0x25,
 
 //bDescriptorSubType字段，该描述符的子类型，为MS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack字段，该端点的内嵌输出插孔的数量，这里只有1个
 0x01,
 
 //baAssocJackID字段，该端点的内嵌输出插孔的ID号。
 //我们在前面定义了一个内嵌输出插孔，ID号为3。
 0x03,
 
 /*************标准批量数据输出端点描述符****************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输出端点2。
 //D7位表示数据方向，输出端点D7为0。所以输出端点2的地址为0x02。
 0x02,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为批端点。批量端点的编号为2。其它位保留为0。
 0x02,
 
 //wMaxPacketSize字段。该端点的最大包长。端点2的最大包长为64字节。
 //注意低字节在先。
 0x40,
 0x00,
 
 //bInterval字段。端点查询的时间，此处无意义。
 0x00,

 /**********类特殊MIDI流批量数据端点描述符********/
 //bLength字段，该描述符的长度。为5字节。
 0x05,
 
 //bDescriptorType字段，该描述符的类型，为类特殊端点描述符（CS_ENDPOINT）
 0x25,
 
 //bDescriptorSubType字段，该描述符的子类型，为MS_GENERAL
 0x01,
 
 //bNumEmbMIDIJack字段，该端点的内嵌输入插孔的数量，这里只有1个
 0x01,
 
 //baAssocJackID字段，该端点的内嵌输入插孔的ID号。
 //我们在前面定义了一个内嵌输入插孔，ID号为1。
 0x01
};
////////////////////////配置描述符集合完毕//////////////////////////

/************************语言ID的定义********************/
code uint8 LanguageId[4]=
{
 0x04, //本描述符的长度
 0x03, //字符串描述符
 //0x0409为美式英语的ID
 0x09,
 0x04
};
////////////////////////语言ID完毕//////////////////////////////////

/**************************************************/
/*********        本转换结果来自         **********/
/********* Http://computer00.21ic.org    **********/
/*********        作者: 电脑圈圈         **********/
/*********         欢迎大家使用          **********/
/*********    版权所有，盗版请写明出处   **********/
/**************************************************/

//http://computer00.21ic.org/user1/2198/archives/2007/42769.html
//字符串“电脑圈圈的USB专区 Http://group.ednchina.com/93/”的Unicode编码
//8位小端格式
code uint8 ManufacturerStringDescriptor[82]={
82,         //该描述符的长度为82字节
0x03,       //字符串描述符的类型编码为0x03
0x35, 0x75, //电
0x11, 0x81, //脑
0x08, 0x57, //圈
0x08, 0x57, //圈
0x84, 0x76, //的
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x13, 0x4e, //专
0x3a, 0x53, //区
0x20, 0x00, // 
0x48, 0x00, //H
0x74, 0x00, //t
0x74, 0x00, //t
0x70, 0x00, //p
0x3a, 0x00, //:
0x2f, 0x00, ///
0x2f, 0x00, ///
0x67, 0x00, //g
0x72, 0x00, //r
0x6f, 0x00, //o
0x75, 0x00, //u
0x70, 0x00, //p
0x2e, 0x00, //.
0x65, 0x00, //e
0x64, 0x00, //d
0x6e, 0x00, //n
0x63, 0x00, //c
0x68, 0x00, //h
0x69, 0x00, //i
0x6e, 0x00, //n
0x61, 0x00, //a
0x2e, 0x00, //.
0x63, 0x00, //c
0x6f, 0x00, //o
0x6d, 0x00, //m
0x2f, 0x00, ///
0x39, 0x00, //9
0x33, 0x00, //3
0x2f, 0x00  ///
};
/////////////////////////厂商字符串结束/////////////////////////////

//字符串“《圈圈教你玩USB》之USB MIDI键盘”的Unicode编码
//8位小端格式
code uint8 ProductStringDescriptor[44]={
44,         //该描述符的长度为44字节
0x03,       //字符串描述符的类型编码为0x03
0x0a, 0x30, //《
0x08, 0x57, //圈
0x08, 0x57, //圈
0x59, 0x65, //教
0x60, 0x4f, //你
0xa9, 0x73, //玩
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x0b, 0x30, //》
0x4b, 0x4e, //之
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x20, 0x00, // 
0x4d, 0x00, //M
0x49, 0x00, //I
0x44, 0x00, //D
0x49, 0x00, //I
0x2e, 0x95, //键
0xd8, 0x76  //盘
};
////////////////////////产品字符串结束////////////////////////////

//字符串“2008-08-08”的Unicode编码
//8位小端格式
code uint8 SerialNumberStringDescriptor[22]={
22,         //该描述符的长度为22字节
0x03,       //字符串描述符的类型编码为0x03
0x32, 0x00, //2
0x30, 0x00, //0
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x30, 0x00, //0
0x38, 0x00, //8
0x2d, 0x00, //-
0x30, 0x00, //0
0x38, 0x00  //8
};
//////////////////////产品序列号字符串结束/////////////////////////

/********************************************************************
函数功能：延时x毫秒函数。
入口参数：x：延时的毫秒数。
返    回：无。
备    注：无。
********************************************************************/
void DelayXms(uint16 x)                
{
 uint16 i;
 uint16 j;
 for(i=0;i<x;i++)
 for(j=0;j<227;j++); //循环语句延时
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：USB断开连接函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbDisconnect(void)
{
#ifdef DEBUG0
 Prints("断开USB连接。\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //写设置模式命令
 D12WriteByte(0x06); //设置模式的第一字节
 D12WriteByte(0x47); //设置模式的第二字节
 DelayXms(1000);  //延迟1秒
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：USB连接函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbConnect(void)
{
#ifdef DEBUG0
 Prints("连接USB。\r\n");
#endif
 D12WriteCommand(D12_SET_MODE);  //写设置模式命令
 D12WriteByte(0x16); //设置模式的第一字节
 D12WriteByte(0x47); //设置模式的第二字节
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：总线挂起中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbBusSuspend(void)
{
#ifdef DEBUG0
 Prints("USB总线挂起。\r\n");
#endif
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：总线复位中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbBusReset(void)
{
#ifdef DEBUG0
 Prints("USB总线复位。\r\n");
#endif
 Ep1InIsBusy=0; //复位后端点1输入缓冲区空闲。
 Ep2InIsBusy=0; //复位后端点2输入缓冲区空闲。
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：根据pData和SendLength将数据发送到端点0的函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp0SendData(void)
{
 //将数据写到端点中去准备发送
 //写之前要先判断一下需要发送的数据是否比端点0
 //最大长度大，如果超过端点大小，则一次只能发送
 //最大包长的数据。端点0的最大包长在DeviceDescriptor[7]
 if(SendLength>DeviceDescriptor[7])
 {
  //按最大包长度发送
  D12WriteEndpointBuffer(1,DeviceDescriptor[7],pSendData);
  //发送后剩余字节数减少最大包长
  SendLength-=DeviceDescriptor[7];
  //发送一次后指针位置要调整
  pSendData+= DeviceDescriptor[7];
 }
 else
 {
  if(SendLength!=0)
  {
   //不够最大包长，可以直接发送
   D12WriteEndpointBuffer(1,SendLength,pSendData);
   //发送完毕后，SendLength长度变为0
   SendLength=0;
  }
  else //如果要发送的数据包长度为0
  {
   if(NeedZeroPacket==1) //如果需要发送0长度数据
   {
    D12WriteEndpointBuffer(1,0,pSendData); //发送0长度数据包
    NeedZeroPacket=0; //清需要发送0长度数据包标志
   }
  }
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点0输出中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp0Out(void)
{
#ifdef DEBUG0
 Prints("USB端点0输出中断。\r\n");
#endif
 //读取端点0输出最后传输状态，该操作清除中断标志
 //并判断第5位是否为1，如果是，则说明是建立包
 if(D12ReadEndpointLastStatus(0)&0x20)
 {
  D12ReadEndpointBuffer(0,16,Buffer); //读建立过程数据
  D12AcknowledgeSetup(); //应答建立包
  D12ClearBuffer(); //清缓冲区
  //将缓冲数据填到设备请求的各字段中
  bmRequestType=Buffer[0];
  bRequest=Buffer[1];
  wValue=Buffer[2]+(((uint16)Buffer[3])<<8);
  wIndex=Buffer[4]+(((uint16)Buffer[5])<<8);
  wLength=Buffer[6]+(((uint16)Buffer[7])<<8);
  //下面的代码判断具体的请求，并根据不同的请求进行相关操作
  //如果D7位为1，则说明是输入请求
  if((bmRequestType&0x80)==0x80)
  {
   //根据bmRequestType的D6~5位散转，D6~5位表示请求的类型
   //0为标准请求，1为类请求，2为厂商请求。
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //标准请求
     #ifdef DEBUG0
      Prints("USB标准输入请求：");
     #endif
     //USB协议定义了几个标准输入请求，我们实现这些标准请求即可
     //请求的代码在bRequest中，对不同的请求代码进行散转
     //事实上，我们还需要对接收者进行散转，因为不同的请求接收者
     //是不一样的。接收者在bmRequestType的D4~D0位中定义。
     //我们这里为了简化操作，有些就省略了对接收者的判断。
     //例如获取描述符的请求，只根据描述符的类型来区别。
     switch(bRequest)
     {
      case GET_CONFIGURATION: //获取配置
       #ifdef DEBUG0
        Prints("获取配置。\r\n");
       #endif
      break;
      
      case GET_DESCRIPTOR:  //获取描述符
       #ifdef DEBUG0
        Prints("获取描述符——");
       #endif
       //对描述符类型进行散转，对于全速设备，
       //标准请求只支持发送到设备的设备、配置、字符串三种描述符
       switch((wValue>>8)&0xFF)
        {
         case DEVICE_DESCRIPTOR: //设备描述符
          #ifdef DEBUG0
           Prints("设备描述符。\r\n");
          #endif
          pSendData=DeviceDescriptor;  //需要发送的数据
          //判断请求的字节数是否比实际需要发送的字节数多
          //这里请求的是设备描述符，因此数据长度就是
          //DeviceDescriptor[0]。如果请求的比实际的长，
          //那么只返回实际长度的数据
          if(wLength>DeviceDescriptor[0])
          {
           SendLength=DeviceDescriptor[0];
           if(SendLength%DeviceDescriptor[7]==0) //并且刚好是整数个数据包时
           {
            NeedZeroPacket=1; //需要返回0长度的数据包
           }
          }
          else
          {
           SendLength=wLength;
          }
          //将数据通过EP0返回
          UsbEp0SendData();
         break;
         
         case CONFIGURATION_DESCRIPTOR:  //配置描述符
          #ifdef DEBUG0
           Prints("配置描述符。\r\n");
          #endif
          pSendData=ConfigurationDescriptor; //需要发送的数据为配置描述符
          //判断请求的字节数是否比实际需要发送的字节数多
          //这里请求的是配置描述符集合，因此数据长度就是
          //ConfigurationDescriptor[3]*256+ConfigurationDescriptor[2]。
          //如果请求的比实际的长，那么只返回实际长度的数据
          SendLength=ConfigurationDescriptor[3];
          SendLength=SendLength*256+ConfigurationDescriptor[2];
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //并且刚好是整数个数据包时
           {
            NeedZeroPacket=1; //需要返回0长度的数据包
           }
          }
          else
          {
           SendLength=wLength;
          }
          //将数据通过EP0返回
          UsbEp0SendData();
         break;
         
         case STRING_DESCRIPTOR:  //字符串描述符
          #ifdef DEBUG0
           Prints("字符串描述符");
          #endif
          switch(wValue&0xFF)  //根据wValue的低字节（索引值）散转
          {
           case 0:  //获取语言ID
            #ifdef DEBUG0
             Prints("(语言ID)。\r\n");
            #endif
            pSendData=LanguageId;
            SendLength=LanguageId[0];
           break;
           
           case 1:  //厂商字符串的索引值为1，所以这里为厂商字符串
           #ifdef DEBUG0
             Prints("(厂商描述)。\r\n");
            #endif
            pSendData=ManufacturerStringDescriptor;
            SendLength=ManufacturerStringDescriptor[0];
           break;
           
           case 2:  //产品字符串的索引值为2，所以这里为产品字符串
           #ifdef DEBUG0
             Prints("(产品描述)。\r\n");
            #endif
            pSendData=ProductStringDescriptor;
            SendLength=ProductStringDescriptor[0];
           break;
           
           case 3:  //产品序列号的索引值为3，所以这里为序列号
           #ifdef DEBUG0
             Prints("(产品序列号)。\r\n");
            #endif
            pSendData=SerialNumberStringDescriptor;
            SendLength=SerialNumberStringDescriptor[0];
           break;
           
           default :
            #ifdef DEBUG0
             Prints("(未知的索引值)。\r\n");
            #endif
            //对于未知索引值的请求，返回一个0长度的包
            SendLength=0;
            NeedZeroPacket=1;
           break;
          }
          //判断请求的字节数是否比实际需要发送的字节数多
          //如果请求的比实际的长，那么只返回实际长度的数据
          if(wLength>SendLength)
          {
           if(SendLength%DeviceDescriptor[7]==0) //并且刚好是整数个数据包时
           {
            NeedZeroPacket=1; //需要返回0长度的数据包
           }
          }
          else
          {
           SendLength=wLength;
          }
          //将数据通过EP0返回
          UsbEp0SendData();         
         break;

         case REPORT_DESCRIPTOR:  //报告描述符
          #ifdef DEBUG0
           Prints("报告描述符。\r\n");
          #endif
         break;
                 
         default:  //其它描述符
          #ifdef DEBUG0
           Prints("其他描述符，描述符代码：");
           PrintHex((wValue>>8)&0xFF);
           Prints("\r\n");
          #endif
         break;
        }
       break;
      
      case GET_INTERFACE: //获取接口
       #ifdef DEBUG0
        Prints("获取接口。\r\n");
       #endif
      break;
      
      case GET_STATUS: //获取状态
       #ifdef DEBUG0
        Prints("获取状态。\r\n");
       #endif
      break;
      
      case SYNCH_FRAME: //同步帧
       #ifdef DEBUG0
        Prints("同步帧。\r\n");
       #endif
      break;
      
      default:  //未定义的标准请求
       #ifdef DEBUG0
        Prints("错误：未定义的标准输入请求。\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //类请求
     #ifdef DEBUG0
      Prints("USB类输入请求：\r\n");
     #endif
    break;
    
    case 2:  //厂商请求
     #ifdef DEBUG0
      Prints("USB厂商输入请求：\r\n");
     #endif
    break;
    
    default: //未定义的请求。这里只显示一个报错信息。
     #ifdef DEBUG0
      Prints("错误：未定义的输入请求。\r\n");
     #endif
    break;
   }
  }
  //否则说明是输出请求
  else //if(bmRequestType&0x80==0x80)之else
  {
   //根据bmRequestType的D6~5位散转，D6~5位表示请求的类型
   //0为标准请求，1为类请求，2为厂商请求。
   switch((bmRequestType>>5)&0x03)
   {
    case 0:  //标准请求
     #ifdef DEBUG0
      Prints("USB标准输出请求：");
     #endif
     //USB协议定义了几个标准输出请求，我们实现这些标准请求即可
     //请求的代码在bRequest中，对不同的请求代码进行散转
     switch(bRequest)
     {
      case CLEAR_FEATURE: //清除特性
       #ifdef DEBUG0
        Prints("清除特性。\r\n");
       #endif
      break;
      
      case SET_ADDRESS:  //设置地址
       #ifdef DEBUG0
        Prints("设置地址。地址为：");
        PrintHex(wValue&0xFF); //显示所设置的地址
        Prints("\r\n");
       #endif
       D12SetAddress(wValue&0xFF); //wValue中的低字节是设置的地址值
       //设置地址没有数据过程，直接进入到状态过程，返回一个0长度的数据包
       SendLength=0;
       NeedZeroPacket=1;
       //将数据通过EP0返回
       UsbEp0SendData();
      break;
      
      case SET_CONFIGURATION: //设置配置
       #ifdef DEBUG0
        Prints("设置配置。\r\n");
       #endif
       //使能非0端点。非0端点只有在设置为非0的配置后才能使能。
       //wValue的低字节为配置的值，如果该值为非0，才能使能非0端点。
       //保存当前配置值
       ConfigValue=wValue&0xFF;
       D12SetEndpointEnable(ConfigValue);
       //返回一个0长度的状态数据包
       SendLength=0;
       NeedZeroPacket=1;
       //将数据通过EP0返回
       UsbEp0SendData();
      break;
      
      case SET_DESCRIPTOR: //设置描述符
       #ifdef DEBUG0
        Prints("设置描述符。\r\n");
       #endif
      break;
      
      case SET_FEATURE: //设置特性
       #ifdef DEBUG0
        Prints("设置特性。\r\n");
       #endif
      break;

      case SET_INTERFACE: //设置接口
       #ifdef DEBUG0
        Prints("设置接口。\r\n");
       #endif
      break;      
      
      default:  //未定义的标准请求
       #ifdef DEBUG0
        Prints("错误：未定义的标准输出请求。\r\n");
       #endif       
      break;
     }
    break;
    
    case 1:  //类请求
     #ifdef DEBUG0
      Prints("USB类输出请求：");
     #endif
     switch(bRequest)
     {
      case SET_IDLE:
       #ifdef DEBUG0
        Prints("设置空闲。\r\n");
       #endif
       //只需要返回一个0长度的数据包即可
       SendLength=0;
       NeedZeroPacket=1;
       //将数据通过EP0返回
       UsbEp0SendData();
      break;
      
      default:
       #ifdef DEBUG0
        Prints("未知请求。\r\n");
       #endif
      break;
     }
    break;
    
    case 2:  //厂商请求
     #ifdef DEBUG0
      Prints("USB厂商输出请求：\r\n");
     #endif
    break;
    
    default: //未定义的请求。这里只显示一个报错信息。
     #ifdef DEBUG0
      Prints("错误：未定义的输出请求。\r\n");
     #endif
    break;
   }
  }
 }
 //普通数据输出
 else //if(D12ReadEndpointLastStatus(0)&0x20)之else
 {
  D12ReadEndpointBuffer(0,16,Buffer);
  D12ClearBuffer();
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点0输入中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp0In(void)
{
#ifdef DEBUG0
 Prints("USB端点0输入中断。\r\n");
#endif
 //读最后发送状态，这将清除端点0的中断标志位
 D12ReadEndpointLastStatus(1);
 //发送剩余的字节数
 UsbEp0SendData();
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点1输出中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp1Out(void)
{
#ifdef DEBUG0
 Prints("USB端点1输出中断。\r\n");
#endif
 //读端点最后状态，这将清除端点1输出的中断标志位
 D12ReadEndpointLastStatus(2);
 //清除端点缓冲区
 D12ClearBuffer();
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点1输入中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp1In(void)
{
#ifdef DEBUG0
 Prints("USB端点1输入中断。\r\n");
#endif
 //读最后发送状态，这将清除端点1输入的中断标志位
 D12ReadEndpointLastStatus(3);
 //端点1输入处于空闲状态
 Ep1InIsBusy=0;
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点2输出中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp2Out(void)
{
 uint8 Buf[4];
 uint8 Len;
 
#ifdef DEBUG0
 Prints("USB端点2输出中断。\r\n");
#endif
/* 旧版代码，有BUG。修改在函数返回时清除中断
 //当双缓冲没有全满时，可以清除中断标志
 if((D12ReadEndpointStatus(4)&0x60)!=0x60)
 {
  //读最后发送状态，这将清除端点2输入的中断标志位
  D12ReadEndpointLastStatus(4);
 }
*/
 //读回数据
 Len=D12ReadEndpointBuffer(4,4,Buf);
 //清除端点缓冲区
 D12ClearBuffer();
 
 //可以在此增加对输出数据的处理。注意处理速度不能太慢，
 //太慢会让应用程序停止响应，甚至让操作系统崩溃。

 //当两个缓冲区中都没有数据时，才能清除中断标志
 if(!(D12ReadEndpointStatus(4)&0x60))
 {
  //读最后发送状态，这将清除端点2输入的中断标志位
  D12ReadEndpointLastStatus(4);
 }
}
////////////////////////End of function//////////////////////////////

/********************************************************************
函数功能：端点2输入中断处理函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void UsbEp2In(void)
{
#ifdef DEBUG0
 Prints("USB端点2输入中断。\r\n");
#endif
 //读最后发送状态，这将清除端点2输入的中断标志位
 D12ReadEndpointLastStatus(5);
 //端点2输入处于空闲状态
 Ep2InIsBusy=0;
}
////////////////////////End of function//////////////////////////////
