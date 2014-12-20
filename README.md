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
zuconverter_open(); // make sure you init the converter before doing any conversion, otherwise you will get the unconverted string when you convert

char *unicode;

unicode = zawgyi_to_unicode("ZawGyi sequence string"); // converts into Unicode encoded string

print_f("This is Unicode string: %s", unicode);

free(unicode); // be sure to free when you are done

zuconverter_open(); // close converter to free the resources
```

### Objective-C Project
1. Link your project with `libicucore`
2. Write `#import "NSString+ParabaikConverter.h"`

#### Usage
```objectivec
NSString *zgString = "ZawGyi sequence string";
NSString *unicodeString = [zgString unicodeString];
NSLog("This is Unicode string: %@", unicodeString);
```
