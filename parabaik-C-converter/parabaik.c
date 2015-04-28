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
#import <pthread.h>

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

struct RegexReplacePair {
    URegularExpression *regex;
    UChar *replace;
};

typedef struct RegexReplacePair RegexReplacePair;

UChar *toUChar(const char *string);
char *toChar(const UChar *ustring);
int strlen_utf8(const char *s);
RegexReplacePair** regex_pairs();

// converter
static UConverter *UConverterUTF8;
// 1 if opened
static int opened = 0;

static pthread_mutex_t mutex;

static inline RegexReplacePair *
RegexReplacePairPtrMake(const char *pattern, const char *replace)
{
    RegexReplacePair *pair;
    
    pair = malloc(sizeof(RegexReplacePair));
    
    pair->regex = NULL;
    pair->replace = NULL;
    
    UParseError pe;
    UErrorCode status = U_ZERO_ERROR;
    if (pattern) {
        URegularExpression *regex = uregex_openC(pattern, 0, &pe, &status);
        if (status != U_ZERO_ERROR) {
            printf("ERROR(%i) on compiling regex: pattern(%s)\n", status, pattern);
        }
        pair->regex = regex;
    }
    
    if (replace) {
        pair->replace = toUChar(replace);
    }
    
    return pair;
}

static inline void
FreeRegexReplacePair(RegexReplacePair *pair)
{
    uregex_close(pair->regex);
    free(pair->replace);
}

int zuconverter_open()
{
    if (opened == 1) { // already opened
        return 0;
    }
    
    pthread_mutex_init(&mutex, NULL);
    
    UErrorCode status = U_ZERO_ERROR;
    
    UConverterUTF8 = ucnv_open("utf-8", &status);
    if (status != U_ZERO_ERROR) {
        printf("ERROR(%i) when opening UConverter.\n", status);
        return 1;
    }
    
    regex_pairs();
    
    opened = 1;
    
    return 0;
}

int zuconverter_close()
{
    if (opened == 0) { // already closed
        return 0;
    }
    
    pthread_mutex_destroy(&mutex);
    
    RegexReplacePair **pairs = regex_pairs();
    for (int i = 0; pairs[i]->regex != NULL; i++) {
        FreeRegexReplacePair(pairs[i]);
        free(pairs[i]);
    }
    pairs = NULL;
    
    ucnv_close(UConverterUTF8);
    UConverterUTF8 = NULL;
    
    opened = 0;
    
    return 0;
}

static inline char *
str_concat(char *s1, char *s2)
{
    char *cat = malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
    strcpy(cat, s1);
    strcat(cat, s2);
    return cat;
}

