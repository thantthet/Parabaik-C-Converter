Parabaik-C-Converter
====================

C version of Parabaik Myanmar Text converter (Zawgyi > Unicode) [ https://github.com/ngwestar/parabaik ] which ment to use with C/C++ or Objective-C.

This project uses the rules from original javascript converter. The code needs ICU library. Project also included NSString category to use with Objective-C on iOS and Mac OS X.

Installing
----------

### C/C++ Project
1. Link your project with libicucore
2. Write `#include "parabaik.h"`

#### Usage
```c
print_f("This is Unicode string: %s", zawgyi_to_unicode("ZawGyi sequence string"));
```

### Objective-C Project
1. Link your project with libicucore
2. Write `#import "NSString+ParabaikConverter.h"`

#### Usage
```objectivec
NSLog("This is Unicode string: %@", ["ZawGyi sequence string" unicodeString]);
```

https://github.com/ngwestar/parabaik is stated as GPL but this project is LGPL with the permission of original author – Ko Ngwe TUN.
