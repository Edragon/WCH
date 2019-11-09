;Filename: usbhidio.asm
;Version: 1.3
;Date: 11/5/99
;Copyright 1999 by Jan Axelson (jan@lvr.com)
;
;Chip: Cypress Semiconductor CY7C63001 USB Microcontroller
;Assembler: cyasm.exe
;Purpose: demonstrates USB communications with an HID-class device
;Description:
;Handles all required standard USB and HID-class requests.
;Receives data from the host in output reports
;using interrupt transfers on Endpoint 1.
;Sends data to the host in input reports
;using control transfers on Endpoint 0.
;(The chip doesn't support OUT transfers on Endpoint 1.)
;I used Cypress Semiconductor's js50.asm joystick example code as a base 
;in creating this program.
;The companion host software is the Visual-Basic project usbhidio.vbp.
;For more information, visit Lakeview Research at http://www.lvr.com .
;Send comments, bug reports, etc. to jan@lvr.com .

;Changes:

;V1.3: 11/20/99
;The length of the string descriptors is now correct.
;(Thanks, John Hyde)
;Changed the control_read routine to prevent error when sending a 
;multiple of 8 bytes.
;(Thanks, Dave Wright)

;V1.2:
;added watchdog resets in wait loops,
;took out the watchdog reset in the 1-msec. timer ISR.

;V1.1:
;Clears the watchdog only in the main routine
;(so the watchdog will detect if the main routine crashes).
;Additions to the comments.

;V1.0:
;Clears the Watchdog timer on 1-msec. interrupt.
;(Not needed on the development board, but needed for stand-alone.)
;The Endpoint 1 ISR now sets bit 7 of Endpoint 1 TX config to 1.
;(The bit is cleared on EP1 interrupt.)

;======================================================================
;assembler directives (equates)
;======================================================================

;----------------------------------------------------------------------
;I/O registers
;----------------------------------------------------------------------

;I/O ports
Port0_Data:            equ     00h      ; GPIO data port 0
Port1_Data:            equ     01h      ; GPIO data port 1
Port0_Interrupt:       equ     04h      ; Interrupt enable for port 0
Port1_Interrupt:       equ     05h      ; Interrupt enable for port 1
Port0_Pullup:          equ     08h      ; Pullup resistor control for port 0
Port1_Pullup:          equ     09h      ; Pullup resistor control for port 1

;USB ports
USB_EP0_TX_Config:      equ     10h     ; USB EP0 transmit configuration
USB_EP1_TX_Config:      equ     11h     ; USB EP1 transmit configuration
USB_Device_Address:     equ     12h     ; USB device address assigned by host
USB_Status_Control:     equ     13h     ; USB status and control register
USB_EP0_RX_Status:      equ     14h     ; USB EP0 receive status

;Control ports
Global_Interrupt:       equ     20h     ; Global interrupt enable
Watchdog:               equ     21h     ; clear watchdog Timer
Timer:                  equ     23h     ; free-running Timer

;GPIO Isink registers
Port0_Isink:            equ     30h
Port0_Isink0:           equ     30h
Port0_Isink1:           equ     31h
Port0_Isink2:           equ     32h
Port0_Isink3:           equ     33h
Port0_Isink4:           equ     34h
Port0_Isink5:           equ     35h
Port0_Isink6:           equ     36h
Port0_Isink7:           equ     37h
Port1_Isink:            equ     38h
Port1_Isink0:           equ     38h
Port1_Isink1:           equ     39h
Port1_Isink2:           equ     3Ah
Port1_Isink3:           equ     3Bh

;Control port
Status_Control:         equ     FFh

;----------------------------------------------------------------------
;Register bit values
;----------------------------------------------------------------------

;CPU Status and Control (Status_Control)
RunBit:                 equ     1h     ; CPU Run bit
USBReset:               equ    20h     ; USB Bus Reset bit
WatchDogReset:          equ    40h     ; Watchdog Reset bit

; USB EP1 transmit configuration (USB_EP1_TX_Config)
DataToggle:               equ     40h     ; Data 0/1 bit

DISABLE_REMOTE_WAKEUP:  equ   0         ; bit[1] = 0
ENABLE_REMOTE_WAKEUP:   equ   2         ; bit[1] = 1

;----------------------------------------------------------------------
;Interrupt masks
;----------------------------------------------------------------------

;The timer-only mask enables the 1-millisecond timer interrupt.
TIMER_ONLY:               equ      4h

;The enumerate mask enables the following interrupts:
;1-millisecond timer, USB Endpoint 0
ENUMERATE_MASK:          equ     0Ch

;The runtime mask enables the following interrupts:
;1-millisecond timer, USB Endpoint 0, USB Endpoint 1, GPIO
RUNTIME_MASK:          equ     5Ch

;----------------------------------------------------------------------
; USB Constants
; from the USB Spec v1.1
;----------------------------------------------------------------------

;standard request codes
get_status:             equ   0
clear_feature:          equ   1
set_feature:            equ   3
set_address:            equ   5
get_descriptor:         equ   6
set_descriptor:         equ   7
get_configuration:      equ   8
set_configuration:      equ   9
get_interface:          equ  10
set_interface:          equ  11
synch_frame:            equ  12

; standard descriptor types
device:              equ  1
configuration:       equ  2
string:              equ  3
interface:           equ  4
endpoint:            equ  5

; standard feature selectors
endpoint_stalled:       equ  0              ; recipient endpoint
device_remote_wakeup:   equ  1              ; recipient device

;----------------------------------------------------------------------
;HID-class descriptors
;from HID Class Definition v1.1 Draft
;----------------------------------------------------------------------

;Class-specific descriptor types from section 7.1 Standard Requests
HID:                    equ  21h
report:                 equ  22h
physical:               equ  23h
  
;Class-specific request codes from section 7.2 Class Specific Requests
get_report:             equ   1
get_idle:               equ   2
get_protocol:           equ   3
set_report:             equ   9
set_idle:               equ  10
set_protocol:           equ  11

;----------------------------------------------------------------------
;USB buffer bytes
;----------------------------------------------------------------------

;Control Endpoint 0 buffer
Endpoint_0:             equ  70h        ; control endpoint
Endpoint0_Byte0:        equ  70h        ; Endpoint 0, byte 0
Endpoint0_Byte1:        equ  71h        ; Endpoint 0 byte 1
Endpoint0_Byte2:        equ  72h        ; Endpoint 0 byte 2
Endpoint0_Byte3:        equ  73h        ; Endpoint 0 byte 3
Endpoint0_Byte4:        equ  74h        ; Endpoint 0 byte 4
Endpoint0_Byte5:        equ  75h        ; Endpoint 0 byte 5
Endpoint0_Byte6:        equ  76h        ; Endpoint 0 byte 6
Endpoint0_Byte7:        equ  77h        ; Endpoint 0 byte 7


;Endpoint 0 SETUP packet bytes
bmRequestType:          equ  70h
bRequest:               equ  71h
wValue:                 equ  72h        ; default wValue (8 bits)
wValueHi:               equ  73h
wIndex:                 equ  74h        ; default wIndex (8 bits)
wIndexHi:               equ  75h
wLength:                equ  76h        ; default wLength (8 bits)
wLengthHi:              equ  77h

;Endpoint 1 buffer
endpoint_1:             equ  78h
Endpoint1_Byte0:        equ  78h        ; Endpoint 1, byte 0
Endpoint1_Byte1:        equ  79h        ; Endpoint 1 byte 1
Endpoint1_Byte2:        equ  7Ah        ; Endpoint 1 byte 2
Endpoint1_Byte3:        equ  7Bh        ; Endpoint 1 byte 3
Endpoint1_Byte4:        equ  7Ch        ; Endpoint 1 byte 4
Endpoint1_Byte5:        equ  7Dh        ; Endpoint 1 byte 5
Endpoint1_Byte6:        equ  7Eh        ; Endpoint 1 byte 6
Endpoint1_Byte7:        equ  7Fh        ; Endpoint 1 byte 7

;----------------------------------------------------------------------
; Variables stored in data memory
;----------------------------------------------------------------------
;USB status
remote_wakeup_status:   equ  30h          ;0=disabled, 2-enabled
configuration_status:   equ  31h          ;0=unconfigured, 1=configured
;idle_status:           equ  33h          ;support SetIdle and GetIdle
protocol_status:        equ  34h          ;0=boot protocol, 1=report protocol

;Other variables:
suspend_counter:        equ  35h          ;number of idle bus milliseconds 
loop_temp:              equ  37h          ;temporary loop variable
start_send:             equ  32h          ;0=false, 1=true

;Received data:
Data_Byte0:               equ  38h
Data_Byte1:               equ  39h
Data_Byte2:               equ  3Ah
Data_Byte3:               equ  3Bh
Data_Byte4:               equ  3Ch
Data_Byte5:               equ  3Dh
Data_Byte6:               equ  3Eh
Data_Byte7:               equ  3Fh

temp:                   equ  25h
start_time:             equ  21h
testbit:                equ  22h
interrupt_mask:         equ  20h
endp0_data_toggle:      equ  23h
loop_counter:           equ  24h
data_start:             equ  27h
data_count:             equ  28h
endpoint_stall:         equ  29h

;======================================================================
;interrupt vectors
;======================================================================

org  00h                ; Reset vector; begin here after a reset.
jmp  Reset

org  02h                ; 128-microsecond interrupt
jmp  DoNothing_ISR

org  04h                ; 1024-millisecond interrupt 
jmp  One_mSec_ISR

org  06h                ; Endpoint 0 interrupt 
jmp  USB_EP0_ISR

org  08h                ; Endpoint 1 interrupt 
jmp  USB_EP1_ISR

org  0Ah                ; reserved interrupt 
jmp  Reset

org  0Ch                ; general purpose I/O interrupt 
jmp  GPIO_ISR           ; not used

org  0Eh                ; Wakeup_ISR or resume interrupt 
jmp  DoNothing_ISR      ; not used

ORG  10h

;======================================================================
;Interrupt routines
;======================================================================

;----------------------------------------------------------------------
; 128-microsecond interrupt, Cext
; Unused. If this interrupt occurs, just push the accumulator (because
; ipret pops it) and re-enable the interrupts.
;----------------------------------------------------------------------

DoNothing_ISR:
     push A
     ;Enable interrupts and return
     mov A,[interrupt_mask]
     ipret Global_Interrupt

;----------------------------------------------------------------------
; 1-millisecond interrupt
; Check to see if the chip is in suspend mode and take appropriate action.
; Copy values to Endpoint 1's buffer for sending.
;----------------------------------------------------------------------

One_mSec_ISR:
     push A
iowr Watchdog
;Find out if enumeration is complete.
;If enumerating is in progress, loop_temp = 0.
     mov A, [loop_temp]
     cmp A, 0h
;If enumeration is still in progress, jump.
     jz not_main
;Enumeration has ended, so decrement the loop counter
;(so it no longer = 0).
     dec [loop_temp]

not_main:                         
;Check for bus activity.
     iord USB_Status_Control
     and A, 01h
     cmp A,0h
;If no bus activity, increment the suspend counter.
     jz Inc_counter
;If bus activity detected, clear the bus-activity bit,
     iord USB_Status_Control
     and A, 0FEh
     iowr USB_Status_Control
;and clear the suspend counter.
     mov A, 0h
     mov [suspend_counter], A
     jmp Suspend_end

Inc_counter:
;Keep track of the amount of time with no bus activity.
     inc [suspend_counter]
;Get the number of milliseconds the bus has been idle.
     mov A, [suspend_counter]
;Has it been 3 milliseconds?
     cmp A, 03h
;If no, there's nothing else to do.
     jnz Suspend_end
;If yes, put the chip in Suspend mode.
;Clear the Suspend counter.
     mov A, 0h
     mov [suspend_counter], A
;Enable pullups on Port 1; disable the output DAC.
     mov A, 0h
     iowr Port1_Pullup
     mov A, 0ffh
     iowr Port1_Data
;Set the Suspend bit. 
     iord Status_Control
     or A, 08h
     iowr Status_Control
;The chip is now in Suspend mode. 
;On exiting Suspend mode, the chip will begin 
;executing instructions here:
     nop     
;Disable pullups on Port 1. Enable the output DAC.
     mov A, 0ffh
     iowr Port1_Pullup
     mov A, 0h
     iowr Port1_Data

Suspend_end:
;Is endpoint 1 enabled?
     iord USB_EP1_TX_Config
     cmp A,0
;If no, do nothing.
     jz Select
;If yes, is start_send = 1?
;(Start_send adds a short delay after enumeration.)
     mov A, [start_send]
     cmp A, 01h
;If no, do nothing
     jnz Select
;If yes, send data:
     jmp send_value

send_value:
;Copies values from RAM into Endpoint 1's buffer
;and enables sending the bytes on the next poll.

;disable Endpoint 1 interrupts
     mov A,[interrupt_mask]
     and A, EFh
     mov [interrupt_mask],A
     iowr Global_Interrupt

;Copy values from RAM to Endpoint 1's buffer for transmitting to the host.
;Two bytes:
     mov A, [Data_Byte0]
     mov [Endpoint1_Byte0], A
     mov A, [Data_Byte1]
     mov [Endpoint1_Byte1], A
;Add more bytes if the report format specifies it:
;     mov A, [Data_Byte2]
;     mov [Endpoint1_Byte2], A
;     mov A, [Data_Byte3]
;     mov [Endpoint1_Byte3], A
;     mov A, [Data_Byte4]
;     mov [Endpoint1_Byte4], A
;     mov A, [Data_Byte5]
;     mov [Endpoint1_Byte5], A
;     mov A, [Data_Byte6]
;     mov [Endpoint1_Byte6], A
;     mov A, [Data_Byte7]
;     mov [Endpoint1_Byte7], A

;Other things to try:
;Set the value at Port 0 to equal byte 0 in Endpoint 1's buffer:
;     iord Port0_Data
;     mov [Endpoint1_Byte0], A

;Or set a value here and copy to Endpoint 1's buffer, byte 1:
;     mov A, A5h
;      mov [Endpoint1_Byte1], A

;Configure Endpoint 1's transmit register 
;so that the bytes will transmit on the next poll.
     iord USB_EP1_TX_Config
;Don't change the Data 0/1 bit.
     and A,40h
;Set bits 4 and 7 to 1 enable transmitting.
;The low nibble is the number of data bytes (2).
     or A,92h
     iowr USB_EP1_TX_Config

Select:
;Enable Endpoint 1 interrupts.
     mov A,[interrupt_mask]
     or A, 10h
     mov [interrupt_mask],A
     ipret Global_Interrupt

;----------------------------------------------------------------------
;GPIO interrupt
;Can be configured to trigger when a port bit toggles.
;Unused here.
;----------------------------------------------------------------------

GPIO_ISR:
     push A
     push X

     pop X
     mov [interrupt_mask],A
     ipret Global_Interrupt

;----------------------------------------------------------------------
;Endpoint 1 ISR
;Endpoint 1 can do IN (device to host) transfers only.
;This interrupt triggers when the host acknowledges
;receiving data from Endpoint 1.
;The ISR toggles the data 0/1 bit for the next transaction and
;sets the EnableRespondToIN bit so the chip will respond to the 
;next poll of the endpoint.
;----------------------------------------------------------------------

USB_EP1_ISR:
     push A
;Toggle the data 0/1 bit so it's correct for the next transaction.
     iord USB_EP1_TX_Config  
     xor A,40h
;The interrupt clears the EnableRespondToIN bit (bit 7) in the TX Config.
;Set this bit to 1 so data will go out on the next poll.
;This will ensure that a ReadFile API call in a Windows application
;won't hang, waiting for the device to send something.
     or A, 92h
     iowr USB_EP1_TX_Config
;Enable interrupts and return.
     mov A, [interrupt_mask]
     ipret Global_Interrupt

;----------------------------------------------------------------------
; Reset processing
; Triggers on Reset or "reserved" interrupt.
;To be safe, initialize everything.
;----------------------------------------------------------------------

Reset:
;Place the data stack pointer at the lowest address of Endpoint 0's buffer.
;This keeps the stack from writing over the USB buffers.
;The USB buffers are in high RAM;
;the data stack pointer pre-decrements on a Push instruction.

     mov A, Endpoint_0
     swap A, dsp

;Initialize to FFh
     mov A, 0ffh
     iowr Port0_Data                 ; output ones to port 0
     iowr Port1_Pullup               ; disable port 1 pullups
                                     ; select rising edge interrupts
     iowr Port1_Isink0               ; maximum isink current Port1 bit 0
     iowr Port1_Isink1               ; maximum isink current Port1 bit 1
     iowr Port1_Isink2               ; maximum isink current Port1 bit 2
     iowr Port1_Isink3               ; maximum isink current Port1 bit 3

;Initialize to 00h
     mov A, 0h
     iowr Port1_Data                 ; output zeros to port 1
     iowr Port0_Interrupt            ; disable port 0 interrupts
     iowr Port0_Pullup               ; enable port 0 pullups
     iowr Port0_Isink0               ; minimum sink current Port0 bit 0
     iowr Port0_Isink1               ; minimum sink current Port0 bit 1
     iowr Port0_Isink2               ; minimum sink current Port0 bit 2
     iowr Port0_Isink3               ; minimum sink current Port0 bit 3
     iowr Port0_Isink4               ; minimum sink current Port0 bit 4
     iowr Port0_Isink5               ; minimum sink current Port0 bit 5
     iowr Port0_Isink6               ; minimum sink current Port0 bit 6
     iowr Port0_Isink7               ; minimum sink current Port0 bit 7
     mov [Endpoint1_Byte0],A
     mov [Endpoint1_Byte1],A
     mov [Endpoint1_Byte2],A
     mov [endpoint_stall], A
     mov [remote_wakeup_status], A
     mov [configuration_status], A
     mov [loop_temp], A
     mov [start_send], A
     iowr Watchdog                   ; clear watchdog timer

;Initialize values to transmit at Endpoint 1.
;     mov A, A5h
;     mov [Data_Byte0], A
;     mov A, F0h
;     mov [Data_Byte1], A

;Enable Port 1, bit 0 interrupts.
;     mov A, 01h
;     iowr Port1_Interrupt
;
;Test what kind of reset occurred: bus or watchdog?
     iord Status_Control
;Was it a bus reset?
     and A, USBReset
;If yes, jump to handle it.
     jnz BusReset
     iord Status_Control
;Was it a watchdog reset?
     and A, WatchDogReset
;If no, continue to wait for a bus reset
     jz suspendReset
;
;Watchog reset:
;A watchdog reset means that the watchdog timer
;wasn't cleared for 8.192 milliseconds.
;Wait for a bus reset to bring the system alive again.
;Enable 1-millisecond interrupt only
     mov A, TIMER_ONLY
     mov [interrupt_mask],A
     iowr Global_Interrupt
;Wait for a bus reset.
WatchdogHandler:
     jmp WatchdogHandler

suspendReset:
;Return to suspend mode to wait for a USB bus reset.
     mov A, 09h
     iowr Status_Control
     nop
     jmp suspendReset

BusReset:
;Clear all reset bits.
;Set bit 0 (the run bit).
     mov A, RunBit
     iowr Status_Control
;Set up for enumeration (Endpoint 0 and 1-millisecond interrupts enabled)
     mov A, ENUMERATE_MASK
     mov [interrupt_mask],A
     iowr Global_Interrupt

wait:
;Wait until configured.
     iord USB_EP1_TX_Config
     cmp A, 0
;Clear the watchdog timer
     iowr Watchdog
;If not configured, continue to wait.
     jz wait     
;When configured, initialize loop_temp.
;Loop_temp adds a delay in the start of transmission of data.
;The chip will respond to the first IN packet no sooner than 
;230 milliseconds after enumeration is complete.
;The delay was included in Cypress' joystick code to prevent problems 
;that occurred when power cycled off and on or the joystick was plugged
;in before the host powered up.
;I've left it in because it does no harm and 
;other hardware might have similar behavior.
;During the delay, the chip sends a NAK in response to any IN packet.
     mov A, 0ffh
     mov [loop_temp], A

;Enable endpoint 1
     iord USB_EP1_TX_Config
     or A, 92h
     iowr USB_EP1_TX_Config

;======================================================================
; The main program loop.
;======================================================================

main:
;Find out if the loop_temp delay has timed out.
;Loop_temp =0 if not timed out, FFh if timed out.
     mov A, [loop_temp]
     cmp A, 0Ah
;If no, don't enable transmitting.
     jnc no_set
;If yes, enable transmitting.
     mov A, 01h
     mov [start_send], A
no_set:
;Clear the watchdog timer.
;This has to be done at least once every 8 milliseconds!
     iowr Watchdog
     iord Port0_Data
nochange:
     jmp main

;----------------------------------------------------------------------
;The Endpoint 0 ISR supports the control endpoint.
;This code enumerates and configures the hardware.
;It also responds to Set Report requests that receive data from the host.
;----------------------------------------------------------------------

USB_EP0_ISR:
     push A
     iord USB_EP0_RX_Status
;Has a Setup packet been received?
     and A, 01h
;If no, find out if it's an OUT packet.
     jz check_for_out_packet
;If yes, handle it.
;Disable Endpoint 0 interrupts.
     mov A,[interrupt_mask]
     and A, 0F7h
     mov [interrupt_mask], A
     iowr Global_Interrupt
;Find out what the Setup packet contains and handle the request.
      call StageOne
;Re-enable Endpoint 0 interrupts.
     mov A, [interrupt_mask]
     or A, 08h
     mov [interrupt_mask], A
     jmp done_with_packet

check_for_out_packet:
     iord USB_EP0_RX_Status
;Is it an OUT packet?
     and A, 02h
;If no, ignore it.
     jz done_with_packet
;If yes, process the received data.
;Disable Endpoint 0 interrupts.
     mov A,[interrupt_mask]
     and A, 0F7h
     mov [interrupt_mask], A
     iowr Global_Interrupt

;For debugging: set Port 0, bit 1 to show that we're here.
;    iord Port0_Data
;    or a, 2
;    iowr Port0_Data

;Read the first byte in the buffer
     mov a, [Endpoint_0]
;For debugging: if the first byte =12h, bring Port 0, bit 0 high
;      cmp a, 12h
;      jnz not_a_match
;      iord Port0_Data
;      or a, 4
;      iowr Port0_Data

not_a_match:

;For debugging, add 1 to each byte read
;and copy the bytes to RAM.
;These bytes will be sent back to the host.

      push X
      ;data_count holds the number of bytes left to read.
      ;X holds the index of the address to read
      ;and the index of the address to store the received data.
      ;Initialize the X register.
      mov X, 0
      
      Get_Received_Data:
      ;Find out if there are any bytes to read.
      mov A, 0
      cmp A, [data_count]
      ;Jump if nothing to read.
      jz DoneWithReceivedData

      ;Get a byte.
      mov A, [X + Endpoint_0]
      ;For debugging, increment the received value.
      ;(Endpoint 1 will send it back to the host.)
      ;If the value is 255, reset to 0.
      ;Otherwise increment it.
      cmp A, 255
      jz ResetToZero
      inc A
      jmp NewValueSet
      ResetToZero:
      mov A, 0
      NewValueSet:
      ;Save the value.
      mov [X + Data_Byte0], A
      ;Decrement the number of bytes left to read.
      dec [data_count]
      ;Increment the address to read.
      inc X
      ;Do another
      jmp Get_Received_Data

      DoneWithReceivedData:
      pop X

;For debugging, set Port 0 to match the value written.
;     iowr Port0_Data

;Handshake by sending a 0-byte data packet.
call Send0ByteDataPacket

done_with_packet:
;Re-enable Endpoint 0 interrupts.
     mov A,[interrupt_mask]
     or A, 08h
     mov [interrupt_mask], A
     ipret Global_Interrupt

;========================================================================
;Control transfers
;========================================================================

;------------------------------------------------------------------------
;Control transfer, stage one.
;Find out whether the request is a standard device or HID-class request,
;the direction of data transfer, 
;and whether the request is to a device, interface, or endpoint.
;(from Table 9.2 in the USB spec)
;------------------------------------------------------------------------

StageOne:
;Clear the Setup flag
     mov A, 00h
     iowr USB_EP0_RX_Status
;Set the StatusOuts bit to cause auto-handshake after receiving a data packet.
     mov A, 8
     iowr USB_Status_Control
;bmRequestType contains the request.
      mov A, [bmRequestType]

;Standard device requests. From the USB spec.
; host to device requests
        cmp A, 00h
        jz RequestType00                 ; bmRequestType = 00000000 device
;       cmp A, 01h                       *** not required ***  
;       jz RequestType01                 ; bmRequestType = 00000001 interface
        cmp A, 02h              
        jz RequestType02                 ; bmRequestType = 00000010 endpoint
        cmp A, 80h             
; device to host requests
        jz RequestType80                 ; bmRequestType = 10000000 device
        cmp A, 81h
        jz RequestType81                 ; bmRequestType = 10000001 interface
        cmp A, 82h
        jz RequestType82                 ; bmRequestType = 10000010 endpoint

;HID-class device requests. From the HID spec
; host to device requests
        cmp A, 21h
        jz RequestType21                 ; bmRequestType = 00100001 interface
        cmp A, 22h                       ; *** not in HID spec ***
        jz RequestType22                 ; bmRequestType = 00100010 endpoint
; device to host requests
        cmp A, A1h
        jz RequestTypeA1                 ; bmRequestType = 10100001 interface

; Stall unsupported requests
SendStall:
      mov A, A0h
     iowr USB_EP0_TX_Config
      ret

;----------------------------------------------------------------------
;Control transfer, stage two
;Find out which request it is.
;----------------------------------------------------------------------

;Host to device with device as recipient
RequestType00:

;The Remote Wakeup feature is disabled on reset.
     mov A, [bRequest]     ; load bRequest
; Clear Feature                      bRequest = 1
     cmp A, clear_feature
     jz ClearRemoteWakeup 
; Set Feature                     bRequest = 3
     cmp A, set_feature
     jz SetRemoteWakeup

; Set the device address to a non-zero value.
; Set Address                     bRequest = 5
     cmp A, set_address
     jz SetAddress

; Set Descriptor is optional.
; Set Descriptor                bRequest = 7    *** not supported ***

;If wValue is zero, the device is not configured.
;The only other legal value for this firmware is 1.
;Set Configuration           bRequest = 9
    cmp A, set_configuration
    jz SetConfiguration

;Stall unsupported requests.
    jmp SendStall


;Host to device with interface as recipient    *** not required ***
; RequestType01:
;        mov A, [bRequest]       ; load bRequest

; There are no interface features defined in the spec.
; Clear Feature                 bRequest = 1    *** not supported ***
; Set Feature                   bRequest = 3    *** not supported ***

; Set Interface is optional.
; Set Interface                 bRequest = 11   *** not supported ***

;Stall unsupported requests.
;        jmp SendStall

;Host to device with endpoint as recipient
RequestType02:
     mov A, [bRequest]     ; load bRequest

; The only standard feature defined for an endpoint is endpoint_stalled.
; Clear Feature               bRequest = 1
     cmp A, clear_feature
     jz ClearEndpointStall
; Set Feature               bRequest = 3
     cmp A, set_feature
     jz SetEndpointStall
 
;Stall unsupported functions.
    jmp SendStall

;Device to host with device as recipient
RequestType80:
     mov A, [bRequest]          ; load bRequest

; Get Status               bRequest = 0
     cmp A, get_status
     jz GetDeviceStatus

; Get Descriptor               bRequest = 6
     cmp A, get_descriptor
     jz GetDescriptor

; Get Configuration          bRequest = 8
     cmp A, get_configuration
     jz GetConfiguration

;Stall unsupported requests.
     jmp SendStall

;Device to host with interface as recipient
RequestType81:
     mov A, [bRequest]     ; load bRequest

; Get Status               bRequest = 0
      cmp A, get_status
      jz GetInterfaceStatus

; Get Interface returns the selected alternate setting.
;  This firmware supports no alternate settings.
; Get Interface                 bRequest = 10   *** not supported ***

;The HID class defines one more request for bmRequestType=10000001
; Get Descriptor                bRequest = 6
      cmp A, get_descriptor
      jz GetDescriptor

;Stall unsupported functions
      jmp SendStall

;Device to host with endpoint as recipient
RequestType82:
     mov A, [bRequest]          ; load bRequest
; Get Status               bRequest = 0
     cmp A, get_status
      jz GetEndpointStatus

; Get Descriptor               bRequest = 6
     cmp A, get_descriptor
     jz GetDescriptor
; Sync Frame                  bRequest = 12   *** not supported ***

;Stall unsupported functions.
     jmp SendStall


;Check for HID class requests

;Host to device with endpoint as recipient
RequestType21:
      mov A, [bRequest]      ; load bRequest

; Set Report               bRequest = 9
     cmp A, set_report
     jz SetReport

; Set Idle                    bRequest = 10
      cmp A, set_idle
      jz SetIdle

; Set Protocol               bRequest = 11
      cmp A, set_protocol
      jz SetProtocol

;Stall unsupported requests
     jmp SendStall

RequestType22:
        mov A, [bRequest]      ; load bRequest

; Set Report               bRequest = 9
     cmp A, set_report     
     jz SetReport

;Stall unsupported requests
     jmp SendStall


;Device to host with endpoint as recipient
RequestTypeA1:
        mov A, [bRequest]      ; load bRequest

; Get Report               bRequest = 1
        cmp A, get_report
        jz GetReport

; Get Idle                 bRequest = 2
        cmp A, get_idle
        jz GetIdle

; Get Protocol             bRequest = 3
        cmp A, get_protocol
        jz GetProtocol

;Stall unsupported requests
        jmp SendStall

;----------------------------------------------------------------------
;Control transfer, stage three
;Process the request.
;----------------------------------------------------------------------

;The host controls whether or not a device can request a remote wakeup.

; Disable the remote wakeup capability.
ClearRemoteWakeup:
        mov A, [wValue]
        cmp A, device_remote_wakeup
        jnz SendStall
;Handshake by sending a data packet
        call Send0ByteDataPacket
        mov A, DISABLE_REMOTE_WAKEUP
        mov [remote_wakeup_status], A
        ret

; Enable the remote wakeup capability.
SetRemoteWakeup:
        mov A, [wValue]
        cmp A, device_remote_wakeup
;If not a match, stall.
        jnz SendStall
;Handshake by sending a 0-byte data packet
        call Send0ByteDataPacket
;Perform the request.
        mov A, ENABLE_REMOTE_WAKEUP
        mov [remote_wakeup_status], A
        ret

SetAddress:
; Set the device address to match wValue in the Setup packet.
;Complete the requested action after completing the transaction.
;Handshake by sending a 0-byte data packet.
        call Send0ByteDataPacket
;Perform the request
        mov A, [wValue]
        iowr USB_Device_Address
        ret

SetConfiguration:
;Unconfigured: wValue=0, configured: wValue=1.
;Also clear any stall condition and set Data 0/1 to Data0.
;Handshake by sending a 0-byte data packet.
      call Send0ByteDataPacket
;Save the configuration status.
      mov A, [wValue]
      mov [configuration_status], A
;Clear any stall condtion
      mov A, 0
      mov [endpoint_stall], A
;Set data 0/1 to Data0
      iord USB_EP1_TX_Config
      and A, ~DataToggle

;Set the configuration status.
     iowr USB_EP1_TX_Config     
     mov A, [configuration_status]
     cmp A, 0
;If configured, jump.
     jnz device_configured

;If unconfigured:
;Disable Endpoint 1
     iord USB_EP1_TX_Config
     and A, EFh
     iowr USB_EP1_TX_Config
;Disable Endpoint 1 interrupts.
     mov A, [interrupt_mask]
     and A, EFh
     mov [interrupt_mask], A
     jmp done_configuration

;If configured:
device_configured:
;Send NAK in response to IN packets
     iord USB_EP1_TX_Config
     and A,7Fh
;Enable Endpoint 1
     or A, 10h
     iowr USB_EP1_TX_Config
;Enable interrupts: Endpoint 1 and GPIO
     mov A, [interrupt_mask]
     or A, 50h
     mov [interrupt_mask], A
;Send NAK in response to Endpoint 0 OUT packets.
     iord USB_Status_Control
     and A,0EFh
     iowr USB_Status_Control
done_configuration:
        ret

ClearEndpointStall:
;Clear the stall (halt) condition for Endpoint 1.
;wValue = 0.
        mov A, [wValue]
        cmp A, endpoint_stalled
;If endpoint_stalled = 0, the endpoint isn't stalled
;and there's nothing to clear. Return a Stall for the request.
        jnz SendStall
;
;Clear Endpoint 1 stall
;Handshake by sending a 0-byte data packet
      call Send0ByteDataPacket
;Clear the stall condition
      mov A,0
      mov [endpoint_stall], A
;Set Data 0/1 to Data0
      iord USB_EP1_TX_Config
      and A, ~DataToggle
      iowr USB_EP1_TX_Config
;Send NAK in response to Endpoint 0 OUT packets.
      iord USB_Status_Control
      and A,0EFh
      iowr USB_Status_Control
      ret

;Stall Endpoint 1
SetEndpointStall:
;wValue = 0.
        mov A, [wValue]
        cmp A, endpoint_stalled
;If endpoint_stalled = 1, the endpoint is already stalled,
;so return a Stall for this request.
        jnz SendStall
;Handshake by sending a 0-byte data packet.
        call Send0ByteDataPacket
;Stall the endpoint.
        mov A,1         
        mov [endpoint_stall], A
        mov A, 30h
        iowr USB_EP1_TX_Config
        ret

GetDeviceStatus:
;Device Status is a 2-byte value.
;Bit 0 must be 0 (bus-powered).
;Bit 1 is remote wakeup: 0=disabled, 1=enabled.
;All other bits are unused.
;Return to status bytes to the host.
        mov A, 2
        mov [data_count], A
;control_read_table holds the two possible values for device status.
;Get the address of the first value.
        mov A, (get_dev_status_table - control_read_table)
;Add an index value to select the correct value.
        add A, [remote_wakeup_status]
;Send the value.
        jmp SendDescriptor

GetDescriptor:
;The high byte of wValue contains the descriptor type.
;The low byte of wValue contains the descriptor index.

        mov A, [wValueHi]               ; load descriptor type

;Test for standard descriptor types first.
;Supported descriptor types are device, configuration, string.
;Unsupported descriptor types are interface, endpoint.

; Get Descriptor (device)               wValueHi = 1
     cmp A, device
     jz GetDeviceDescriptor
; Get Descriptor (configuration)        wValueHi = 2
     cmp A, configuration
     jz GetConfigurationDescriptor
; Get Descriptor (string)               wValueHi = 3
     cmp A, string
     jz GetStringDescriptor

; Test for HID-class descriptor types.
; Get Descriptor (HID)                  wValueHi = 21h
        cmp A, HID
        jz GetHIDDescriptor
; Get Descriptor (report)               wValueHi = 22h  
     cmp A, report
     jz GetReportDescriptor
; Get Descriptor (physical)             wValueHi = 23h  *** not supported ***
;Stall unsupported requests.
     jmp SendStall

GetConfiguration:
;Send the current device configuration.
;0 = unconfigured, 1 = configured.

;Send 1 byte
        mov A, 1
        mov [data_count], A
;Get the address of the data to send.
        mov A, (get_configuration_status_table - control_read_table)
;Add an index to point to the correct configuration.
        add A, [configuration_status]
;Send the data.
        jmp SendDescriptor

GetInterfaceStatus:
;Interface status is 2 bytes, which are always 0.
;Send 2 bytes.
        mov A, 2
        mov [data_count], A
;Get the address of the data to send.
        mov A, (get_interface_status_table - control_read_table)
;Send the data.
        jmp SendDescriptor

GetEndpointStatus:
;Endpoint status is 2 bytes.
;Bit 0 = 0 when the endpoint is not stalled.
;Bit 0 = 1 when the endpoint is stalled.
;All other bits are unused.
;Send 2 bytes.
        mov A, 2
        mov [data_count], A
;Get the stall status.
        mov A, [endpoint_stall]
;Shift left to get an index (0 or 2) to point to data
;in the endpoint status table
        asl A
;Get the address of the data to send.
        add A, (get_endpoint_status_table - control_read_table)
;Send the data.
        jmp SendDescriptor

SetReport:
;The CY7C63000 doesn't support interrupt-mode OUT transfers.
;So the host uses Control transfers with Set_Report requests
;to get data from the device.

;Get the report data.

;For debugging: set Port 0, bit 0 =1 to show that we're here.
;      iord Port0_Data
;      or a, 1
;      iowr Port0_Data

;Find out how many bytes to read. This value is in WLength.
;Save the length in data_count.
     mov A, [wLength]
     mov [data_count], A

;Enable receiving data at Endpoint 0 by setting the EnableOuts bit
;The bit clears following any Setup or OUT transaction.
     iord USB_Status_Control
     or A, 10h
;Clear the StatusOuts bit to disable automatic sending of ACK after
;receiving a valid status packet in a Control read (IN) transfer.
;Otherwise, the USB engine will respond to a data OUT packet with a Stall.
     and A, F7h
     iowr USB_Status_Control
;Now we're ready to receive the report data.
;An Endpoint 0 OUT interrupt indicates the arrival of the report data.
ret

SetIdle:
        jmp SendStall   ; *** not supported ***

SetProtocol:
;Switches between a boot protocol (wValue=0) and report protocol (wValue=1).
;This firmware doesn't distinguish between protocols.
        mov A, [wValue]
        mov [protocol_status], A
        call Send0ByteDataPacket
        ret

GetReport:
;Sends a report to the host.
;The high byte of wValue contains the report type. 
;The low byte of wValue contains the report ID.
;Not supported (Use interrupt transfers to send data.)
     jmp SendStall

GetReportDescriptor:
;Save the descriptor length
     mov A, (end_hid_report_desc_table - hid_report_desc_table)
     mov [data_count], A
;Get the descriptor's starting address.
     mov A, (hid_report_desc_table - control_read_table)
     call SendDescriptor

      ret

GetIdle:
;Not supported
        jmp SendStall

GetProtocol:
;Send the current protocol status.
;Send 1 byte.
        mov A, 1
        mov [data_count], A
;Get the address of the data to send.
        mov A, (get_protocol_status_table - control_read_table)
;Add an index that points to the correct data.
        add A, [protocol_status]
;Send the data.
        jmp SendDescriptor
        
; Standard Get Descriptor routines
;
;Send the device descriptor.
GetDeviceDescriptor:
;Get the length of the descriptor
;(stored in the first byte in the device descriptor table).
        mov A, 0
       index device_desc_table
        mov [data_count], A
;Get the starting address of the descriptor.
        mov A, (device_desc_table - control_read_table)
;Send the descriptor.
        jmp SendDescriptor

GetConfigurationDescriptor:
;Send the configuration descriptor.
;Get the length of the descriptor.
     mov A, (end_config_desc_table - config_desc_table)
      mov [data_count], A
;Get the starting address of the descriptor.
     mov A, (config_desc_table - control_read_table)
;Send the descriptor.
     jmp SendDescriptor

GetStringDescriptor:
;Use the string index to find out which string it is.
     mov A, [wValue]
     cmp A, 0h
     jz LanguageString
     cmp A, 01h
     jz ManufacturerString
     cmp A, 02h
     jz ProductString
;     cmp A, 03h
;     jz SerialNumString
;     cmp A, 04h
;     jz ConfigurationString
;     cmp A, 05h
;     jz InterfaceString
; No other strings supported
      jmp SendStall

SendDescriptor:
;The starting address of the descriptor is in the accumulator. Save it.
        mov [data_start], A
;Get the descriptor length.
        call get_descriptor_length
;Send the descriptor.
        call control_read
        ret

;Send the requested string.
;For each, store the descriptor length in data_count, then send the descriptor.
LanguageString:
     mov A, (USBStringDescription1 - USBStringLanguageDescription)
      mov [data_count], A
     mov A, (USBStringLanguageDescription - control_read_table)
      jmp SendDescriptor
ManufacturerString:     
     mov A, ( USBStringDescription2 - USBStringDescription1)
      mov [data_count], A
     mov A, (USBStringDescription1 - control_read_table)
      jmp SendDescriptor
ProductString:
     mov A, ( USBStringDescription3 - USBStringDescription2)
      mov [data_count], A
     mov A, (USBStringDescription2 - control_read_table)
      jmp SendDescriptor
;SerialNumString:
;     mov A, ( USBStringDescription4 - USBStringDescription3)
;      mov [data_count], A
;     mov A, (USBStringDescription3 - control_read_table)
;      jmp SendDescriptor
;ConfigurationString:
;     mov A, ( USBStringDescription5 - USBStringDescription4)
;      mov [data_count], A
;     mov A, (USBStringDescription4 - control_read_table)
;      jmp SendDescriptor
;InterfaceString:
;     mov A, ( USBStringEnd - USBStringDescription5)
;      mov [data_count], A
;     mov A, (USBStringDescription5 - control_read_table)
;      jmp SendDescriptor

; HID class Get Descriptor routines
;
GetHIDDescriptor:
;Send the HID descriptor.
;Get the length of the descriptor.
     mov A, (Endpoint_Descriptor - Class_Descriptor)
      mov [data_count], A
;Get the descriptor's starting address.
     mov A, ( Class_Descriptor - control_read_table)
;Send the descriptor.
      call SendDescriptor
      ret

;======================================================================
;USB support routines
;======================================================================

get_descriptor_length:
;The host sometimes lies about the number of bytes it
;wants from a descriptor.
;A request to get a descriptor should return the smaller of the number 
;of bytes requested or the actual length of the descriptor.
;Get the requested number of bytes to send
     mov A, [wLengthHi]
;If the requested high byte is >0, 
;ignore the high byte and use the firmware's value.
;(255 bytes is the maximum allowed.)
     cmp A, 0
     jnz use_actual_length
;If the low byte =0, use the firmware's value.
     mov A, [wLength]
     cmp A, 0
     jz use_actual_length
;If the requested number of bytes => the firmware's value,
;use the firmware's value.
     cmp A, [data_count]
     jnc use_actual_length
;If the requested number of bytes < the firmware's value,
;use the requested number of bytes.
     mov [data_count], A
use_actual_length:
        ret

Send0ByteDataPacket:
;Send a data packet with 0 bytes.
;Use this handshake after receiving an OUT data packet.
;Enable responding to IN packets and set Data 0/1 to Data 1.
     mov A, C0h
     iowr USB_EP0_TX_Config
;Enable interrupts.
     mov A, [interrupt_mask]
     iowr Global_Interrupt
WaitForDataToTransfer:
;Wait for the data to transfer.
;Clear the watchdog timer
     iowr Watchdog
;Bit 7 of USB_EP0_TX_Config is cleared when the host acknowledges
;receiving the data.
     iord USB_EP0_TX_Config
     and A, 80h
     jnz WaitForDataToTransfer
     ret

control_read: 
;Do a Control Read transfer.
;The device receives a Setup packet in the Setup stage,
;sends 1 or more data packets (IN) in the Data stage,
;and receives a 0-length data packet (OUT) in the Status stage.
;Before calling this routine, the firmware must set 2 values:
;data_start is the starting address of the descriptor to send,
;expressed as an offset from the control read table.
;data_count is the number of bytes in the descriptor.
     push X
;Set the Data 0/1 bit to 0.
     mov A, 00h
     mov [endp0_data_toggle], A

control_read_data_stage:
;Initialize count variables.
     mov X, 00h
     mov A, 00h
     mov [loop_counter], A

;Clear the Setup bit.
     iowr USB_EP0_RX_Status
;Check the Setup bit.
     iord USB_EP0_RX_Status
     and A, 01h
;If not cleared, another setup packet has arrived, 
;so exit the routine.
     jnz control_read_status_stage

;Set the StatusOuts bit to 1 to cause the device to automatically return
;ACK in response to a received OUT packet in the Status stage.
     mov A, 08h
     iowr USB_Status_Control
;If there is no data to send, prepare a 0-length data packet.
;(The host might require a final 0-length packet if the descriptor is 
;a multiple of 8 bytes.)
     mov A, [data_count]
     cmp A, 00h
     jz dma_load_done

dma_load_loop:
;Copy up to 8 bytes for transmitting into Endpoint 0's buffer
;and increment/decrement the various counting variables.

;Place the byte to send in the accumulator:
;(control_read_table) + (data_start).
     mov A, [data_start]
     index control_read_table
;Place the byte in Endpoint 0's buffer.
     mov [X + Endpoint_0], A
;Increment the offset of the data being sent.
     inc [data_start]
;Increment the offset of Endpoint 0's buffer.
     inc X
;Increment the number of bytes stored in the buffer.
     inc [loop_counter]
;Decrement the number of bytes left to send.
     dec [data_count]
;If the count = 0, there's no more data to load.
     jz dma_load_done
;If 8 bytes haven't been loaded into the buffer, get another byte.
;If 8 bytes have been loaded, it's the maximum for the transaction,
;so send the data.
     mov A, [loop_counter]
     cmp A, 08h
     jnz dma_load_loop

dma_load_done:
;Send the data.

;Check the Setup bit.
;If it's not 0, another Setup packet has arrived, 
;so exit the routine.
     iord USB_EP0_RX_Status
     and A, 01h
     jnz control_read_status_stage

;Set the bits in the USB_EP0_TX_Config register.
;Toggle the Data 0/1 bit.
     mov A, [endp0_data_toggle]
     xor A, 40h
     mov [endp0_data_toggle], A
;Enable responding to IN token packets.
     or A, 80h
;The low 4 bits hold the number of bytes to send.
     or A, [loop_counter]
     iowr USB_EP0_TX_Config
;Enable interrupts
     mov A, [interrupt_mask]
     iowr Global_Interrupt

wait_control_read:

;Clear the watchdog timer
     iowr Watchdog

;Wait for the data to transfer and the host to acknowledge,
;indicated by Bit 7 = 0.
     iord USB_EP0_TX_Config
     and A, 80h
;When all of the transaction's data has transferred, 
;find out if there is more data to send in the transfer.
     jz control_read_data_stage
;Find out if the host has sent an OUT packet to acknowledge 
;and end the transfer.
     iord USB_EP0_RX_Status
     and A, 02h
     jz wait_control_read

control_read_status_stage:
;The transfer is complete.
     pop X
     mov A, [interrupt_mask]
     iowr Global_Interrupt
     ret

;======================================================================
;Lookup Tables
;Contain the descriptors and the codes for status indicators.
;The firmware accesses the information by referencing a specific
;table's address as an offset from the control_read_table.
;======================================================================

control_read_table:

device_desc_table:
     db 12h          ; Descriptor length (18 bytes)
     db 01h          ; Descriptor type (Device)
     db 10h,01h      ; Complies with USB Spec. Release (0110h = release 1.10)
     db 00h          ; Class code (0)
     db 00h          ; Subclass code (0)
     db 00h          ; Protocol (No specific protocol)
     db 08h          ; Maximum packet size for Endpoint 0 (8 bytes)
     db 25h,09h      ; Vendor ID (Lakeview Research, 0925h)
     db 34h,12h      ; Product ID (1234)
     db 01h,00h      ; Device release number (0001)
     db 01h          ; Manufacturer string descriptor index
     db 02h          ; Product string descriptor index
     db 00h          ; Serial Number string descriptor index (None)
     db 01h          ; Number of possible configurations (1)
end_device_desc_table:

config_desc_table:
     db 09h          ; Descriptor length (9 bytes)
     db 02h          ; Descriptor type (Configuration)
     db 22h,00h      ; Total data length (34 bytes)
     db 01h          ; Interface supported (1)
     db 01h          ; Configuration value (1)
     db 00h          ; Index of string descriptor (None)
     db 80h          ; Configuration (Bus powered)
     db 32h          ; Maximum power consumption (100mA)

Interface_Descriptor:
     db 09h          ; Descriptor length (9 bytes)
     db 04h          ; Descriptor type (Interface)
     db 00h          ; Number of interface (0) 
     db 00h          ; Alternate setting (0)
     db 01h          ; Number of interface endpoint (1)
     db 03h          ; Class code ()                    
     db 00h          ; Subclass code ()                 
     db 00h          ; Protocol code ()
     db 00h          ; Index of string()       

Class_Descriptor:
     db 09h          ; Descriptor length (9 bytes)
     db 21h          ; Descriptor type (HID)
     db 00h,01h      ; HID class release number (1.00)
     db 00h          ; Localized country code (None)
     db 01h          ; # of HID class dscrptr to follow (1)
     db 22h          ; Report descriptor type (HID)
                     ; Total length of report descriptor
     db (end_hid_report_desc_table - hid_report_desc_table),00h

Endpoint_Descriptor:
     db 07h          ; Descriptor length (7 bytes)
     db 05h          ; Descriptor type (Endpoint)
     db 81h          ; Encoded address (Respond to IN, 1 endpoint)
     db 03h          ; Endpoint attribute (Interrupt transfer)
     db 06h,00h      ; Maximum packet size (6 bytes)
     db 0Ah          ; Polling interval (10 ms)
      
end_config_desc_table:

;----------------------------------------------------------------------
;The HID-report descriptor table
;----------------------------------------------------------------------

hid_report_desc_table:   
     db 06h, A0h, FFh      ;         Usage Page (vendor defined) FFA0
     db 09h, 01h     ;               Usage (vendor defined)
     db A1h, 01h     ;               Collection (Application)
     db 09h, 02h     ;               Usage (vendor defined)
     db A1h, 00h     ;               Collection (Physical)
     db 06h, A1h, FFh ;              Usage Page (vendor defined) 

;The input report
     db 09h, 03h     ;               usage - vendor defined
     db 09h, 04h     ;               usage - vendor defined
     db 15h, 80h     ;               Logical Minimum (-128)
     db 25h, 7Fh     ;               Logical Maximum (127)
     db 35h, 00h     ;               Physical Minimum (0)
     db 45h, FFh;                    Physical Maximum (255)
;    db 66h, 00h, 00h;               Unit (None (2 bytes))
     db 75h, 08h     ;               Report Size (8)  (bits)
     db 95h, 02h     ;               Report Count (2)  (fields)
     db 81h, 02h     ;               Input (Data, Variable, Absolute)  

;The output report
     db 09h, 05h     ;               usage - vendor defined
     db 09h, 06h     ;               usage - vendor defined
     db 15h, 80h     ;               Logical Minimum (-128)
     db 25h, 7Fh     ;               Logical Maximum (127)
     db 35h, 00h     ;               Physical Minimum (0)
     db 45h, FFh     ;               Physical Maximum (255)
;    db 66h, 00h, 00h;               Unit (None (2 bytes))
     db 75h, 08h     ;               Report Size (8)  (bits)
     db 95h, 02h     ;               Report Count (2)  (fields)
     db 91h, 02h     ;               Output (Data, Variable, Absolute)  

     db C0h          ;               End Collection

     db C0h          ;               End Collection

end_hid_report_desc_table:

;----------------------------------------------------------------------
;String Descriptors
;----------------------------------------------------------------------

;Define the strings

; string 0
USBStringLanguageDescription:
    db 04h          ; Length
    db 03h          ; Type (3=string)
    db 09h          ; Language:  English
    db 04h          ; Sub-language: US
; string 1

;The Length value for each string =
;((number of characters) * 2) + 2

USBStringDescription1:     ; IManufacturerName
    db 1Ah          ; Length
    db 03h          ; Type (3=string)
    dsu "USB Complete" ;

; string 2
USBStringDescription2:     ; IProduct
    db 16h          ; Length
    db 03h          ; Type (3=string)
    dsu "HID Sample"  ;

;string 3
;If the firmware contains a serial number, it must be unique
;for each device or the devices may not enumerate properly.
USBStringDescription3:     ; serial number

; string 4                 
;USBStringDescription4:     ; configuration string descriptor
;    db 16h          ; Length
;    db 03h          ; Type (3=string)
;    dsu "Sample HID"  ;

;string 5
;USBStringDescription5:     ; configuration string descriptor
;    db 32h          ; Length
;    db 03h          ; Type (3=string)
;    dsu "EndPoint1 Interrupt Pipe"  ;

USBStringEnd:

;----------------------------------------------------------------------
;Status information.
;The status can apply to the device or an interface or endpoint.
;An index selects the correct value.
;----------------------------------------------------------------------
get_dev_status_table:
        db      00h, 00h        ; remote wakeup disabled, bus powered
        db      02h, 00h        ; remote wakeup enabled, bus powered
get_interface_status_table:
        db      00h, 00h        ; always return both bytes zero
get_endpoint_status_table:
        db      00h, 00h        ; not stalled
        db      01h, 00h        ; stalled
get_configuration_status_table:
        db      00h             ; not configured
        db      01h             ; configured
get_protocol_status_table:
        db      00h             ; boot protocol
        db      01h             ; report protocol

