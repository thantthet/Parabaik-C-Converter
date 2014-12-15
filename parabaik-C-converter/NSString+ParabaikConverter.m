//
//  NSString+ParabaikConverter.m
//  parabaik-C-converter
//
//  Created by Thant Thet Khin Zaw on 12/16/14.
//  Copyright (c) 2014 myOpenware. All rights reserved.
//

#import "NSString+ParabaikConverter.h"
#import "parabaik.h"

@implementation NSString (ParabaikConverter)

- (NSString *)unicodeString
{
    const char *zawgyiUTF8 = [self UTF8String];
    char * unicodeUTF8 = zawgyi_to_unicode(zawgyiUTF8);
    NSString *unicodeString = [NSString stringWithCString:unicodeUTF8 encoding:NSUTF8StringEncoding];
    free(unicodeUTF8);
    return unicodeString;
}

@end
