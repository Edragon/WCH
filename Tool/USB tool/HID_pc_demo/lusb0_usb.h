#ifndef __USB_H__
#define __USB_H__

#include <stdlib.h>
#include <windows.h>

/*
 * 'interface' is defined somewhere in the Windows header files. This macro
 * is deleted here to avoid conflicts and compile errors.
 */

#ifdef interface
#undef interface
#endif

/*
 * PATH_MAX from limits.h can't be used on Windows if the dll and
 * import libraries are build/used by different compilers
 */

#define LIBUSB_PATH_MAX 512


/*
 * USB spec information
 *
 * This is all stuff grabbed from various USB specs and is pretty much
 * not subject to change
 */

/*
 * Device and/or Interface Class codes
 */

//设备或接口类编码
#define USB_CLASS_PER_INTERFACE		0	/* for DeviceClass */
#define USB_CLASS_AUDIO			      1  	//audio 音频
#define USB_CLASS_COMM			      2		//com	串口
#define USB_CLASS_HID			        3	//HID  人体输入学设备
#define USB_CLASS_PRINTER		      7		//printer 打印机
#define USB_CLASS_MASS_STORAGE		8		//大容量存储
#define USB_CLASS_HUB			        9	//HUB
#define USB_CLASS_DATA			      10	//？？
#define USB_CLASS_VENDOR_SPEC		  0xff

/*
 * Descriptor types
 */
//描述符类型
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE	0x04
#define USB_DT_ENDPOINT		0x05

#define USB_DT_HID			0x21
#define USB_DT_REPORT		0x22
#define USB_DT_PHYSICAL	0x23
#define USB_DT_HUB			0x29

/*
 * Descriptor sizes per descriptor type
 */
//每种描述符的大小
#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9
#define USB_DT_INTERFACE_SIZE		9
#define USB_DT_ENDPOINT_SIZE		7
#define USB_DT_ENDPOINT_AUDIO_SIZE	9	/* Audio extension */
#define USB_DT_HUB_NONVAR_SIZE		7


/* ensure byte-packed structures */
#include <pshpack1.h>


/* All standard descriptors have these 2 fields in common */
//标准描述符都有如下两个文件

//描述符头
struct usb_descriptor_header
{
    unsigned char  bLength;	//长度
    unsigned char  bDescriptorType;//描述符类型
};

/* String descriptor */
//字符串描述符
struct usb_string_descriptor
{
    unsigned char  bLength;	//长度
    unsigned char  bDescriptorType;//描述符类型
    unsigned short wData[1];	//wDATA
};

/* HID descriptor */
//HID描述符
struct usb_hid_descriptor
{
    unsigned char  bLength;	//长度
    unsigned char  bDescriptorType;//描述符类型
    unsigned short bcdHID;	//？？
    unsigned char  bCountryCode;//国家代码？？
    unsigned char  bNumDescriptors;//描述符号码？
};

/* Endpoint descriptor */
//Endpoint描述符
#define USB_MAXENDPOINTS	32
struct usb_endpoint_descriptor
{
    unsigned char  bLength;//长度
    unsigned char  bDescriptorType;//描述符类型
    unsigned char  bEndpointAddress;//endpoint地址
    unsigned char  bmAttributes;//属性
    unsigned short wMaxPacketSize;//最大包大小
    unsigned char  bInterval;//间隔
    unsigned char  bRefresh;//刷新
    unsigned char  bSynchAddress;//同步地址

    unsigned char *extra;	/* Extra descriptors */ 
    int extralen;
};

#define USB_ENDPOINT_ADDRESS_MASK	0x0f    /* in bEndpointAddress */
#define USB_ENDPOINT_DIR_MASK		  0x80

//endpoint类型
#define USB_ENDPOINT_TYPE_MASK		0x03    /* in bmAttributes */ //mask模式
#define USB_ENDPOINT_TYPE_CONTROL	    0			//control模式
#define USB_ENDPOINT_TYPE_ISOCHRONOUS	1			//等时性的
#define USB_ENDPOINT_TYPE_BULK		    2			//bulk 大量？？
#define USB_ENDPOINT_TYPE_INTERRUPT	  3				//中断模式

/* Interface descriptor */
//接口描述符
#define USB_MAXINTERFACES	32
struct usb_interface_descriptor
{
    unsigned char  bLength;
    unsigned char  bDescriptorType;
    unsigned char  bInterfaceNumber;
    unsigned char  bAlternateSetting;
    unsigned char  bNumEndpoints;
    unsigned char  bInterfaceClass;
    unsigned char  bInterfaceSubClass;
    unsigned char  bInterfaceProtocol;
    unsigned char  iInterface;

    struct usb_endpoint_descriptor *endpoint;

    unsigned char *extra;	/* Extra descriptors */
    int extralen;
};

#define USB_MAXALTSETTING	128	/* Hard limit */

struct usb_interface
{
    struct usb_interface_descriptor *altsetting;

    int num_altsetting;
};

