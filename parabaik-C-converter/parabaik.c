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

#include "parabaik.h"
#include <stdlib.h>
#include <string.h>

#define U_DISABLE_RENAMING 1

#include <unicode/ustring.h>
#include <unicode/uregex.h>
#if TARGET_OS_IPHONE
    typedef struct UConverter UConverter;

    U_STABLE UConverter* U_EXPORT2
    ucnv_open(const char *converterName, UErrorCode *err);

    U_STABLE void  U_EXPORT2
    ucnv_close(UConverter * converter);

    U_STABLE int32_t U_EXPORT2
    ucnv_toUChars(UConverter *cnv,
                  UChar *dest, int32_t destCapacity,
                  const char *src, int32_t srcLength,
                  UErrorCode *pErrorCode);

    U_STABLE int32_t U_EXPORT2
    ucnv_fromUChars(UConverter *cnv,
                    char *dest, int32_t destCapacity,
                    const UChar *src, int32_t srcLength,
                    UErrorCode *pErrorCode);
#else
    #include <unicode/ucnv.h> // this header seems to be missing on iOS sdk
#endif

UChar *toUChar(const char *string);
char *toChar(const UChar *ustring);
int strlen_utf8(const char *s);

struct RegexReplacePair {
    URegularExpression *regex;
    UChar *replace;
};

typedef struct RegexReplacePair RegexReplacePair;

static inline RegexReplacePair
RegexReplacePairMake(const char *pattern, const char *replace)
{
    RegexReplacePair pair;
    
    UParseError pe;
    UErrorCode status = U_ZERO_ERROR;
    URegularExpression *regex = uregex_openC(pattern, 0, &pe, &status);
    if (status != U_ZERO_ERROR) {
        printf("ERROR(%i) on compiling regex: pattern(%s)\n", status, pattern);
    }
    pair.regex = regex;
    
    pair.replace = toUChar(replace);
    
    return pair;
}

static inline char *
str_concat(char *s1, char *s2)
{
    char *cat = malloc(sizeof(char) * (strlen(s1) + strlen(s2)));
    strcpy(cat, s1);
    strcat(cat, s2);
    return cat;
}

