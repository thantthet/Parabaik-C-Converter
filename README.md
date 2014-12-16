Parabaik-C-Converter
====================

C implementation of Parabaik Myanmar Text converter (Zawgyi > Unicode) [ https://github.com/ngwestar/parabaik ] which ment to use with C/C++ or Objective-C.

This project uses the conversion patterns from original javascript implementation. ICU library is needed. Project also included NSString Category to use with Objective-C on iOS and Mac OS X.

Installing
----------

### C/C++ Project
1. Link your project with `libicucore`
2. Write `#include "parabaik.h"`

#### Usage
```c
char *unicode = zawgyi_to_unicode("ZawGyi sequence string");
print_f("This is Unicode string: %s", unicode);
free(unicode); // be sure to free when you are done
```

### Objective-C Project
1. Link your project with `libicucore`
2. Write `#import "NSString+ParabaikConverter.h"`

#### Usage
```objectivec
NSLog("This is Unicode string: %@", ["ZawGyi sequence string" unicodeString]);
```

https://github.com/ngwestar/parabaik is stated as GPL but this project is LGPL with the permission of original author – Ko Ngwe TUN.