/* Configuration descriptor information.. */
//usb配置描述符
#define USB_MAXCONFIG		8
struct usb_config_descriptor
{
    unsigned char  bLength;
    unsigned char  bDescriptorType;
    unsigned short wTotalLength;
    unsigned char  bNumInterfaces;
    unsigned char  bConfigurationValue;
    unsigned char  iConfiguration;
    unsigned char  bmAttributes;
    unsigned char  MaxPower;

    struct usb_interface *interface;

    unsigned char *extra;	/* Extra descriptors */
    int extralen;
};

/* Device descriptor */
//设备描述符
struct usb_device_descriptor
{
    unsigned char  bLength;
    unsigned char  bDescriptorType;
    unsigned short bcdUSB;
    unsigned char  bDeviceClass;
    unsigned char  bDeviceSubClass;
    unsigned char  bDeviceProtocol;
    unsigned char  bMaxPacketSize0;
    unsigned short idVendor;
    unsigned short idProduct;
    unsigned short bcdDevice;
    unsigned char  iManufacturer;
    unsigned char  iProduct;
    unsigned char  iSerialNumber;
    unsigned char  bNumConfigurations;
};

//usb控制设置
struct usb_ctrl_setup
{
    unsigned char  bRequestType;	//请求类型
    unsigned char  bRequest;		//请求
    unsigned short wValue;			//value
    unsigned short wIndex;			//index
    unsigned short wLength;			//长度
};

/*
 * Standard requests
 */
//标准请求

#define USB_REQ_GET_STATUS		    0x00	//获取状态
#define USB_REQ_CLEAR_FEATURE	    0x01	//清除feature？
/* 0x02 is reserved */						//0x02保留
#define USB_REQ_SET_FEATURE		    0x03	//设置feature
/* 0x04 is reserved */						//0x04保留
#define USB_REQ_SET_ADDRESS		    0x05	//设置地址
#define USB_REQ_GET_DESCRIPTOR		0x06	//获取描述符
#define USB_REQ_SET_DESCRIPTOR		0x07	//设置描述符
#define USB_REQ_GET_CONFIGURATION	0x08	//获取配置
#define USB_REQ_SET_CONFIGURATION	0x09	//设置配置
#define USB_REQ_GET_INTERFACE		  0x0A	//获取接口
#define USB_REQ_SET_INTERFACE		  0x0B	//设置接口
#define USB_REQ_SYNCH_FRAME		    0x0C	//同步帧

#define USB_TYPE_STANDARD		(0x00 << 5)	//标准USB
#define USB_TYPE_CLASS			(0x01 << 5)	//class
#define USB_TYPE_VENDOR			(0x02 << 5)	//vender
#define USB_TYPE_RESERVED		(0x03 << 5)	//保留类型

#define USB_RECIP_DEVICE		0x00		//设备
#define USB_RECIP_INTERFACE	0x01			//接口
#define USB_RECIP_ENDPOINT	0x02			//endpoint
#define USB_RECIP_OTHER			0x03		//其他
	
/*
 * Various libusb API related stuff
 */

#define USB_ENDPOINT_IN			0x80	//endpoint in
#define USB_ENDPOINT_OUT		0x00	//endpoint out

/* Error codes */
#define USB_ERROR_BEGIN			500000	//错误码

/*
 * This is supposed to look weird. This file is generated from autoconf
 * and I didn't want to make this too complicated.
 */
#define USB_LE16_TO_CPU(x)		//？？

/*
 * Device reset types for usb_reset_ex.
 * http://msdn.microsoft.com/en-us/library/ff537269%28VS.85%29.aspx
 * http://msdn.microsoft.com/en-us/library/ff537243%28v=vs.85%29.aspx
 */
#define USB_RESET_TYPE_RESET_PORT (1 << 0)	//reset 
#define USB_RESET_TYPE_CYCLE_PORT (1 << 1)	//reset 周期
#define USB_RESET_TYPE_FULL_RESET (USB_RESET_TYPE_CYCLE_PORT | USB_RESET_TYPE_RESET_PORT)	//全部重启


/* Data types */
/* struct usb_device; */
/* struct usb_bus; */

//usb设备
struct usb_device
{
    struct usb_device *next, *prev;

    char filename[LIBUSB_PATH_MAX];

    struct usb_bus *bus;

    struct usb_device_descriptor descriptor;
    struct usb_config_descriptor *config;

    void *dev;		/* Darwin support */

    unsigned char devnum;

    unsigned char num_children;
    struct usb_device **children;
};

//usb总线
struct usb_bus
{
    struct usb_bus *next, *prev;

    char dirname[LIBUSB_PATH_MAX];

    struct usb_device *devices;
    unsigned long location;

    struct usb_device *root_dev;
};

/* Version information, Windows specific */
//usb版本，仅windows支持
struct usb_version
{
    struct
    {
        int major;
        int minor;
        int micro;
        int nano;
    } dll;
    struct
    {
        int major;
        int minor;
        int micro;
        int nano;
    } driver;
};


struct usb_dev_handle;
typedef struct usb_dev_handle usb_dev_handle;

