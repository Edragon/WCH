#include <stdio.h> 
#include <string.h> 
#include <iostream>
#include "conio.h"
#include "lusb0_usb.h"

#define m_dev_VENDOR    0x0483	/* Vendor ID of the m_dev         */ 
#define m_dev_PRODUCT   0x5710	/* Product ID of the m_dev        */ 
#define m_dev_DATA_SIZE 4       /* Number of bytes to get at once */ 

int main(int argc, char *argv[]) 
{ 
	int ret, i; 
	char data[m_dev_DATA_SIZE] = {0}; 
	struct usb_bus *bus; 
	struct usb_device * m_dev = NULL; 
	struct usb_dev_handle *m_dev_handle; 
	char str[64];
	usb_init(); 
	ret = usb_find_busses(); 
	if(ret<0) 
	{ 
		printf("usb_find_busses failed - returned: %i\n", ret); 
		return ret; 
	} 
	ret = usb_find_devices(); 
	if(ret<0) 
	{ 
		printf("usb_find_devices failed - returned: %i\n", ret); 
		return ret; 
	} 
  
	for(bus = usb_get_busses(); bus; bus = bus->next) 
	{ 
		struct usb_device *dev; 
		for(dev = bus->devices; dev; dev = dev->next) 
		{ 
			if(dev->descriptor.idVendor == m_dev_VENDOR && dev->descriptor.idProduct == m_dev_PRODUCT) 
			m_dev=dev; 
		} 
	} 
	if(!m_dev) 
	{ 
		printf("m_dev not found\n"); 
		return 1; 
	} 
  
	m_dev_handle = usb_open(m_dev); 
	if(!m_dev_handle) 
	{ 
	printf("Could not open m_dev\n"); 
	return 1; 
	} 
  
	printf("m_dev opened: %s\n", m_dev->filename); 
	printf("Device Protocol : %d\n",m_dev->descriptor.bDeviceProtocol);
	printf("Report Length : %d\n",m_dev->descriptor.bLength);
	printf("Decriptor Type : %d\n",m_dev->descriptor.bDescriptorType);
	printf("End Points : %d\n",m_dev->config->interface->altsetting->bNumEndpoints);
	printf("Interface Class : %d\n",m_dev->config->interface->altsetting->bInterfaceClass);
	printf("Protocol : %d\n",m_dev->config->interface->altsetting->bInterfaceProtocol);
	printf("Interface Number: %d\n",m_dev->config->interface->altsetting->bInterfaceNumber);
	printf("Device Filename : %s\n",m_dev->filename);
      
	usb_get_string_simple(m_dev_handle, m_dev->descriptor.iManufacturer, str, sizeof(str));
	printf("Device Manfucaturer : %s\n",str);
	usb_get_string_simple(m_dev_handle,m_dev->descriptor.iProduct,str,sizeof(str));
	printf("Product Name : %s\n",str);
	usb_get_string_simple(m_dev_handle,m_dev->descriptor.iSerialNumber,str,sizeof(str));
	printf("Device Serial Number: %s\n",str);
	printf("End point addresses : 0x%x\n",m_dev->config->interface->altsetting->endpoint->bEndpointAddress);

		//return 0;
	if(usb_set_configuration(m_dev_handle, 1) < 0) 
	{ 
		printf("Could not set configuration\n"); 
		usb_close(m_dev_handle); 
		return 1; 
	} 

	if(usb_claim_interface(m_dev_handle, 0) < 0) //claim_interface 0指向第一个设备
	{ 
		printf("Could not claim interface\n"); 
		usb_close(m_dev_handle); 
		return 1; 
	} 
	
	char key = '\0';
	while(key != 'q') 
	{ 
		ret = usb_interrupt_read(m_dev_handle, 0x81, data, m_dev_DATA_SIZE, 0); 
		//ret = usb_bulk_read(m_dev_handle, 0x81, data, m_dev_DATA_SIZE, 0); 
		printf("ret: %i: ", ret); 
		for(i = 0; i< ret; i++) 
		{
			printf("%hhd ", (unsigned char)(data[i])); 
		}
		printf("\n"); 
	} 
  
	usb_release_interface(m_dev_handle, 0); 
	usb_close(m_dev_handle); 
	printf("m_dev closed\n"); 
  
	return 0; 
} 