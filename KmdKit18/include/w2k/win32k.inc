
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:           This file was created on 16 Jan 2005 with Protogen v1.0 by Four-F.                   ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; __cdecls was fixed by hand !!!

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                            STDCALL                                             ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BRUSHOBJ_hGetColorTransform    proto stdcall :DWORD
BRUSHOBJ_pvAllocRbrush         proto stdcall :DWORD, :DWORD
BRUSHOBJ_pvGetRbrush           proto stdcall :DWORD
BRUSHOBJ_ulGetBrushColor       proto stdcall :DWORD
CLIPOBJ_bEnum                  proto stdcall :DWORD, :DWORD, :DWORD
CLIPOBJ_cEnumStart             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CLIPOBJ_ppoGetPath             proto stdcall :DWORD
EngAcquireSemaphore            proto stdcall :DWORD
EngAllocMem                    proto stdcall :DWORD, :DWORD, :DWORD
EngAllocPrivateUserMem         proto stdcall :DWORD, :DWORD, :DWORD
EngAllocUserMem                proto stdcall :DWORD, :DWORD
EngAlphaBlend                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngAssociateSurface            proto stdcall :DWORD, :DWORD, :DWORD
EngBitBlt                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCheckAbort                  proto stdcall :DWORD
EngComputeGlyphSet             proto stdcall :DWORD, :DWORD, :DWORD
EngControlSprites              proto stdcall :DWORD, :DWORD
EngCopyBits                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCreateBitmap                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCreateClip                  proto stdcall
EngCreateDeviceBitmap          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngCreateDeviceSurface         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngCreateDriverObj             proto stdcall :DWORD, :DWORD, :DWORD
EngCreateEvent                 proto stdcall :DWORD
EngCreatePalette               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCreatePath                  proto stdcall
EngCreateSemaphore             proto stdcall
EngCreateWnd                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngDebugBreak                  proto stdcall
EngDebugPrint                  proto stdcall :DWORD, :DWORD, :DWORD
EngDeleteClip                  proto stdcall :DWORD
EngDeleteDriverObj             proto stdcall :DWORD, :DWORD, :DWORD
EngDeleteEvent                 proto stdcall :DWORD
EngDeleteFile                  proto stdcall :DWORD
EngDeletePalette               proto stdcall :DWORD
EngDeletePath                  proto stdcall :DWORD
EngDeleteSafeSemaphore         proto stdcall :DWORD
EngDeleteSemaphore             proto stdcall :DWORD
EngDeleteSurface               proto stdcall :DWORD
EngDeleteWnd                   proto stdcall :DWORD
EngDeviceIoControl             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngDitherColor                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngEnumForms                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngEraseSurface                proto stdcall :DWORD, :DWORD, :DWORD
EngFileIoControl               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngFileWrite                   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngFillPath                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngFindImageProcAddress        proto stdcall :DWORD, :DWORD
EngFindResource                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngFntCacheAlloc               proto stdcall :DWORD, :DWORD
EngFntCacheFault               proto stdcall :DWORD, :DWORD
EngFntCacheLookUp              proto stdcall :DWORD, :DWORD
EngFreeMem                     proto stdcall :DWORD
EngFreeModule                  proto stdcall :DWORD
EngFreePrivateUserMem          proto stdcall :DWORD, :DWORD
EngFreeUserMem                 proto stdcall :DWORD
EngGetCurrentCodePage          proto stdcall :DWORD, :DWORD
EngGetCurrentProcessId         proto stdcall
EngGetCurrentThreadId          proto stdcall
EngGetDriverName               proto stdcall :DWORD
EngGetFileChangeTime           proto stdcall :DWORD, :DWORD
EngGetFilePath                 proto stdcall :DWORD, :DWORD
EngGetForm                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetLastError                proto stdcall
EngGetPrinter                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetPrinterData              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetPrinterDataFileName      proto stdcall :DWORD
EngGetPrinterDriver            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetProcessHandle            proto stdcall
EngGetTickCount                proto stdcall
EngGetType1FontList            proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGradientFill                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngInitializeSafeSemaphore     proto stdcall :DWORD
EngLineTo                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngLoadImage                   proto stdcall :DWORD
EngLoadModule                  proto stdcall :DWORD
EngLoadModuleForWrite          proto stdcall :DWORD, :DWORD
EngLockDirectDrawSurface       proto stdcall :DWORD
EngLockDriverObj               proto stdcall :DWORD
EngLockSurface                 proto stdcall :DWORD
EngLpkInstalled                proto stdcall
EngMapEvent                    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMapFile                     proto stdcall :DWORD, :DWORD, :DWORD
EngMapFontFile                 proto stdcall :DWORD, :DWORD, :DWORD
EngMapFontFileFD               proto stdcall :DWORD, :DWORD, :DWORD
EngMapModule                   proto stdcall :DWORD, :DWORD
EngMarkBandingSurface          proto stdcall :DWORD
EngModifySurface               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMovePointer                 proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngMulDiv                      proto stdcall :DWORD, :DWORD, :DWORD
EngMultiByteToUnicodeN         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMultiByteToWideChar         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngPaint                       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngPlgBlt                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngProbeForRead                proto stdcall :DWORD, :DWORD, :DWORD
EngProbeForReadAndWrite        proto stdcall :DWORD, :DWORD, :DWORD
EngQueryDeviceAttribute        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngQueryLocalTime              proto stdcall :DWORD
EngQueryPalette                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngQueryPerformanceCounter     proto stdcall :DWORD
EngQueryPerformanceFrequency   proto stdcall :DWORD
EngQuerySystemAttribute        proto stdcall :DWORD, :DWORD
EngReleaseSemaphore            proto stdcall :DWORD
EngRestoreFloatingPointState   proto stdcall :DWORD
EngSaveFloatingPointState      proto stdcall :DWORD, :DWORD
EngSecureMem                   proto stdcall :DWORD, :DWORD
EngSetEvent                    proto stdcall :DWORD
EngSetLastError                proto stdcall :DWORD
EngSetPointerShape             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSetPointerTag               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSetPrinterData              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSort                        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
EngStretchBlt                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStretchBltROP               proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStrokeAndFillPath           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStrokePath                  proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngTextOut                     proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngTransparentBlt              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngUnicodeToMultiByteN         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngUnloadImage                 proto stdcall :DWORD
EngUnlockDirectDrawSurface     proto stdcall :DWORD
EngUnlockDriverObj             proto stdcall :DWORD
EngUnlockSurface               proto stdcall :DWORD
EngUnmapEvent                  proto stdcall :DWORD
EngUnmapFile                   proto stdcall :DWORD
EngUnmapFontFile               proto stdcall :DWORD
EngUnmapFontFileFD             proto stdcall :DWORD
EngUnsecureMem                 proto stdcall :DWORD
EngWaitForSingleObject         proto stdcall :DWORD, :DWORD
EngWideCharToMultiByte         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngWritePrinter                proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
FLOATOBJ_Add                   proto stdcall :DWORD, :DWORD
FLOATOBJ_AddFloat              proto stdcall :DWORD, :DWORD
FLOATOBJ_AddLong               proto stdcall :DWORD, :DWORD
FLOATOBJ_Div                   proto stdcall :DWORD, :DWORD
FLOATOBJ_DivFloat              proto stdcall :DWORD, :DWORD
FLOATOBJ_DivLong               proto stdcall :DWORD, :DWORD
FLOATOBJ_Equal                 proto stdcall :DWORD, :DWORD
FLOATOBJ_EqualLong             proto stdcall :DWORD, :DWORD
FLOATOBJ_GetFloat              proto stdcall :DWORD
FLOATOBJ_GetLong               proto stdcall :DWORD
FLOATOBJ_GreaterThan           proto stdcall :DWORD, :DWORD
FLOATOBJ_GreaterThanLong       proto stdcall :DWORD, :DWORD
FLOATOBJ_LessThan              proto stdcall :DWORD, :DWORD
FLOATOBJ_LessThanLong          proto stdcall :DWORD, :DWORD
FLOATOBJ_Mul                   proto stdcall :DWORD, :DWORD
FLOATOBJ_MulFloat              proto stdcall :DWORD, :DWORD
FLOATOBJ_MulLong               proto stdcall :DWORD, :DWORD
FLOATOBJ_Neg                   proto stdcall :DWORD
FLOATOBJ_SetFloat              proto stdcall :DWORD, :DWORD
FLOATOBJ_SetLong               proto stdcall :DWORD, :DWORD
FLOATOBJ_Sub                   proto stdcall :DWORD, :DWORD
FLOATOBJ_SubFloat              proto stdcall :DWORD, :DWORD
FLOATOBJ_SubLong               proto stdcall :DWORD, :DWORD
FONTOBJ_cGetAllGlyphHandles    proto stdcall :DWORD, :DWORD
FONTOBJ_cGetGlyphs             proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FONTOBJ_pQueryGlyphAttrs       proto stdcall :DWORD, :DWORD
FONTOBJ_pfdg                   proto stdcall :DWORD
FONTOBJ_pifi                   proto stdcall :DWORD
FONTOBJ_pjOpenTypeTablePointer proto stdcall :DWORD, :DWORD, :DWORD
FONTOBJ_pvTrueTypeFontFile     proto stdcall :DWORD, :DWORD
FONTOBJ_pwszFontFilePaths      proto stdcall :DWORD, :DWORD
FONTOBJ_pxoGetXform            proto stdcall :DWORD
FONTOBJ_vGetInfo               proto stdcall :DWORD, :DWORD, :DWORD
HT_ComputeRGBGammaTable        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HT_Get8BPPFormatPalette        proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
HT_Get8BPPMaskPalette          proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HeapVidMemAllocAligned         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
PALOBJ_cGetColors              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
PATHOBJ_bCloseFigure           proto stdcall :DWORD
PATHOBJ_bEnum                  proto stdcall :DWORD, :DWORD
PATHOBJ_bEnumClipLines         proto stdcall :DWORD, :DWORD, :DWORD
PATHOBJ_bMoveTo                proto stdcall :DWORD, :DWORD, :DWORD
PATHOBJ_bPolyBezierTo          proto stdcall :DWORD, :DWORD, :DWORD
PATHOBJ_bPolyLineTo            proto stdcall :DWORD, :DWORD, :DWORD
PATHOBJ_vEnumStart             proto stdcall :DWORD
PATHOBJ_vEnumStartClipLines    proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
PATHOBJ_vGetBounds             proto stdcall :DWORD, :DWORD
RtlAnsiCharToUnicodeChar       proto stdcall :DWORD
RtlMultiByteToUnicodeN         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlRaiseException              proto stdcall :DWORD
RtlUnicodeToMultiByteN         proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteSize      proto stdcall :DWORD, :DWORD, :DWORD
RtlUnwind                      proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeChar           proto stdcall :DWORD
RtlUpcaseUnicodeToMultiByteN   proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
STROBJ_bEnum                   proto stdcall :DWORD, :DWORD, :DWORD
STROBJ_bEnumPositionsOnly      proto stdcall :DWORD, :DWORD, :DWORD
STROBJ_bGetAdvanceWidths       proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
STROBJ_dwGetCodePage           proto stdcall :DWORD
STROBJ_fxBreakExtra            proto stdcall :DWORD
STROBJ_fxCharacterExtra        proto stdcall :DWORD
STROBJ_vEnumStart              proto stdcall :DWORD
VidMemFree                     proto stdcall :DWORD, :DWORD
WNDOBJ_bEnum                   proto stdcall :DWORD, :DWORD, :DWORD
WNDOBJ_cEnumStart              proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
WNDOBJ_vSetConsumer            proto stdcall :DWORD, :DWORD
XFORMOBJ_bApplyXform           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
XFORMOBJ_iGetFloatObjXform     proto stdcall :DWORD, :DWORD
XFORMOBJ_iGetXform             proto stdcall :DWORD, :DWORD
XLATEOBJ_cGetPalette           proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
XLATEOBJ_hGetColorTransform    proto stdcall :DWORD
XLATEOBJ_iXlate                proto stdcall :DWORD, :DWORD
XLATEOBJ_piVector              proto stdcall :DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                             CDECL                                              ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

