/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
			
        欢迎访问我的USB专区：http://group.ednchina.com/93/
        欢迎访问我的blog：   http://www.ednchina.com/blog/computer00
                             http://computer00.21ic.org

        感谢PCB赞助商——电子园： http://bbs.cepark.com/

UsbCore.c file

作者：电脑圈圈
建立日期: 2008.06.29
修改日期: 2008.07.14
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

//返回了多少次报告的计数器
uint32 Count=0;

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
 
//idProduct字段。产品ID号，由于是第六个实验，我们这里取0x0006。
//注意小端模式，低字节应该在前。
 0x06,
 0x00,
 
//bcdDevice字段。设备版本号，取1.0版，即0x0100。
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

//USB报告描述符的定义
code uint8 ReportDescriptor[]=
{
 //每行开始的第一字节为该条目的前缀，前缀的格式为：
 //D7~D4：bTag。D3~D2：bType；D1~D0：bSize。以下分别对每个条目注释。
 
 //这是一个全局（bType为1）条目，将用途页选择为普通桌面Generic Desktop Page。
 //后面跟1字节数据（bSize为1），后面的字节数就不注释了，自己根据bSize来判断。
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 
 //这是一个局部（bType为2）条目，用途选择为0x00。在普通桌面页中，
 //该用途是未定义的，如果使用该用途来开集合，那么系统将不会把它
 //当作标准系统设备，从而就成了一个用户自定义的HID设备。
 0x09, 0x00, // USAGE (0)
 
 //这是一个主条目（bType为0）条目，开集合，后面跟的数据0x01表示
 //该集合是一个应用集合。它的性质在前面由用途页和用途定义为
 //用户自定义。
 0xa1, 0x01, // COLLECTION (Application)

 //这是一个全局条目，说明逻辑值最小值为0。
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 
 //这是一个全局条目，说明逻辑值最大为255。
 0x25, 0xff, //     LOGICAL_MAXIMUM (255)
 
 //这是一个局部条目，说明用途的最小值为1。
 0x19, 0x01, //     USAGE_MINIMUM (1)
 
 //这是一个局部条目，说明用途的最大值8。
 0x29, 0x08, //     USAGE_MAXIMUM (8) 
 
 //这是一个全局条目，说明数据域的数量为八个。
 0x95, 0x08, //     REPORT_COUNT (8)
 
 //这是一个全局条目，说明每个数据域的长度为8bit，即1字节。
 0x75, 0x08, //     REPORT_SIZE (8)
 
 //这是一个主条目，说明有8个长度为8bit的数据域做为输入。
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 
 //这是一个局部条目，说明用途的最小值为1。
 0x19, 0x01, //     USAGE_MINIMUM (1)
 
 //这是一个局部条目，说明用途的最大值8。
 0x29, 0x08, //     USAGE_MAXIMUM (8) 
 
 //这是一个主条目。定义输出数据（8字节，注意前面的全局条目）。
 0x91, 0x02, //   OUTPUT (Data,Var,Abs)
 
 //下面这个主条目用来关闭前面的集合。bSize为0，所以后面没数据。
 0xc0        // END_COLLECTION
};
//通过上面的报告描述符的定义，我们知道返回的输入报告具有8字节。
//输出报告也有8字节。至于这8字节的数据是干什么用的，就要由用户
//自己来决定了。像我们这个实验，输入报告的第一字节用来描述8个按键
//的状态，第二到第五字节返回返回报告的次数（增加一个长整型的变量Count，
//每发送一次报告就加1）。而输出报告的第一字节则用来控制板上8个LED
//的状态，第二字节（非0时）用来清除上面的报告计数器Count。
///////////////////////////报告描述符完毕////////////////////////////

