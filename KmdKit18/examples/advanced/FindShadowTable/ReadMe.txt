
This is an example how to find ServiceDescriptorTableShadow.

It just scans threads whith ID 80h-400h trying to find a thread which KTHREAD.ServiceTable not equal to KeServiceDescriptorTable. If such a thread is found its KTHREAD.ServiceTable holds ServiceDescriptorTableShadow address.

Watch driver's debug output with the DbgView (www.sysinternals.com) or use SoftICE.

Tested under: Windows 2000, XP and Server 2003.

______________________
Four-F, four-f@mail.ru