comment ^
Module Name:
    ntddser.inc

Abstract:
    This is the include file that defines all constants and types for
    accessing the Serial device.

Author:
    Four-F (four-f@mail.ru)

Last update:
	09-July-2004
^

IFNDEF _NTDDSER_
_NTDDSER_ equ 1

IFNDEF CTL_CODE
CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM
ENDIF

; NtDeviceIoControlFile IoControlCode values for this device.

IOCTL_SERIAL_SET_BAUD_RATE      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_QUEUE_SIZE     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_LINE_CONTROL   equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 3,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_BREAK_ON       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 4,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_BREAK_OFF      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 5,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_IMMEDIATE_CHAR     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 6,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_TIMEOUTS       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 7,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_TIMEOUTS       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 8,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_DTR            equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 9,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_CLR_DTR            equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,10,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_RESET_DEVICE       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,11,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_RTS            equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,12,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_CLR_RTS            equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,13,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_XOFF           equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,14,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_XON            equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,15,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_WAIT_MASK      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,16,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_WAIT_MASK      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,17,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_WAIT_ON_MASK       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,18,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_PURGE              equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,19,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_BAUD_RATE      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,20,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_LINE_CONTROL   equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,21,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_CHARS          equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,22,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_CHARS          equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,23,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_HANDFLOW       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,24,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_HANDFLOW       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,25,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_MODEMSTATUS    equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,26,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_COMMSTATUS     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,27,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_XOFF_COUNTER       equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,28,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_PROPERTIES     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,29,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_DTRRTS         equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,30,METHOD_BUFFERED,FILE_ANY_ACCESS)

; Serenum reserves function codes between 128 and 255.  Do not use.

; begin_winioctl

IOCTL_SERIAL_LSRMST_INSERT      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,31,METHOD_BUFFERED,FILE_ANY_ACCESS)

