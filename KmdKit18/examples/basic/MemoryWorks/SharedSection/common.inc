SECTION_SIZE			equ 1000h

IOCTL_SHARE_MY_SECTION 	equ CTL_CODE(FILE_DEVICE_UNKNOWN, 800h, 0, 0)

.const
CCOUNTED_UNICODE_STRING	"\\BaseNamedObjects\\UserKernelSharedSection", g_usSectionName, 4
