//    Copyright (c) 2014 Parabaik C Converter Project
//
//    Parabaik C Converter is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Parabaik C Converter is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#import "NSString+ParabaikConverter.h"
#import "parabaik.h"

@implementation NSString (ParabaikConverter)

+ (void)load
{
    int status = zuconverter_open();
    if (status) {
        NSLog(@"ZU converter cannot be opened. Failed with error : %d", status);
    }
}

- (NSString *)unicodeString
{
    const char *zawgyiUTF8 = [self UTF8String];

    char * unicodeUTF8 = zawgyi_to_unicode(zawgyiUTF8);
    
    NSString *unicodeString = [NSString stringWithCString:unicodeUTF8 encoding:NSUTF8StringEncoding];
    free(unicodeUTF8);
    return unicodeString;
}

- (NSString *)zawgyiString
{
    const char *unicodeUTF8 = [self UTF8String];
    
    char * zawgyiUTF8 = unicode_to_zawgyi(unicodeUTF8);
    
    NSString *zawgyiString = [NSString stringWithCString:zawgyiUTF8 encoding:NSUTF8StringEncoding];
    free(zawgyiUTF8);
    return zawgyiString;
}

@end