RegexReplacePair** regex_pairs()
{
    static RegexReplacePair **pairs;
    
    if (pairs) {
        return pairs;
    }
    
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
    
    size_t capacity = 100;
    pairs = malloc(sizeof(RegexReplacePair *) * capacity);
    
    int i = 0;
    pairs[i++] = RegexReplacePairPtrMake("\u106A","\u1009");
    pairs[i++] = RegexReplacePairPtrMake("\u1025(?=[\u1039\u102C])","\u1009"); //new
    pairs[i++] = RegexReplacePairPtrMake("\u1025\u102E","\u1026"); //new
    pairs[i++] = RegexReplacePairPtrMake("\u106B","\u100A");
    pairs[i++] = RegexReplacePairPtrMake("\u1090","\u101B");
    pairs[i++] = RegexReplacePairPtrMake("\u1040",zero);
    
    pairs[i++] = RegexReplacePairPtrMake("\u108F","\u1014");
    pairs[i++] = RegexReplacePairPtrMake("\u1012","\u1012");
    pairs[i++] = RegexReplacePairPtrMake("\u1013","\u1013");
    /////////////
    
    
    pairs[i++] = RegexReplacePairPtrMake("[\u103D\u1087]",ha);
    // ha
    pairs[i++] = RegexReplacePairPtrMake("\u103C",wa);
    // wa
    pairs[i++] = RegexReplacePairPtrMake("[\u103B\u107E\u107F\u1080\u1081\u1082\u1083\u1084]",ra);
    // ya yint(ra)
    pairs[i++] = RegexReplacePairPtrMake("[\u103A\u107D]",ya);
    // ya
    
    pairs[i++] = RegexReplacePairPtrMake("\u103E\u103B",str_concat(ya, ha));
    // reorder
    
    pairs[i++] = RegexReplacePairPtrMake("\u108A",str_concat(wa, ha));
    pairs[i++] = RegexReplacePairPtrMake("\u103E\u103D",str_concat(wa, ha));
    // wa ha
    
    ////////////////////// Reordering
    
    pairs[i++] = RegexReplacePairPtrMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u1064","\u1064$1$2$3");
    // reordering kinzi
    pairs[i++] = RegexReplacePairPtrMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108B","\u1064$1$2$3\u102D");
    // reordering kinzi lgt
    pairs[i++] = RegexReplacePairPtrMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108C","\u1064$1$2$3\u102E");
    // reordering kinzi lgtsk
    pairs[i++] = RegexReplacePairPtrMake("(\u1031)?(\u103C)?([\u1000-\u1021])\u108D","\u1064$1$2$3\u1036");
    // reordering kinzi ttt
    
    ////////////////////////////////////////
    
    pairs[i++] = RegexReplacePairPtrMake("\u105A",str_concat(tallAA, asat));
    pairs[i++] = RegexReplacePairPtrMake("\u108E",str_concat(vi, ans));
    // lgt ttt
    pairs[i++] = RegexReplacePairPtrMake("\u1033",u);
    pairs[i++] = RegexReplacePairPtrMake("\u1034",uu);
    pairs[i++] = RegexReplacePairPtrMake("\u1088",str_concat(ha, u));
    // ha  u
    pairs[i++] = RegexReplacePairPtrMake("\u1089",str_concat(ha, uu));
    // ha  uu
    
    ///////////////////////////////////////
    
    pairs[i++] = RegexReplacePairPtrMake("\u1039","\u103A");
    pairs[i++] = RegexReplacePairPtrMake("[\u1094\u1095]",db);
    // aukmyint
    
    ///////////////////////////////////////Pasint order human error
    pairs[i++] = RegexReplacePairPtrMake("([\u1000-\u1021])([\u102C\u102D\u102E\u1032\u1036]){1,2}([\u1060\u1061\u1062\u1063\u1065\u1066\u1067\u1068\u1069\u1070\u1071\u1072\u1073\u1074\u1075\u1076\u1077\u1078\u1079\u107A\u107B\u107C\u1085])","$1$3$2");  //new
    
    
    
    /////////////
    
    pairs[i++] = RegexReplacePairPtrMake("\u1064","\u1004\u103A\u1039");
    
    pairs[i++] = RegexReplacePairPtrMake("\u104E","\u104E\u1004\u103A\u1038");
    
    pairs[i++] = RegexReplacePairPtrMake("\u1086","\u103F");
    
    pairs[i++] = RegexReplacePairPtrMake("\u1060","\u1039\u1000");
    pairs[i++] = RegexReplacePairPtrMake("\u1061","\u1039\u1001");
    pairs[i++] = RegexReplacePairPtrMake("\u1062","\u1039\u1002");
    pairs[i++] = RegexReplacePairPtrMake("\u1063","\u1039\u1003");
    pairs[i++] = RegexReplacePairPtrMake("\u1065","\u1039\u1005");
    pairs[i++] = RegexReplacePairPtrMake("[\u1066\u1067]","\u1039\u1006");
    pairs[i++] = RegexReplacePairPtrMake("\u1068","\u1039\u1007");
    pairs[i++] = RegexReplacePairPtrMake("\u1069","\u1039\u1008");
    pairs[i++] = RegexReplacePairPtrMake("\u106C","\u1039\u100B");
    pairs[i++] = RegexReplacePairPtrMake("\u1070","\u1039\u100F");
    pairs[i++] = RegexReplacePairPtrMake("[\u1071\u1072]","\u1039\u1010");
    pairs[i++] = RegexReplacePairPtrMake("[\u1073\u1074]","\u1039\u1011");
    pairs[i++] = RegexReplacePairPtrMake("\u1075","\u1039\u1012");
    pairs[i++] = RegexReplacePairPtrMake("\u1076","\u1039\u1013");
    pairs[i++] = RegexReplacePairPtrMake("\u1077","\u1039\u1014");
    pairs[i++] = RegexReplacePairPtrMake("\u1078","\u1039\u1015");
    pairs[i++] = RegexReplacePairPtrMake("\u1079","\u1039\u1016");
    pairs[i++] = RegexReplacePairPtrMake("\u107A","\u1039\u1017");
    pairs[i++] = RegexReplacePairPtrMake("\u107B","\u1039\u1018");
    pairs[i++] = RegexReplacePairPtrMake("\u107C","\u1039\u1019");
    pairs[i++] = RegexReplacePairPtrMake("\u1085","\u1039\u101C");
    pairs[i++] = RegexReplacePairPtrMake("\u106D","\u1039\u100C");
    
    pairs[i++] = RegexReplacePairPtrMake("\u1091","\u100F\u1039\u100D");
    pairs[i++] = RegexReplacePairPtrMake("\u1092","\u100B\u1039\u100C");
    pairs[i++] = RegexReplacePairPtrMake("\u1097","\u100B\u1039\u100B");
    pairs[i++] = RegexReplacePairPtrMake("\u106F","\u100D\u1039\u100E");
    pairs[i++] = RegexReplacePairPtrMake("\u106E","\u100D\u1039\u100D");
    
    /////////////////////////////////////////////////////////
    
    pairs[i++] = RegexReplacePairPtrMake("(\u103C)([\u1000-\u1021])(\u1039[\u1000-\u1021])?","$2$3$1");
    // reordering ra
    
    //pairs[i++] = RegexReplacePairPtrMake("(\u103E)?(\u103D)?([\u103B\u103C])","$3$2$1");
    // reordering ra
    
    pairs[i++] = RegexReplacePairPtrMake("(\u103E)(\u103D)([\u103B\u103C])","$3$2$1");
    pairs[i++] = RegexReplacePairPtrMake("(\u103E)([\u103B\u103C])","$2$1");
    pairs[i++] = RegexReplacePairPtrMake("(\u103D)([\u103B\u103C])","$2$1");
    
    pairs[i++] = RegexReplacePairPtrMake("([\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F])(\u1040)(?!\\s)", "$1\u101D");
    // zero and wa
    
    pairs[i++] = RegexReplacePairPtrMake("(\u101D)(\u1040)(?!\\s)", "$1\u101D");
    // zero and wa
    
    pairs[i++] = RegexReplacePairPtrMake("([\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F\\s])(\u1047)", "$1\u101B");
    // seven and ra
    
    pairs[i++] = RegexReplacePairPtrMake("(\u1047)(?=[\u1000-\u101C\u101E-\u102A\u102C\u102E-\u103F\u104C-\u109F\\s])","\u101B");
    // seven and ra
    
    pairs[i++] = RegexReplacePairPtrMake("(\u1031)?([\u1000-\u1021])(\u1039[\u1000-\u1021])?([\u102D\u102E\u1032])?([\u1036\u1037\u1038]{0,2})([\u103B-\u103E]{0,3})([\u102F\u1030])?([\u1036\u1037\u1038]{0,2})([\u102D\u102E\u1032])?","$2$3$6$1$4$9$7$5$8");
    // reordering storage order
    pairs[i++] = RegexReplacePairPtrMake(str_concat(ans,u), str_concat(u,ans));
    
    pairs[i++] = RegexReplacePairPtrMake("(\u103A)(\u1037)","$2$1");
    // For Latest Myanmar3
    
    pairs[i++] = RegexReplacePairPtrMake(NULL, NULL);
    
    return pairs;
}