char *zawgyi_to_unicode(const char *input)
{
    char * tallAA = "\u102B";
    __unused char * AA = "\u102C";
    char * vi = "\u102D";
    // lone gyi tin
    __unused char * ii = "\u102E";
    char * u = "\u102F";
    char * uu = "\u1030";
    __unused char * ve = "\u1031";
    __unused char * ai = "\u1032";
    char * ans = "\u1036";
    char * db = "\u1037";
    __unused char * visarga = "\u1038";
    
    char * asat = "\u103A";
    
    char * ya = "\u103B";
    char * ra = "\u103C";
    char * wa = "\u103D";
    char * ha = "\u103E";
    char * zero = "\u1040";
    
    RegexReplacePair pattern[] = {
        RegexReplacePairMake("\u106A","\u1009"),
        RegexReplacePairMake("\u1025(?=[\u1039\u102C])","\u1009"), //new
        RegexReplacePairMake("\u1025\u102E","\u1026"), //new
        RegexReplacePairMake("\u106B","\u100A"),
        RegexReplacePairMake("\u1090","\u101B"),
        RegexReplacePairMake("\u1040",zero),
        
        RegexReplacePairMake("\u108F","\u1014"),
        RegexReplacePairMake("\u1012","\u1012"),
        RegexReplacePairMake("\u1013","\u1013"),
        /////////////
        
        
        RegexReplacePairMake("[\u103D\u1087]",ha),
        // ha
        RegexReplacePairMake("\u103C",wa),
        // wa
        RegexReplacePairMake("[\u103B\u107E\u107F\u1080\u1081\u1082\u1083\u1084]",ra),
        // ya yint(ra)
        RegexReplacePairMake("[\u103A\u107D]",ya),
        // ya
        
        RegexReplacePairMake("\u103E\u103B",str_concat(ya, ha)),
        // reorder
        
        RegexReplacePairMake("\u108A",str_concat(wa, ha)),
        RegexReplacePairMake("\u103E\u103D",str_concat(wa, ha)),
        // wa ha
        
        ////////////////////// Reordering
        
        RegexReplacePairMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u1064","\u1064$1$2$3"),
        // reordering kinzi
        RegexReplacePairMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108B","\u1064$1$2$3\u102D"),
        // reordering kinzi lgt
        RegexReplacePairMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108C","\u1064$1$2$3\u102E"),
        // reordering kinzi lgtsk
        RegexReplacePairMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108D","\u1064$1$2$3\u1036"),
        // reordering kinzi ttt
        
        ////////////////////////////////////////
        
        RegexReplacePairMake("\u105A",str_concat(tallAA, asat)),
        RegexReplacePairMake("\u108E",str_concat(vi, ans)),
        // lgt ttt
        RegexReplacePairMake("\u1033",u),
        RegexReplacePairMake("\u1034",uu),
        RegexReplacePairMake("\u1088",str_concat(ha, u)),
        // ha  u
        RegexReplacePairMake("\u1089",str_concat(ha, uu)),
        // ha  uu
        
        ///////////////////////////////////////
        
        RegexReplacePairMake("\u1039","\u103A"),
        RegexReplacePairMake("[\u1094\u1095]",db),
        // aukmyint
        
        ///////////////////////////////////////Pasint order human error
        RegexReplacePairMake("([\u1000-\u1021])([\u102C\u102D\u102E\u1032\u1036]){1,2}([\u1060\u1061\u1062\u1063\u1065\u1066\u1067\u1068\u1069\u1070\u1071\u1072\u1073\u1074\u1075\u1076\u1077\u1078\u1079\u107A\u107B\u107C\u1085])","$1$3$2"),  //new
        
        
        
        /////////////
        
        RegexReplacePairMake("\u1064","\u1004\u103A\u1039"),
        
        RegexReplacePairMake("\u104E","\u104E\u1004\u103A\u1038"),
        
        RegexReplacePairMake("\u1086","\u103F"),
        
        RegexReplacePairMake("\u1060","\u1039\u1000"),
        RegexReplacePairMake("\u1061","\u1039\u1001"),
        RegexReplacePairMake("\u1062","\u1039\u1002"),
        RegexReplacePairMake("\u1063","\u1039\u1003"),
        RegexReplacePairMake("\u1065","\u1039\u1005"),
        RegexReplacePairMake("[\u1066\u1067]","\u1039\u1006"),
        RegexReplacePairMake("\u1068","\u1039\u1007"),
        RegexReplacePairMake("\u1069","\u1039\u1008"),
        RegexReplacePairMake("\u106C","\u1039\u100B"),
        RegexReplacePairMake("\u1070","\u1039\u100F"),
        RegexReplacePairMake("[\u1071\u1072]","\u1039\u1010"),
        RegexReplacePairMake("[\u1073\u1074]","\u1039\u1011"),
        RegexReplacePairMake("\u1075","\u1039\u1012"),
        RegexReplacePairMake("\u1076","\u1039\u1013"),
        RegexReplacePairMake("\u1077","\u1039\u1014"),
        RegexReplacePairMake("\u1078","\u1039\u1015"),
        RegexReplacePairMake("\u1079","\u1039\u1016"),
        RegexReplacePairMake("\u107A","\u1039\u1017"),
        RegexReplacePairMake("\u107B","\u1039\u1018"),
        RegexReplacePairMake("\u107C","\u1039\u1019"),
        RegexReplacePairMake("\u1085","\u1039\u101C"),
        RegexReplacePairMake("\u106D","\u1039\u100C"),
        
        RegexReplacePairMake("\u1091","\u100F\u1039\u100D"),
        RegexReplacePairMake("\u1092","\u100B\u1039\u100C"),
        RegexReplacePairMake("\u1097","\u100B\u1039\u100B"),
        RegexReplacePairMake("\u106F","\u100D\u1039\u100E"),
        RegexReplacePairMake("\u106E","\u100D\u1039\u100D"),
        
        /////////////////////////////////////////////////////////
        
        RegexReplacePairMake("(\u103C)([\u1000-\u1021])(\u1039[\u1000-\u1021])?","$2$3$1"),
        // reordering ra
        
        //RegexReplacePairMake("(\u103E)?(\u103D)?([\u103B\u103C])","$3$2$1"),
        // reordering ra
        
        RegexReplacePairMake("(\u103E)(\u103D)([\u103B\u103C])","$3$2$1"),
        RegexReplacePairMake("(\u103E)([\u103B\u103C])","$2$1"),
        RegexReplacePairMake("(\u103D)([\u103B\u103C])","$2$1"),
        
        RegexReplacePairMake("([\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F])(\u1040)(?!\\s)", "$1\u101D"),
        // zero and wa
        
        RegexReplacePairMake("(\u101D)(\u1040)(?!\\s)", "$1\u101D"),
        // zero and wa
        
        RegexReplacePairMake("([\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F\\s])(\u1047)", "$1\u101B"),
        // seven and ra
        
        RegexReplacePairMake("(\u1047)(?=[\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F\\s])","\u101B"),
        // seven and ra
        
        RegexReplacePairMake("(\u1031)?([\u1000-\u1021])(\u1039[\u1000-\u1021])?([\u102D\u102E\u1032])?([\u1036\u1037\u1038]{0,2})([\u103B-\u103E]{0,3})([\u102F\u1030])?([\u1036\u1037\u1038]{0,2})([\u102D\u102E\u1032])?","$2$3$6$1$4$9$7$5$8"),
        // reordering storage order
        RegexReplacePairMake(str_concat(ans,u), str_concat(u,ans)),
        
        RegexReplacePairMake("(\u103A)(\u1037)","$2$1")
        // For Latest Myanmar3
    };
    
    UChar *inputUStr = toUChar(input);
    
    uint32_t outputCapacity = u_strlen(inputUStr) * 2;
    UChar *output = malloc(outputCapacity * U_SIZEOF_UCHAR);
    u_strcpy(output, inputUStr);
    
    uint32_t count = sizeof(pattern) / sizeof(RegexReplacePair);
    
    for (int i = 0; i < count; i++) {
        URegularExpression *regex = pattern[i].regex;
        
        UErrorCode errorCode = U_ZERO_ERROR;
        int32_t length = u_strlen(output);
        UChar *temp = malloc(length * U_SIZEOF_UCHAR);
        u_strncpy(temp, output, length);
        uregex_setText(regex, temp, length, &errorCode);
        errorCode = U_ZERO_ERROR;
        __unused int32_t after_replaced = uregex_replaceAll(regex, pattern[i].replace, -1, output, outputCapacity, &errorCode);
        free(temp);
        if (errorCode == U_STRING_NOT_TERMINATED_WARNING) { // not enough space ?
            outputCapacity *= 2;
            temp = malloc(outputCapacity * U_SIZEOF_UCHAR);
            u_strncpy(temp, output, outputCapacity / 2);
            free(output);
            output = temp;
            
            char *tempchar = toChar(output);
            free(tempchar);
        } else if (errorCode != U_ZERO_ERROR) {
            printf("ERROR(%i) while replacing text.\n", errorCode);
        }
    }
    
    char *outCStr = toChar(output);
    free(output);
    return outCStr;
}

