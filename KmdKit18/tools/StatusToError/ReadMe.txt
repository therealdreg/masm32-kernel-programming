
Nt Status To Win32 Error

Let you convert kernel-mode status code to the corresponding Win32 error code.
The Win32 application call GetLastError get it.

Below is the correspondence for three status codes:

For ex, kernel-side STATUS_BUFFER_TOO_SMALL code will be user-side ERROR_INSUFFICIENT_BUFFER code.
And kernel-side STATUS_INVALID_DEVICE_REQUEST code will be user-side ERROR_INVALID_FUNCTION code.

You will not get mnemonics from this tool but only hexadecimal numbers.

______________________
Four-F, four-f@mail.ru