//USB配置描述符集合的定义
//配置描述符总长度为9+9+9+7+7字节
code uint8 ConfigurationDescriptor[9+9+9+7+7]=
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
 
 //bNumInterfaces字段。该配置包含的接口数，只有一个接口。
 0x01,
 
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
 
 /*******************接口描述符*********************/
 //bLength字段。接口描述符的长度为9字节。
 0x09,
 
 //bDescriptorType字段。接口描述符的编号为0x04。
 0x04,
 
 //bInterfaceNumber字段。该接口的编号，第一个接口，编号为0。
 0x00,
 
 //bAlternateSetting字段。该接口的备用编号，为0。
 0x00,
 
 //bNumEndpoints字段。非0端点的数目。本实例需要二个
 //中断端点（一个输入一个输出），因此该值为2。
 0x02,
 
 //bInterfaceClass字段。该接口所使用的类。本实例是HID类，
 //HID类的编码为0x03。
 0x03,
 
 //bInterfaceSubClass字段。该接口所使用的子类。在HID1.1协议中，
 //只规定了一种子类：支持BIOS引导启动的子类。
 //USB键盘、鼠标属于该子类，子类代码为0x01。
 //但这里我们是自定义的HID设备，所以不使用子类。
 0x00,
 
 //bInterfaceProtocol字段。如果子类为支持引导启动的子类，
 //则协议可选择鼠标和键盘。键盘代码为0x01，鼠标代码为0x02。
 //自定义的HID设备，也不使用协议。
 0x00,
 
 //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
 0x00,
 
 /******************HID描述符************************/
 //bLength字段。本HID描述符下只有一个下级描述符。所以长度为9字节。
 0x09,
 
 //bDescriptorType字段。HID描述符的编号为0x21。
 0x21,
 
 //bcdHID字段。本协议使用的HID1.1协议。注意低字节在先。
 0x10,
 0x01,
 
 //bCountyCode字段。设备适用的国家代码，这里选择为美国，代码0x21。
 0x21,
 
 //bNumDescriptors字段。下级描述符的数目。我们只有一个报告描述符。
 0x01,
 
 //bDescriptorType字段。下级描述符的类型，为报告描述符，编号为0x22。
 0x22,
 
 //bDescriptorLength字段。下级描述符的长度。下级描述符为报告描述符。
 sizeof(ReportDescriptor)&0xFF,
 (sizeof(ReportDescriptor)>>8)&0xFF,
 
 /**********************输入端点描述符***********************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输入端点1。
 //D7位表示数据方向，输入端点D7为1。所以输入端点1的地址为0x81。
 0x81,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为中断端点。中断端点的编号为3。其它位保留为0。
 0x03,
 
 //wMaxPacketSize字段。该端点的最大包长。端点1的最大包长为16字节。
 //注意低字节在先。
 0x10,
 0x00,
 
 //bInterval字段。端点查询的时间，我们设置为10个帧时间，即10ms。
 0x0A,
 
 /**********************输出端点描述符***********************/
 //bLength字段。端点描述符长度为7字节。
 0x07,
 
 //bDescriptorType字段。端点描述符编号为0x05。
 0x05,
 
 //bEndpointAddress字段。端点的地址。我们使用D12的输出端点1。
 //D7位表示数据方向，输出端点D7为0。所以输出端点1的地址为0x01。
 0x01,
 
 //bmAttributes字段。D1~D0为端点传输类型选择。
 //该端点为中断端点。中断端点的编号为3。其它位保留为0。
 0x03,
 
 //wMaxPacketSize字段。该端点的最大包长。端点1的最大包长为16字节。
 //注意低字节在先。
 0x10,
 0x00,
 
 //bInterval字段。端点查询的时间，我们设置为10个帧时间，即10ms。
 0x0A
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

//字符串“《圈圈教你玩USB》之用户自定义的USB HID设备”的Unicode编码
//8位小端格式
code uint8 ProductStringDescriptor[54]={
54,         //该描述符的长度为54字节
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
0x28, 0x75, //用
0x37, 0x62, //户
0xea, 0x81, //自
0x9a, 0x5b, //定
0x49, 0x4e, //义
0x84, 0x76, //的
0x55, 0x00, //U
0x53, 0x00, //S
0x42, 0x00, //B
0x20, 0x00, // 
0x48, 0x00, //H
0x49, 0x00, //I
0x44, 0x00, //D
0xbe, 0x8b, //设
0x07, 0x59  //备
};
////////////////////////产品字符串结束////////////////////////////

//字符串“2008-07-19”的Unicode编码
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
0x37, 0x00, //7
0x2d, 0x00, //-
0x31, 0x00, //1
0x39, 0x00  //9
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
          pSendData=ReportDescriptor; //需要发送的数据为报告描述符
          SendLength=sizeof(ReportDescriptor); //需要返回的数据长度         
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
 uint8 Buf[8]; //用来保存8字节的输出报告。
#ifdef DEBUG0
 Prints("USB端点1输出中断。\r\n");
#endif
 //读端点最后状态，这将清除端点1输出的中断标志位
 D12ReadEndpointLastStatus(2);
 //从端点1输出缓冲读回8字节数据
 D12ReadEndpointBuffer(2,8,Buf);
 //清除端点缓冲区
 D12ClearBuffer();
 
 //输出报告第一字节为LED状态，某位为1时，表示LED亮。
 LEDs=~Buf[0];
 
 //输出报告的第二字节非0时，清除发送计数器Count。
 if(Buf[1]!=0)
 {
  Count=0;
 }
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
#ifdef DEBUG0
 Prints("USB端点2输出中断。\r\n");
#endif
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
}
////////////////////////End of function//////////////////////////////
