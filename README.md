Parabaik-C-Converter
====================

C version of Parabaik Myanmar Text converter (Zawgyi > Unicode) [ https://github.com/ngwestar/parabaik ] which ment to use with C/C++ or Objective-C.

This project uses the rules from original javascript converter. The code needs ICU library. Project also included NSString category to use with Objective-C on iOS and Mac OS X.

Non Objective-C program can just include parabaik.h and parabaik.c in your program.

call `char *zawgyi_to_unicode(const char *input)` passing UTF-8 encoded ZawGyi string as argument. The function will returns UTF-8 encoded Burmese unicode string.

https://github.com/ngwestar/parabaik is stated as GPL but this project is LGPL with the permission of original author – Ko Ngwe TUN.