IOCTL_SERENUM_EXPOSE_HARDWARE   equ CTL_CODE(FILE_DEVICE_SERENUM,128,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERENUM_REMOVE_HARDWARE   equ CTL_CODE(FILE_DEVICE_SERENUM,129,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERENUM_PORT_DESC         equ CTL_CODE(FILE_DEVICE_SERENUM,130,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERENUM_GET_PORT_NAME     equ CTL_CODE(FILE_DEVICE_SERENUM,131,METHOD_BUFFERED,FILE_ANY_ACCESS)

; end_winioctl

IOCTL_SERIAL_CONFIG_SIZE        equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,32,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_COMMCONFIG     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,33,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_COMMCONFIG     equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,34,METHOD_BUFFERED,FILE_ANY_ACCESS)

IOCTL_SERIAL_GET_STATS          equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,35,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_CLEAR_STATS        equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,36,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_GET_MODEM_CONTROL  equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,37,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_MODEM_CONTROL  equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,38,METHOD_BUFFERED,FILE_ANY_ACCESS)
IOCTL_SERIAL_SET_FIFO_CONTROL   equ CTL_CODE(FILE_DEVICE_SERIAL_PORT,39,METHOD_BUFFERED,FILE_ANY_ACCESS)


; internal serial IOCTL's

IOCTL_SERIAL_INTERNAL_DO_WAIT_WAKE      equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SERIAL_INTERNAL_CANCEL_WAIT_WAKE  equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SERIAL_INTERNAL_BASIC_SETTINGS    equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
IOCTL_SERIAL_INTERNAL_RESTORE_SETTINGS  equ CTL_CODE(FILE_DEVICE_SERIAL_PORT, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

SERIALPERF_STATS STRUCT
	ReceivedCount			DWORD	?
	TransmittedCount		DWORD	?
	FrameErrorCount			DWORD	?
	SerialOverrunErrorCount	DWORD	?
	BufferOverrunErrorCount	DWORD	?
	ParityErrorCount		DWORD	?
SERIALPERF_STATS ENDS
PSERIALPERF_STATS typedef ptr SERIALPERF_STATS

SERIALCONFIG STRUCT
	_Size			DWORD	?
	Version			WORD	?
					WORD	?	; padding
	SubType			DWORD	?
	ProvOffset		DWORD	?
	ProviderSize	DWORD	?
	ProviderData	WORD 1 dup(?)	; WCHAR
					WORD	?	; padding
SERIALCONFIG ENDS
PSERIALCONFIG typedef ptr SERIALCONFIG

; NtDeviceIoControlFile InputBuffer/OutputBuffer record structures for
; this device.

; This structure used to set line parameters.

SERIAL_LINE_CONTROL STRUCT
	StopBits	BYTE	?
	Parity		BYTE	?
	WordLength	BYTE	?
SERIAL_LINE_CONTROL ENDS
PSERIAL_LINE_CONTROL typedef ptr SERIAL_LINE_CONTROL

SERIAL_TIMEOUTS STRUCT
	ReadIntervalTimeout				DWORD	?
	ReadTotalTimeoutMultiplier		DWORD	?
	ReadTotalTimeoutConstant		DWORD	?
	WriteTotalTimeoutMultiplier		DWORD	?
	WriteTotalTimeoutConstant		DWORD	?
SERIAL_TIMEOUTS ENDS
PSERIAL_TIMEOUTS typedef ptr SERIAL_TIMEOUTS

; This structure used to resize the input/output buffers.
; An error code will be returned if the size exceeds the
; drivers capacity.  The driver reserves the right to
; allocate a larger buffer.

SERIAL_QUEUE_SIZE STRUCT
	InSize		DWORD	?
	OutSize		DWORD	?
SERIAL_QUEUE_SIZE ENDS
PSERIAL_QUEUE_SIZE typedef ptr SERIAL_QUEUE_SIZE

; This structure used by set baud rate

SERIAL_BAUD_RATE STRUCT
	BaudRate	DWORD	?
SERIAL_BAUD_RATE ENDS
PSERIAL_BAUD_RATE typedef ptr SERIAL_BAUD_RATE

; Defines the bitmask that the driver can used to notify
; app of various changes in the state of the UART.

SERIAL_EV_RXCHAR           equ 0001  ; Any Character received
SERIAL_EV_RXFLAG           equ 0002  ; Received certain character
SERIAL_EV_TXEMPTY          equ 0004  ; Transmitt Queue Empty
SERIAL_EV_CTS              equ 0008  ; CTS changed state
SERIAL_EV_DSR              equ 0010h  ; DSR changed state
SERIAL_EV_RLSD             equ 0020h  ; RLSD changed state
SERIAL_EV_BREAK            equ 0040h  ; BREAK received
SERIAL_EV_ERR              equ 0080h  ; Line status error occurred
SERIAL_EV_RING             equ 0100h  ; Ring signal detected
SERIAL_EV_PERR             equ 0200h  ; Printer error occured
SERIAL_EV_RX80FULL         equ 0400h  ; Receive buffer is 80 percent full
SERIAL_EV_EVENT1           equ 0800h  ; Provider specific event 1
SERIAL_EV_EVENT2           equ 1000h  ; Provider specific event 2

; +
; A longword is used to send down a mask that
; instructs the driver what to purge.
;
; SERIAL_PURGE_TXABORT - Implies the current and all pending writes.
; SERIAL_PURGE_RXABORT - Implies the current and all pending reads.
; SERIAL_PURGE_TXCLEAR - Implies the transmit buffer if exists
; SERIAL_PURGE_RXCLEAR - Implies the receive buffer if exists.
; -

SERIAL_PURGE_TXABORT equ 00000001
SERIAL_PURGE_RXABORT equ 00000002
SERIAL_PURGE_TXCLEAR equ 00000004
SERIAL_PURGE_RXCLEAR equ 00000008

; Communication defines

STOP_BIT_1      equ 0
STOP_BITS_1_5   equ 1
STOP_BITS_2     equ 2

NO_PARITY        equ 0
ODD_PARITY       equ 1
EVEN_PARITY      equ 2
MARK_PARITY      equ 3
SPACE_PARITY     equ 4


; +
; This structure is used to set and retrieve the special characters
; used by the nt serial driver.
;
; Note that the driver will return an error if xonchar == xoffchar.
; -

SERIAL_CHARS STRUCT
	EofChar		BYTE	?
	ErrorChar	BYTE	?
	BreakChar	BYTE	?
	EventChar	BYTE	?
	XonChar		BYTE	?
	XoffChar	BYTE	?
SERIAL_CHARS ENDS
PSERIAL_CHARS typedef ptr SERIAL_CHARS

; +
; This structure is used to contain the flow control
; and handshaking setup.
;
; A reasonably precise explaination of how they all
; work can be found in the OS/2 tech references.
;
; For Xon/Xofflimit:
;
; When there are more characters then
;
; (typeaheadbuffersize - xofflimit)
;
; in the typeahead buffer then the driver will perform all flow
; control that the app has enabled so that the sender will (hopefully)
; stop sending characters.
;
; When there are less than xonlimit number of characters in the
; typeahead buffer the driver will perform all flow control that
; the app has enabled so that the sender will hopefully start sending
; characters again.
;
; It should be noted that if Xoff character is sent then the
; driver will also stop transmitting any more characters.  This is to
; provide support for those systems that take any character that
; follows an Xoff as an implied Xon.
; -

SERIAL_HANDFLOW STRUCT
	ControlHandShake	DWORD	?
	FlowReplace			DWORD	?
	XonLimit			SDWORD	?
	XoffLimit			SDWORD	?
SERIAL_HANDFLOW ENDS
PSERIAL_HANDFLOW typedef ptr SERIAL_HANDFLOW

SERIAL_DTR_MASK           equ 03
SERIAL_DTR_CONTROL        equ 01
SERIAL_DTR_HANDSHAKE      equ 02
SERIAL_CTS_HANDSHAKE      equ 08
SERIAL_DSR_HANDSHAKE      equ 10h
SERIAL_DCD_HANDSHAKE      equ 20h
SERIAL_OUT_HANDSHAKEMASK  equ 38h
SERIAL_DSR_SENSITIVITY    equ 40h
SERIAL_ERROR_ABORT        equ 80000000h
SERIAL_CONTROL_INVALID    equ 7fffff84h
SERIAL_AUTO_TRANSMIT      equ 01
SERIAL_AUTO_RECEIVE       equ 02
SERIAL_ERROR_CHAR         equ 04
SERIAL_NULL_STRIPPING     equ 08h
SERIAL_BREAK_CHAR         equ 10h
SERIAL_RTS_MASK           equ 0c0h
SERIAL_RTS_CONTROL        equ 40h
SERIAL_RTS_HANDSHAKE      equ 80h
SERIAL_TRANSMIT_TOGGLE    equ 0c0h
SERIAL_XOFF_CONTINUE      equ 80000000h
SERIAL_FLOW_INVALID       equ 7fffff20h

; These are the following reasons that the device could be holding.

SERIAL_TX_WAITING_FOR_CTS      equ 00000001
SERIAL_TX_WAITING_FOR_DSR      equ 00000002
SERIAL_TX_WAITING_FOR_DCD      equ 00000004
SERIAL_TX_WAITING_FOR_XON      equ 00000008
SERIAL_TX_WAITING_XOFF_SENT    equ 00000010h
SERIAL_TX_WAITING_ON_BREAK     equ 00000020h
SERIAL_RX_WAITING_FOR_DSR      equ 00000040h

; These are the error values that can be returned by the driver.

SERIAL_ERROR_BREAK             equ 00000001
SERIAL_ERROR_FRAMING           equ 00000002
SERIAL_ERROR_OVERRUN           equ 00000004
SERIAL_ERROR_QUEUEOVERRUN      equ 00000008
SERIAL_ERROR_PARITY            equ 00000010h

; This structure is used by IOCTL_SERIAL_INTERNAL_BASIC_SETTINGS
; and IOCTL_SERIAL_INTERNAL_RESTORE_SETTINGS

SERIAL_BASIC_SETTINGS STRUCT
	Timeouts	SERIAL_TIMEOUTS	<>
	HandFlow	SERIAL_HANDFLOW	<>
	RxFifo		DWORD			?
	TxFifo		DWORD			?
SERIAL_BASIC_SETTINGS ENDS
PSERIAL_BASIC_SETTINGS typedef ptr SERIAL_BASIC_SETTINGS

; This structure is used to get the current error and
; general status of the driver.

SERIAL_STATUS STRUCT
	Errors				DWORD	?
	HoldReasons			DWORD	?
	AmountInInQueue		DWORD	?
	AmountInOutQueue	DWORD	?
	EofReceived			BOOLEAN	?
	WaitForImmediate	BOOLEAN	?
						db 2 dup(?)	; padding
SERIAL_STATUS ENDS
PSERIAL_STATUS typedef ptr SERIAL_STATUS

; +
; This structure is used for XOFF counter ioctl.  The xoff ioctl
; is used to support those subsystems that feel the need to emulate
; the serial chip in software.
;
; It has the following semantics:
;
; This io request is placed into the normal device write
; queue.  That is, it will be queued behind any writes
; already given to the driver.
;
; When this request becomes the current request, the character
; specified in the field XoffChar will be sent, subject to
; all other flow control already defined.
;
; Immediately upon sending the character the driver will
; perform the following actions.
;
; A timer will be initiated that will expire after the
; number of milliseconds in the Timeout field of the
; SERIAL_XOFF_COUNTER structure.
;
; The driver will initialize a counter to the value specified
; in the Counter field of the SERIAL_XOFF_RECORD.  The driver
; will decrement this counter whenever a character is received.
;
; This request will then be held by the driver.  It will
; actually complete under the following circumstances:
;
; 1) If there is another "write" request behind it in the queue.
;    The "xoff" request will be completed with the informational status
;    STATUS_SERIAL_MORE_WRITES.  The Information field of the
;    IOSTATUS block will be set to 0.
;
;    Note: By write request we mean another SERIAL_XOFF_COUNTER
;    request, or a simple write request.  If the only subsequent
;    request is a flush request, the driver WILL NOT automatically
;    complete the SERIAL_XOFF_COUNTER request.  NOTE: Transmit
;    immediate requests DO NOT count as a normal write, and therefore
;    would not cause a counter request to complete.
;
; 2) The timer expires.  The driver will complete the request
;    with the informational status STATUS_SERIAL_COUNTER_TIMEOUT.
;    The Information field of the IOSTATUS of the request will be set to 0.
;
; 3) The driver maintained counter goes to zero.  (By implication,
;    at least "Counter" number of characters have been received.)
;    The request will be completed with a successful status
;    of STATUS_SUCCESS.  The Information field of the
;    IOSTATUS of the request will be set to 0.
;
; 4) This is really a degenerate case of "1" above.  The request
;    is started and no request follow it on the queue.  However
;    at some point, before "2" or "3" above occur, another "write"
;    request is started.  This will cause the completion actions
;    stated in "1" to occur.
;
; NOTE: This request being issued WILL NOT cause the normal flow
;       control code of the driver to be invoked.
;
; NOTE: This request has no interaction with the IOCTL_SERIAL_WAIT_ON_MASK
;       request.  An application CAN NOT wait via the above ^^^^^^ ioctl
;       on the counter going to zero.  The application must synchronize
;       with the particular IOCTL_SERIAL_XOFF_COUNTER request.
;
; NOTE: The Timeout value equal to zero would cause the counter
;       to NEVER timeout.  The only way that such a request could
;       be killed at that point would be issue another write, or
;       to purge the WRITE queue.
; -

