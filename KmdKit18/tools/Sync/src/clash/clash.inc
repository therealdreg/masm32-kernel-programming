;;
;; CLASH -- Command Line Argument and Switch Handling
;;
;; include file, version 2001.06.22
;;
;; Copyright (c) 2001 by Joergen Ibsen / Jibz
;; All Rights Reserved
;;

; -------------------------------------------
;  Usage information at the end of this file
; -------------------------------------------

	; function prototypes
	CL_ScanArgs  PROTO :DWORD
	CL_ScanArgsX PROTO :DWORD

	; data declarations
	externdef CL_argv:DWORD
	externdef CL_argc:DWORD
	externdef CL_switch:BYTE

; ===================================================================
;
;    CL_ScanArgs takes one argument, which is a pointer to the
;    command line (or any other zero-terminated string with a
;    similar layout). It parses this string into a list of
;    pointers to the different arguments, CL_argv, which works
;    like the C/C++ argv[]. So you can do e.g.
;
;       invoke someFunction, CL_argv[4]
;
;    to pass a pointer to the second command line argument (with
;    argument number 1) to someFunction.
;
;    It returns the number of arguments found, and also stores
;    it in CL_argc, which works like the corresponding C/C++
;    argc.
;
;    The first argment (number 0) of a standard command line
;    is the full path and filename of the program itself.
;
;    It also makes a byte array with all switches (arguments
;    starting with '-' or '/') found. E.g. you can check if
;    there was a '-o' switch specified by
;
;       .if CL_switch['o']
;          ...
;
;    The value stored in CL_switch['o'] is the last argument
;    number where the switch was specified, so if the command
;    line contains
;
;       -o filename.ext
;
;    it is possible to use
;
;       movzx eax,CL_switch['o']  ; get arg number of '-o'
;       lea eax,[eax*4+4]         ; calculate (eax+1)*4
;       mov eax,CL_argv[eax]      ; get pointer to next arg
;
;    and now eax points to the argument right after the '-o'
;    switch.
;
;    CL_switch[0] contains the argument number of the last
;    argument which was a switch (or 0), so it can be usefull
;    if all arguments must come before filenames in your code.
;
;    All characters in a switch argument are handled as
;    switches, which means that
;
;       -abc
;
;    will set switches 'a', 'b' and 'c' to the same argument
;    number.
;
;    CL_ScanArgsX is identical to CL_ScanArgs, except that it
;    only handles the first character in a switch argument as
;    a switch, so
;
;       -abc
;
;    will only set switch 'a' to the argument number.
;
;    Both handle quoted parts inside arguments as well as regular
;    quoted arguments, which means that
;
;       -o"argument with spaces"
;
;    will work fine.
;
;    Anything below or equal to a quote ('"' = 0x20) is handled as
;    an argument delimiter (unless inside quotes). So tabs and EOL
;    will also work as delimiters, which is nice if it is used to
;    scan an environment string or a file containing options.
;
;    Empty argument strings (i.e. "") are also handled.
;
;    Since the code will only be run once at program startup there
;    is no reason to optimize for speed. So I spent some time on
;    making the code small instead. The current code takes up only
;    119 bytes (118 for CL_ScanArgsX).
;
; ===================================================================