char *zawgyi_to_unicode(const char *input)
{
    if (opened != 1) {
        printf("You need to call zuconverter_open() once before using this function. (returning a copy of input string for now).\n");
        return strdup(input);
    }
    
    pthread_mutex_lock(&mutex);
    
    UChar *inputUStr = toUChar(input);
    
    uint32_t outputCapacity = u_strlen(inputUStr) * 3;
    UChar *output = malloc(outputCapacity * U_SIZEOF_UCHAR);
    u_strcpy(output, inputUStr);
    
    free(inputUStr);
    
    RegexReplacePair **pattern = regex_pairs();
    
    for (int i = 0; pattern[i]->regex != NULL; i++) {
        URegularExpression *regex = pattern[i]->regex;
        
        UErrorCode errorCode = U_ZERO_ERROR;
        int32_t length = u_strlen(output);
        UChar *temp = malloc(length * U_SIZEOF_UCHAR);
        u_strncpy(temp, output, length);
        uregex_setText(regex, temp, length, &errorCode);
        
        errorCode = U_ZERO_ERROR;
        __unused int32_t after_replaced = uregex_replaceAll(regex, pattern[i]->replace, -1, output, outputCapacity, &errorCode);
        
        free(temp);
        
        if (errorCode == U_STRING_NOT_TERMINATED_WARNING) { // not enough space ?
            uint32_t newOutputCapacity = outputCapacity * 2;
            temp = malloc(newOutputCapacity * U_SIZEOF_UCHAR);
            memcpy(temp, output, outputCapacity * U_SIZEOF_UCHAR);
            free(output);
            
            outputCapacity = newOutputCapacity;
            output = temp;
        } else if (errorCode != U_ZERO_ERROR) {
            UErrorCode status = U_ZERO_ERROR;
            char *p = toChar(uregex_pattern(regex, NULL, &status));
            char *r = toChar(pattern[i]->replace);
            printf("ERROR(%i) regex id (%d) pattern (%s) while replacing with replace string (%s).\n", errorCode, i, p, r);
            free(p);
            free(r);
        }
    }
    
    char *outCStr = toChar(output);
    free(output);
    
    pthread_mutex_unlock(&mutex);
    
    return outCStr;
}