SERIAL_XOFF_COUNTER STRUCT
	Timeout		DWORD	?	; Zero based.  In milliseconds
	Counter		SDWORD	?	; Must be greater than zero.
	XoffChar	BYTE	?
				db 3 dup(?)	; padding
SERIAL_XOFF_COUNTER ENDS
PSERIAL_XOFF_COUNTER typedef ptr SERIAL_XOFF_COUNTER

; The following structure (and defines) are passed back by
; the serial driver in response to the get properties ioctl.

SERIAL_SP_SERIALCOMM         equ 00000001

; Provider subtypes

SERIAL_SP_UNSPECIFIED       equ 00000000
SERIAL_SP_RS232             equ 00000001
SERIAL_SP_PARALLEL          equ 00000002
SERIAL_SP_RS422             equ 00000003
SERIAL_SP_RS423             equ 00000004
SERIAL_SP_RS449             equ 00000005
SERIAL_SP_MODEM             equ 00000006
SERIAL_SP_FAX               equ 00000021h
SERIAL_SP_SCANNER           equ 00000022h
SERIAL_SP_BRIDGE            equ 00000100h
SERIAL_SP_LAT               equ 00000101h
SERIAL_SP_TELNET            equ 00000102h
SERIAL_SP_X25               equ 00000103h

