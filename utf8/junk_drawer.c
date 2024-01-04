//
// Created by chaya on 12/31/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void utf8ToCodePointNotation(const char* utf8String) {
    // Iterate through each character in the UTF-8 string
    while (*utf8String != '\0') {
        // Calculate the Unicode code point for the current character
        unsigned int codePoint = 0;

        // Handle multi-byte characters
        if ((*utf8String & 0xE0) == 0xC0) {
            codePoint = (*utf8String & 0x1F) << 6;
            utf8String++;
            codePoint |= (*utf8String & 0x3F);
        } else if ((*utf8String & 0xF0) == 0xE0) {
            codePoint = (*utf8String & 0x0F) << 12;
            utf8String++;
            codePoint |= ((*utf8String & 0x3F) << 6);
            utf8String++;
            codePoint |= (*utf8String & 0x3F);
        } else {
            codePoint = *utf8String;
        }

        // Print the Unicode code point in the desired notation
        printf("\\u%04X ", codePoint);

        // Move to the next character in the UTF-8 string
        utf8String++;
    }

    printf("\n");
}

int main() {
    const char* utf8String = "\U0001fa02";  // UTF-8 encoded string

    // Convert UTF-8 to Unicode code point notation and print the result
    utf8ToCodePointNotation(utf8String);

    for (int i = 0; i < strlen(utf8String); i++)
        printf("%X ", (unsigned char)utf8String[i]);

    return 0;
}


int my_utf8_decode(unsigned char *input, unsigned char *output) {
    int in_index = 0, index = 0;
    unsigned int byte, byte2, byte3, byte4, unicode;
    unsigned char byteString[7], unicodeString[5];
    while (input[in_index] != '\0') {
//        printf("%c  x%X  next: x%X\n", input[in_index], input[in_index], input[in_index+1]);
        byte = input[in_index++];
        output[index] = 0x5C;    // ASCII character backslash
        output[++index] = 0x75;  // ASCII character u
        index++;
        sprintf(byteString, "%X", byte);
//        printf("byte: x%X  byteString: %s\n", byte, byteString);
        if (byte <= 0x7F) { // Expect 1 byte
            output[index] = 0x30; // ASCII character 0
            output[++index] = 0x30;
            output[++index] = byteString[0];
            output[++index] = byteString[1];
            output[++index] = '\0';
        } else if (byte <= 0xDF) {
            byte2 = input[in_index++];
            output[index] = 0x30; // 0
            printf("%X, %X, %X\n", byte, byte2, (((byte & 0x1F) << 6) | (byte2 & 0x3F)));
            unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
            sprintf(unicodeString, "%X", unicode);
            output[++index] = unicodeString[0];
            output[++index] = unicodeString[1];
            output[++index] = unicodeString[2];
            output[++index] = '\0';
        } else if (byte <= 0xEF) {
            output[index] = (char) (0xE0 | (byte >> 12));
            output[++index] = (char) (0x80 | ((byte >> 6) & 0x3F));
            output[++index] = (char) (0x80 | (byte & 0x3F));

            byte2 = input[in_index++];
            byte3 = input[in_index++];
            output[index] = 0x30; // 0
            printf("%X, %X, %X\n", byte, byte2, (((byte & 0x1F) << 6) | (byte2 & 0x3F)));
            unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
            sprintf(unicodeString, "%X", unicode);
            output[++index] = unicodeString[0];
            output[++index] = unicodeString[1];
            output[++index] = unicodeString[2];
            output[++index] = '\0';
        } else if (byte <= 0xF4) {
            output[index] = (char) (0xF0 | (byte >> 18));
            output[++index] = (char) (0x80 | ((byte >> 12) & 0x3F));
            output[++index] = (char) (0x80 | ((byte >> 6) & 0x3F));
            output[++index] = (char) (0x80 | (byte & 0x3F));

            byte2 = input[in_index++];
            byte3 = input[in_index++];
            byte4 = input[in_index++];
            output[index] = 0x30; // 0
            printf("%X, %X, %X\n", byte, byte2, (((byte & 0x1F) << 6) | (byte2 & 0x3F)));
            unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
            sprintf(unicodeString, "%X", unicode);
            output[++index] = unicodeString[0];
            output[++index] = unicodeString[1];
            output[++index] = unicodeString[2];
            output[++index] = '\0';
        } else {
            printf("Invalid hex byte %X\n", byte);
            return 0; //unsuccessful encode
        }//end if else
    }//end while
    return 0;
}//end my_utf8_decode

//#include <stdio.h>
//#include <string.h>
////#include "my_utf8.c"
//
//int main(void) {
//
////    // Junk tests
////    printf("Hello, World!\U0001F984 \n");
////    unsigned char aleph[2] = "\u05D0";
////    printf("Aleph  %X %X  \u05D0 %c%c \n", aleph[0], aleph[1], 0xD7, 0x90);
////    printf("\xD7\x90 \n");
//
//    unsigned char a[5] = "\u05d0";
//    for (int i = 0; i < strlen(a); i++)
//        printf("a[%d] %c, %x\n", i, a[i],  a[i]);
//    printf("Print as indeces: %c%c\n", a[0], a[1]);
//    printf("Print as string: %s\n\n", a);
//
////    printf("\u05d0 \n");
////    char unicode[10] = {0x5c, 0x75, 0x30, 0x35, 0x44, 0x30};
////    printf("%s", unicode);
//
////    printf("%c, %x, %d", '5', '5', '5' < 0x40);
//
////    printf("1 Byte\n");
////    test_my_utf8_encode("\\u0000");
////    test_my_utf8_encode("\\u001F");
////    test_my_utf8_encode("\\u007F");
////    printf("2 Byte\n");
////    test_my_utf8_encode("\\u0080");
////    test_my_utf8_encode("\\u01FF");
////    test_my_utf8_encode("\\u07FF");
////    printf("3 Byte\n");
////    test_my_utf8_encode("\\u0800");
////    test_my_utf8_encode("\\u1FFF");
////    test_my_utf8_encode("\\uFFFF");
////    printf("4 Byte\n");
////    test_my_utf8_encode("\\U00010000");
////    test_my_utf8_encode("\\U0010FFFF");
//
//}//end main
//