UChar *toUChar(const char *string)
{
    if (string == NULL) {
        return NULL;
    }
    
    UErrorCode errorCode = U_ZERO_ERROR;
    
    uint32_t outputCapacity = 100;
    UChar *output = (UChar*) malloc((outputCapacity + 10) * U_SIZEOF_UCHAR);
    
    uint32_t destLen = ucnv_toUChars(UConverterUTF8, output, outputCapacity, string, -1, &errorCode);
    if (errorCode == U_BUFFER_OVERFLOW_ERROR) {
        errorCode = U_ZERO_ERROR;
        free(output);
        output = (UChar*) malloc(destLen * U_SIZEOF_UCHAR);
        outputCapacity = destLen + 1;
        destLen = ucnv_toUChars(UConverterUTF8, output, outputCapacity, string, -1, &errorCode);
    }
    
    return output;
}

char *toChar(const UChar *ustring)
{
    if (ustring == NULL) {
        return NULL;
    }
    
    UErrorCode errorCode = U_ZERO_ERROR;
    
    uint32_t destCapcaity = 100;
    char * output = (char*) malloc((destCapcaity + 10) * sizeof(char));

    uint32_t destLen = ucnv_fromUChars(UConverterUTF8, output, destCapcaity, ustring, -1, &errorCode);

    if (errorCode == U_BUFFER_OVERFLOW_ERROR) {
        errorCode = U_ZERO_ERROR;
        free(output);
        output = (char*) malloc(destLen * sizeof(char));
        destCapcaity = destLen + 1;
        destLen = ucnv_fromUChars(UConverterUTF8, output, destCapcaity, ustring, -1, &errorCode);
    }
    
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