; Provider capabilities flags.

SERIAL_PCF_DTRDSR        equ 0001
SERIAL_PCF_RTSCTS        equ 0002
SERIAL_PCF_CD            equ 0004
SERIAL_PCF_PARITY_CHECK  equ 0008
SERIAL_PCF_XONXOFF       equ 0010h
SERIAL_PCF_SETXCHAR      equ 0020h
SERIAL_PCF_TOTALTIMEOUTS equ 0040h
SERIAL_PCF_INTTIMEOUTS   equ 0080h
SERIAL_PCF_SPECIALCHARS  equ 0100h
SERIAL_PCF_16BITMODE     equ 0200h

; Comm provider settable parameters.

SERIAL_SP_PARITY         equ 0001
SERIAL_SP_BAUD           equ 0002
SERIAL_SP_DATABITS       equ 0004
SERIAL_SP_STOPBITS       equ 0008
SERIAL_SP_HANDSHAKING    equ 0010h
SERIAL_SP_PARITY_CHECK   equ 0020h
SERIAL_SP_CARRIER_DETECT equ 0040h

; Settable baud rates in the provider.

SERIAL_BAUD_075          equ 00000001h
SERIAL_BAUD_110          equ 00000002h
SERIAL_BAUD_134_5        equ 00000004h
SERIAL_BAUD_150          equ 00000008h
SERIAL_BAUD_300          equ 00000010h
SERIAL_BAUD_600          equ 00000020h
SERIAL_BAUD_1200         equ 00000040h
SERIAL_BAUD_1800         equ 00000080h
SERIAL_BAUD_2400         equ 00000100h
SERIAL_BAUD_4800         equ 00000200h
SERIAL_BAUD_7200         equ 00000400h
SERIAL_BAUD_9600         equ 00000800h
SERIAL_BAUD_14400        equ 00001000h
SERIAL_BAUD_19200        equ 00002000h
SERIAL_BAUD_38400        equ 00004000h
SERIAL_BAUD_56K          equ 00008000h
SERIAL_BAUD_128K         equ 00010000h
SERIAL_BAUD_115200       equ 00020000h
SERIAL_BAUD_57600        equ 00040000h
SERIAL_BAUD_USER         equ 10000000h