UChar *toUChar(const char *string)
{
    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter *conv = ucnv_open("utf-8", &errorCode);
    
    uint32_t outputCapacity = 100;
    UChar *output = (UChar*) malloc(outputCapacity * U_SIZEOF_UCHAR);
    
    uint32_t destLen = ucnv_toUChars(conv, output, outputCapacity, string, -1, &errorCode);
    if (errorCode == U_BUFFER_OVERFLOW_ERROR) {
        errorCode = U_ZERO_ERROR;
        free(output);
        output = (UChar*) malloc(destLen * U_SIZEOF_UCHAR);
        outputCapacity = destLen + 1;
        destLen = ucnv_toUChars(conv, output, outputCapacity, string, -1, &errorCode);
    }
    
    ucnv_close(conv);
    
    return output;
}

char *toChar(const UChar *ustring)
{
    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter *conv = ucnv_open("utf-8", &errorCode);
    
    uint32_t destCapcaity = 100;
    char * output = (char*) malloc(destCapcaity * sizeof(char));

    uint32_t destLen = ucnv_fromUChars(conv, output, destCapcaity, ustring, -1, &errorCode);

    if (errorCode == U_BUFFER_OVERFLOW_ERROR) {
        errorCode = U_ZERO_ERROR;
        free(output);
        output = (char*) malloc(destLen * sizeof(char));
        destCapcaity = destLen + 1;
        destLen = ucnv_fromUChars(conv, output, destCapcaity, ustring, -1, &errorCode);
    }
    
    ucnv_close(conv);
    
    return output;
}

int strlen_utf8(const char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80) j++;
        i++;
    }
    return j;
}