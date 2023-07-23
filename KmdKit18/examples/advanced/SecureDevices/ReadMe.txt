
This is an example how to apply particular security settings
to named device object by calling IoCreateDeviceSecure instead of
IoCreateDevice.

Since IoCreateDeviceSecure routine is not a part of the operating system,
we link wdmsec.lib, which contains all needed routines.

The wdmsec.lib library you will find here is not one shipped with the DDK.
I had to rebuild it to reduce its size, removing not needed members.

Use my WinObjEx utility to explore the security information for created
device objects.

Tested under: Windows 2000, XP and Server 2003.

______________________
Four-F, four-f@mail.ru