; Settable Data Bits

SERIAL_DATABITS_5        equ 0001
SERIAL_DATABITS_6        equ 0002
SERIAL_DATABITS_7        equ 0004
SERIAL_DATABITS_8        equ 0008
SERIAL_DATABITS_16       equ 0010h
SERIAL_DATABITS_16X      equ 0020h

; Settable Stop and Parity bits.

SERIAL_STOPBITS_10       equ 0001)
SERIAL_STOPBITS_15       equ 0002)
SERIAL_STOPBITS_20       equ 0004
SERIAL_PARITY_NONE       equ 0100h
SERIAL_PARITY_ODD        equ 0200h
SERIAL_PARITY_EVEN       equ 0400h
SERIAL_PARITY_MARK       equ 0800h
SERIAL_PARITY_SPACE      equ 1000h

SERIAL_COMMPROP STRUCT
	PacketLength		WORD	?
	PacketVersion		WORD	?
	ServiceMask			DWORD	?
	Reserved1			DWORD	?
	MaxTxQueue			DWORD	?
	MaxRxQueue			DWORD	?
	MaxBaud				DWORD	?
	ProvSubType			DWORD	?
	ProvCapabilities	DWORD	?
	SettableParams		DWORD	?
	SettableBaud		DWORD	?
	SettableData		WORD	?
	SettableStopParity	WORD	?
	CurrentTxQueue		DWORD	?
	CurrentRxQueue		DWORD	?
	ProvSpec1			DWORD	?
	ProvSpec2			DWORD	?
	ProvChar			WORD 1 dup(?)	; WCHAR
						WORD	?
SERIAL_COMMPROP ENDS
PSERIAL_COMMPROP typedef ptr SERIAL_COMMPROP

; Define masks for the rs-232 input and output.

SERIAL_DTR_STATE         equ 00000001
SERIAL_RTS_STATE         equ 00000002
SERIAL_CTS_STATE         equ 00000010h
SERIAL_DSR_STATE         equ 00000020h
SERIAL_RI_STATE          equ 00000040h
SERIAL_DCD_STATE         equ 00000080h

