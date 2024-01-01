// UTF-8 Endcoding in C
// By Sarale Goldberger

#include <stdio.h>

int my_strlen(unsigned char *string);
int my_utf8_encode(unsigned char *input, unsigned char *output);
int my_utf8_decode(unsigned char *input, unsigned char *output);
int my_utf8_check(unsigned char *string);

int test_my_utf8_check(unsigned char *string) {
    printf("%-8s is a valid UTF8 string: %s\n", string, my_utf8_check(string) == 1 ? "True" : "False");
    return 0;
}//end test_my_utf8_check

int test_my_utf8_encode(unsigned char *input) {
    // Each character of the input ASCII can be at most 4 bytes in UTF-8
    int max_len = 4*my_strlen(input)+1;
    unsigned char output[max_len];
    my_utf8_encode(input, output);
    printf("\nInput: %s \nOutput: %s\nHex Output: ", input, output);
    for (int i = 0; i < my_strlen(output); i++)
        printf("%X ", (unsigned char)output[i]);
    printf("\n--\n");
    return 0;
}//end test_my_utf8_encode

int main() {
    // Junk tests
    printf("Hello, World!\U0001F984 \n");
    unsigned char aleph[2] = "\u05D0";
    printf("Aleph  %X %X  \u05D0 %c%c \n", aleph[0], aleph[1], 0xD7, 0x90);
    printf("\xD7\x90 \n");


    // Test my_utf8_check
//    printf("\n----- Test my_utf8_check -----\n");
//    test_my_utf8_check("\\u0000");
//    test_my_utf8_check("\\u10FF");
//    test_my_utf8_check("\\u11FFFF");
//    test_my_utf8_check("\\uFFFFFF");

    // Test my_utf8_encode
    printf("----- Test my_utf8_encode -----\n");
    test_my_utf8_encode("Aleph \\u05D0");
    test_my_utf8_encode("Sarale \\u05E9\\u05E8\\u05D4\\u05DC\\u05D4");
    test_my_utf8_encode("Happy \\u1F600 YAY!");
    test_my_utf8_encode("\\u1F44B\\u1F30E Hello, World!");
    test_my_utf8_encode("umm\\u1F914umm\\u1F914");
    test_my_utf8_encode("");
    printf("¯\\_(ツ)_/¯");


} //end main

// Returns the amount of characters in the string
int my_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen

// Returns 1 if the character indicates the end of the word/string
int EOS(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
} //end EOS

// Returns 1 if the character is valid for hex, i.e. [0-9] or [A-F] or [a-f]
int validHex(unsigned char c) {
    // check if c is a character [0-5] or [A-F] or [a-f]
    for (int i = 0; i < 6; i++)
        if (c == '0'+i || c == 'A'+i || c == 'a'+i)
            return 1;
    // check if c is a character [6-9]
    for (int i = 6; i <= 9; i++)
        if (c == '0'+i)
            return 1;
    // if c is invalid, return 0
    return 0;
} //end validHex

// Validates that the input string is a valid UTF8 encoded string.
// Invalid if it is out of range. (negative? or greater than \\u10FFFF)
int my_utf8_check(unsigned char *string) {
    string += 2;
//    return ((int)string <= 0x10FFFF);
    return 0;
}

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.

// Input: "Aleph \u05D0"
// Output: xD7x90
int my_utf8_encode(unsigned char *input, unsigned char *output) {
//    printf("input %s\n", input);
    int index = 0;
    unsigned char unicodeString[5];
    unsigned int unicode;
    while (*input != '\0') {
//        printf("c: %c\n", *input);
        // if we are encountering unicode
        if (*input == '\\' && *(input+1) == 'u') {
//            printf("input: %s\n", input);
            input += 2;
//            printf("input: %s\n", input);
            // We've encountered codepoint notation, so we must extract it
            // If the unicode starts with a 0, it will have 4 digits i.e. go till index 3
            // If the unicode starts with a 1, it will have 5 digits i.e. go till index 4
            // The unicode must only contain valid hex digits, and otherwise throw an error
            int i = 0, limit = 3;
            if (*input == '1')
                limit = 4;
            while (*input != '\\' && validHex(*input) && !EOS(*input) && i <= limit) {
                unicodeString[i] = *input;
                input++;
                i++;
            }//end while
            unicodeString[i] = '\0';
            if (sscanf(unicodeString, "%x", &unicode) != 1) {
                printf("ERROR: converting unicode string to unicode hex failed\n");
                return 0; //unsuccessful encode
            }//end if
//            printf("unicodeString: %s, unicode: %x\n", unicodeString, unicode);
            if (unicode <= 0x7F) {
                output[index] = (char) unicode;
                output[++index] = '\0';
            } else if (unicode <= 0x7FF) {
                output[index] = (char) (0xC0 | (unicode >> 6));
                output[++index] = (char) (0x80 | (unicode & 0x3F));
                output[++index] = '\0';
            } else if (unicode <= 0xFFFF) {
                output[index] = (char) (0xE0 | (unicode >> 12));
                output[++index] = (char) (0x80 | ((unicode >> 6) & 0x3F));
                output[++index] = (char) (0x80 | (unicode & 0x3F));
                output[++index] = '\0';
            } else if (unicode <= 0x10FFFF) {
                output[index] = (char) (0xF0 | (unicode >> 18));
                output[++index] = (char) (0x80 | ((unicode >> 12) & 0x3F));
                output[++index] = (char) (0x80 | ((unicode >> 6) & 0x3F));
                output[++index] = (char) (0x80 | (unicode & 0x3F));
                output[++index] = '\0';
            } else {
                printf("Invalid unicode code point");
                return 0; //unsuccessful encode
            }//end if else
//            printf("utf8: %X %X  output: %s \n", (unsigned char)output[index-2], (unsigned char)output[index-1], output);
//            printf("input after utf8 conversion:%s\n", input);
        } //end if '\u'
        else {
            output[index] = *input;
            index++;
            input++;
        } //end else
    } //end while
//    for (int i = 0; i < length; i++)
//        printf("%c", input[i]);
    return 1; //successful encode!
} //end my_utf8_encode

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    return 0;
}

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
    }
    return count;
}

// Returns the UTF8 encoded character at the location specified.
char *my_utf8_charat(unsigned char *string, int index) {
    return 0;
}

// Returns whether the two strings are the same (similar result set to strcmp() )
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    return 0;
}


// Each student should add 2 functions that they believe are useful.  We will grade based on the complexity and
// creativity of these functions.