comment ^
DDVPTYPE_BROOKTREE                  proto c :VARARG
DDVPTYPE_CCIR656                    proto c :VARARG
DDVPTYPE_E_HREFH_VREFH              proto c :VARARG
DDVPTYPE_E_HREFH_VREFL              proto c :VARARG
DDVPTYPE_E_HREFL_VREFH              proto c :VARARG
DDVPTYPE_E_HREFL_VREFL              proto c :VARARG
DDVPTYPE_PHILIPS                    proto c :VARARG
GUID_AGP_INTERFACE                  proto c :VARARG
GUID_ColorControlCallbacks          proto c :VARARG
GUID_D3DCallbacks                   proto c :VARARG
GUID_D3DCallbacks2                  proto c :VARARG
GUID_D3DCallbacks3                  proto c :VARARG
GUID_D3DCaps                        proto c :VARARG
GUID_D3DExtendedCaps                proto c :VARARG
GUID_D3DParseUnknownCommandCallback proto c :VARARG
GUID_DDMoreCaps                     proto c :VARARG
GUID_DDMoreSurfaceCaps              proto c :VARARG
GUID_DDStereoMode                   proto c :VARARG
GUID_DxApi                          proto c :VARARG
GUID_GetHeapAlignment               proto c :VARARG
GUID_KernelCallbacks                proto c :VARARG
GUID_KernelCaps                     proto c :VARARG
GUID_Miscellaneous2Callbacks        proto c :VARARG
GUID_MiscellaneousCallbacks         proto c :VARARG
GUID_MotionCompCallbacks            proto c :VARARG
GUID_NTCallbacks                    proto c :VARARG
GUID_NTPrivateDriverCaps            proto c :VARARG
GUID_NonLocalVidMemCaps             proto c :VARARG
GUID_UpdateNonLocalHeap             proto c :VARARG
GUID_VideoPortCallbacks             proto c :VARARG
GUID_VideoPortCaps                  proto c :VARARG
GUID_ZPixelFormats                  proto c :VARARG
IID_IDDVideoPortContainer           proto c :VARARG
IID_IDirectDrawVideoPort            proto c :VARARG
^
;_abnormal_termination               proto c :VARARG
;_except_handler2                    proto c :VARARG
;_global_unwind2                     proto c :VARARG
_itoa                               proto c :DWORD, :DWORD, :DWORD
_itow                               proto c :DWORD, :DWORD, :DWORD
;_local_unwind2                      proto c :VARARG

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;:                                       STDCALL EQUATIONS                                        ::
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFDEF UNICODE
ELSE
ENDIF