; begin_winioctl

; The following values follow the escape designator in the
; data stream if the LSRMST_INSERT mode has been turned on.

SERIAL_LSRMST_ESCAPE     equ 00

; Following this value is the contents of the line status
; register, and then the character in the RX hardware when
; the line status register was encountered.

SERIAL_LSRMST_LSR_DATA   equ 01

; Following this value is the contents of the line status
; register.  No error character follows

SERIAL_LSRMST_LSR_NODATA equ 02

; Following this value is the contents of the modem status
; register.

SERIAL_LSRMST_MST        equ 03

; Bit values for FIFO Control Register

SERIAL_IOC_FCR_FIFO_ENABLE      equ 00000001
SERIAL_IOC_FCR_RCVR_RESET       equ 00000002
SERIAL_IOC_FCR_XMIT_RESET       equ 00000004
SERIAL_IOC_FCR_DMA_MODE         equ 00000008
SERIAL_IOC_FCR_RES1             equ 00000010h
SERIAL_IOC_FCR_RES2             equ 00000020h
SERIAL_IOC_FCR_RCVR_TRIGGER_LSB equ 00000040h
SERIAL_IOC_FCR_RCVR_TRIGGER_MSB equ 00000080h

; Bit values for Modem Control Register

SERIAL_IOC_MCR_DTR              equ 00000001
SERIAL_IOC_MCR_RTS              equ 00000002
SERIAL_IOC_MCR_OUT1             equ 00000004
SERIAL_IOC_MCR_OUT2             equ 00000008
SERIAL_IOC_MCR_LOOP             equ 00000010h

; end_winioctl

; Serenum internal ioctl's
IFNDEF PHYSICAL_ADDRESS
PHYSICAL_ADDRESS UNION
	struct
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	struct u
		LowPart    DWORD ?
		HighPart   SDWORD ?
	ends
	QuadPart     QWORD ?	; signed
PHYSICAL_ADDRESS ENDS
PPHYSICAL_ADDRESS typedef PTR PHYSICAL_ADDRESS
ENDIF

SERENUM_PORT_DESC STRUCT
	_Size		DWORD			?	; IN sizeof (struct _PORT_DESC)
	PortHandle	PVOID			?	; OUT
	PortAddress	PHYSICAL_ADDRESS <>	; OUT
	Reserved	WORD 1 dup(?)
				WORD 3 dup(?)	; padding
SERENUM_PORT_DESC ENDS
PSERENUM_PORT_DESC typedef ptr SERENUM_PORT_DESC

; **************************************************************************
; Internal IOCTL interface for (pdo)
; The HID to legacy serial port minidriver uses this interface to
; find the address of the device.
; **************************************************************************

IOCTL_INTERNAL_SERENUM_REMOVE_SELF equ CTL_CODE(FILE_DEVICE_SERENUM, 129, METHOD_NEITHER, FILE_ANY_ACCESS)

; of which IO_STACK_LOCATION->Parameters.Others.Argument1 is set to
; a pointer to struct _SERENUM_SER_PARAMETERS

;typedef enum _SERENUM_PORTION {
    SerenumFirstHalf	equ 0
    SerenumSecondHalf	equ 1
    SerenumWhole		equ 2

SERENUM_PORT_PARAMETERS STRUCT
	_Size			DWORD	?	; IN sizeof (SERENUM_GET_PORT_PARAMETERS)

	ReadAccessor	PVOID	?	; OUT PSERENUM_READPORT read the serial port
	WriteAccessor	PVOID	?	; OUT PSERENUM_WRITEPORT write the serial port
	SerPortAddress	PVOID	?	; OUT token to read this serial port

	HardwareHandle	PVOID	?	; OUT a handle to this particular PDO.
	Portion			DWORD	?	; OUT SERENUM_PORTION
	NumberAxis		WORD	?	; OUT legacy joysticks only
	Reserved		WORD 3 dup(?)
SERENUM_PORT_PARAMETERS ENDS
PSERENUM_PORT_PARAMETERS typedef ptr SERENUM_PORT_PARAMETERS

ENDIF ; _NTDDSER_