/* Variables */
#ifndef __USB_C__
#define usb_busses usb_get_busses()		//获取usb总线
#endif



#include <poppack.h>

//常用的usb函数接口
#ifdef __cplusplus
extern "C"
{
#endif

    /* Function prototypes */

    /* usb.c */
    usb_dev_handle *usb_open(struct usb_device *dev);	//打开usb
    int usb_close(usb_dev_handle *dev);					//关闭usb
    int usb_get_string(usb_dev_handle *dev, int index, int langid, char *buf,
                       size_t buflen);	//获取usb的string
    int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf,
                              size_t buflen);	//获取usb的string，简化版

    /* descriptors.c */
    int usb_get_descriptor_by_endpoint(usb_dev_handle *udev, int ep,
                                       unsigned char type, unsigned char index,
                                       void *buf, int size);	//根据endpoint获取usb描述符
    int usb_get_descriptor(usb_dev_handle *udev, unsigned char type,
                           unsigned char index, void *buf, int size);	//获取描述符

    /* <arch>.c */
    int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int size,
                       int timeout);	
    int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
                      int timeout);
    int usb_interrupt_write(usb_dev_handle *dev, int ep, char *bytes, int size,
                            int timeout);
    int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size,
                           int timeout);
    int usb_control_msg(usb_dev_handle *dev, int requesttype, int request,
                        int value, int index, char *bytes, int size,
                        int timeout);
    int usb_set_configuration(usb_dev_handle *dev, int configuration);//usb配置

    int usb_claim_interface(usb_dev_handle *dev, int interface);	  //声明接口
    int usb_release_interface(usb_dev_handle *dev, int interface);
    int usb_set_altinterface(usb_dev_handle *dev, int alternate);
    int usb_resetep(usb_dev_handle *dev, unsigned int ep);
    int usb_clear_halt(usb_dev_handle *dev, unsigned int ep);
    int usb_reset(usb_dev_handle *dev);
    int usb_reset_ex(usb_dev_handle *dev, unsigned int reset_type);

    char *usb_strerror(void);

    void usb_init(void);	//usb初始化
    void usb_set_debug(int level);	//设置比debug，作用是什么？
    int usb_find_busses(void);	//查找usb总线
    int usb_find_devices(void);	//查找设备
    struct usb_device *usb_device(usb_dev_handle *dev);
    struct usb_bus *usb_get_busses(void);


    /* Windows specific functions */

#define LIBUSB_HAS_INSTALL_SERVICE_NP 1
    int usb_install_service_np(void);	//安装服务 np
    void CALLBACK usb_install_service_np_rundll(HWND wnd, HINSTANCE instance,
            LPSTR cmd_line, int cmd_show);

#define LIBUSB_HAS_UNINSTALL_SERVICE_NP 1
    int usb_uninstall_service_np(void);	//卸载服务np
    void CALLBACK usb_uninstall_service_np_rundll(HWND wnd, HINSTANCE instance,
            LPSTR cmd_line, int cmd_show);

#define LIBUSB_HAS_INSTALL_DRIVER_NP 1
    int usb_install_driver_np(const char *inf_file);	//安装设备驱动np
    void CALLBACK usb_install_driver_np_rundll(HWND wnd, HINSTANCE instance,
            LPSTR cmd_line, int cmd_show);

#define LIBUSB_HAS_TOUCH_INF_FILE_NP 1
    int usb_touch_inf_file_np(const char *inf_file);//touch inf np
    void CALLBACK usb_touch_inf_file_np_rundll(HWND wnd, HINSTANCE instance,
            LPSTR cmd_line, int cmd_show);

#define LIBUSB_HAS_INSTALL_NEEDS_RESTART_NP 1
    int usb_install_needs_restart_np(void);

#define LIBUSB_HAS_INSTALL_NP 1
    int usb_install_npW(HWND hwnd, HINSTANCE instance, LPCWSTR cmd_line, int starg_arg);
    int usb_install_npA(HWND hwnd, HINSTANCE instance, LPCSTR cmd_line, int starg_arg);
	#define usb_install_np usb_install_npA
    void CALLBACK usb_install_np_rundll(HWND wnd, HINSTANCE instance, 
            LPSTR cmd_line, int cmd_show);

    const struct usb_version *usb_get_version(void);		//usb版本

    int usb_isochronous_setup_async(usb_dev_handle *dev, void **context,
                                    unsigned char ep, int pktsize);	//设置异步
    int usb_bulk_setup_async(usb_dev_handle *dev, void **context,
                             unsigned char ep);		//bulk设置异步
    int usb_interrupt_setup_async(usb_dev_handle *dev, void **context,
                                  unsigned char ep);	//中断设置 异步

    int usb_submit_async(void *context, char *bytes, int size);
    int usb_reap_async(void *context, int timeout);
    int usb_reap_async_nocancel(void *context, int timeout);
    int usb_cancel_async(void *context);
    int usb_free_async(void **context);


#ifdef __cplusplus
}
#endif

#endif /* __USB_H__ */

