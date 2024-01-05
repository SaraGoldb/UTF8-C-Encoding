// UTF-8 Endcoding in C
// By Sarale Goldberger

#include <stdio.h>
#include <stdlib.h>

// Function Declarations
int my_utf8_encode(unsigned char *input, unsigned char *output);
int my_utf8_decode(unsigned char *input, unsigned char *output);
int my_utf8_check(unsigned char *string);
int my_utf8_strlen(unsigned char *string);
unsigned char *my_utf8_charat(unsigned char *string, int index);
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2);
int my_utf8_gematria(int g, unsigned char *output);                     // extra function
int my_utf8_numbytes(unsigned char *c);                                 //extra function
int my_utf8_validHex(unsigned char c);                                  //extra function, demonstrated use in encode
int my_utf8_printHex(unsigned char *string, int rep);                   //extra function, demonstrated use in test funcs
int my_utf8_print_conversion_table();                                   //extra function, printed at the top of main
int my_strlen(unsigned char *string);

int BUFFER = 90, NUM_PASS = 0, NUM_FAIL = 0;
int test_pass_fail(void);
int test_header(char* func);
int my_utf8_encode_tests(void);
int my_utf8_decode_tests(void);
int my_utf8_check_tests(void);
int my_utf8_strlen_tests(void);
int my_utf8_charat_tests(void);
int my_utf8_strcmp_tests(void);
int my_utf8_gematria_tests(void);
int my_utf8_numbytes_tests(void);

int main() {
    my_utf8_print_conversion_table();

//    my_utf8_encode_tests();     // Test encode     !!!!!!!!!!!!!!!!!!!
    my_utf8_decode_tests();     // Test decode
    my_utf8_check_tests();      // Test check
    my_utf8_strlen_tests();     // Test strlen
    my_utf8_charat_tests();     // Test charat
    my_utf8_strcmp_tests();     // Test strcmp
    my_utf8_gematria_tests();     // Test gematria
    my_utf8_numbytes_tests();   // Test numbytes

    printf("\n");       // PASS/FAIL message
    test_pass_fail();

} //end main

// Encoding a UTF8 string, taking as input an ASCII string, with UTF8 characters encoded using the Codepoint numbering
// scheme notation, and returns a UTF8 encoded string.
int my_utf8_encode(unsigned char *input, unsigned char *output) {
//    printf("input %s\n", input);
    int index = 0, limit = 0, i;
    unsigned char cur;
    unsigned char unicodeString[9];
    unsigned int unicode;
    while (*input != '\0') {
//        printf("c: %c\n", *input);
        // if we are encountering unicode notation
        cur = *(input + 1);
        if (*input == '\\' && (cur == 'u' || cur == 'U')) {
            if (cur == 'u') // will need to extract 4 digits i.e. till index 3
                limit = 3;
            if (cur == 'U') // will need to extract 8 digits i.e. till index 7
                limit = 7;
//            printf("input: %s\n", input);
            input += 2;
//            printf("input: %s\n", input);
            // We've encountered codepoint notation, so we must extract it
            // If the unicode starts with a 0, it will have 4 digits i.e. go till index 3
            // If the unicode starts with a 1, it will have 5 digits i.e. go till index 4
            // The unicode must only contain valid hex digits, and otherwise throw an error
            i = 0;
            while (i <= limit && my_utf8_validHex(*input)) { //CLEAN THIS UP *input != '\\' &&  && !EOS(*input)
                unicodeString[i] = *input;
//                printf("unicodeString %c\n", unicodeString[i]);
                input++;
                i++;
            }//end while
            unicodeString[i] = '\0';

            // if we have not filled the limit this was not unicode
            // append the (tricky) '\\u' to output, as well as the characters mistaken for hex, and break
            if ((i - 1) < limit) {
                output[index++] = '\\';
                output[index++] = cur;  // or 'U'
                for (int j = 0; j < i; j++) {
                    output[index++] = unicodeString[j];
                }//end for
//                output[index++] = *input;
            } else {
                if (sscanf(unicodeString, "%x", &unicode) != 1) {
                    printf("ERROR: converting unicode string to unicode hex failed\n");
                    return 0;
                }//end if
//            printf("unicodeString: %s, unicode: x%x\n", unicodeString, unicode);
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
                } else { // Invalid unicode code point, treat as ASCII
                    output[index++] = '\\';
                    output[index++] = 'U';
                    for (int j = 0; j < i; j++) {
                        output[index++] = unicodeString[j];
                    }//end for
                }//end if else
//            printf("utf8: x%X x%X  output: %s \n", (unsigned char)output[index-2], (unsigned char)output[index-1], output);
//            printf("input after utf8 conversion:%s\n", input);
            }//end if else
        }//end if '\u'
        else {
            output[index] = *input;
            index++;
            input++;
        } //end else
    } //end while
//    for (int i = 0; i < length; i++)
//        printf("%c", input[i]);
    output[index] = '\0';
    return 0;
} //end my_utf8_encode

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    unsigned int byte, byte2, byte3, byte4, unicode;
    unsigned char unicodeString[8];
    int index = 0;
    while (*input != '\0') {
//        printf("%c  x%X  next: x%X\n", *input, *input, *(input+1));
        *output++ = 0x5C;    // ASCII character backslash
        index++;
        byte = *input++;
        if ((byte & 0xF8) == 0xF0) { //byte <= 0xF4
            byte2 = *input++;
            byte3 = *input++;
            byte4 = *input++;

            // make sure the utf8 is valid         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if (byte2 < 0x80 || byte2 > 0xC0 || byte3 < 0x80 || byte3 > 0xC0 || byte4 < 0x80 || byte4 > 0xC0) {
                // RESET output to "\\u0000"
                *output++ = 0x75;  // ASCII character u
                for (int i = 0; i < 4; i++)
                    *output++ = '0';
                *output = '\0';
                return 0;
            }// end if valid

            unicode = (byte & 0x07) << 18 | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
            sprintf(unicodeString, "%X", unicode);
//            printf("byte: x%X  unicode: x%X  unicodeString: %s\n", byte, unicode, unicodeString);
            *output++ = 0x55;  // ASCII character U
            *output++ = 0x30; // ASCII character 0
            *output++ = 0x30; // ASCII character 0
            if (my_strlen(unicodeString) == 5) {
                *output++ = 0x30; // ASCII character 0
                index++;
            }//end if len 5
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
            *output++ = unicodeString[3];
            *output++ = unicodeString[4];
            *output++ = unicodeString[5];
            index+=9;
            *output = '\0';
        } else {
            *output++ = 0x75;  // ASCII character u
            index++;
//        printf("byte: x%X  unicodeString: %s\n", byte, unicodeString);
            if (byte <= 0x7F) { // 1 byte
                sprintf(unicodeString, "%X", byte);
                *output++ = 0x30; // ASCII character 0
                *output++ = 0x30; // ASCII character 0
                *output++ = unicodeString[0];
                *output++ = unicodeString[1];
                index+=4;
                *output = '\0';
            } else if ((byte & 0xE0) == 0xC0) { //2 bytes //byte <= 0xDF
                byte2 = *input++;

                // make sure the utf8 is valid         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                if (byte2 < 0x80 || byte2 > 0xC0) {
                    // RESET output to "\\u0000"
                    for (int i = 0; i < 4; i++)
                        *output++ = '0';
                    *output = '\0';
                    return 0;
                }// end if valid

                unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
                sprintf(unicodeString, "%X", unicode);
                *output++ = 0x30; // ASCII character 0
                *output++ = unicodeString[0];
                *output++ = unicodeString[1];
                *output++ = unicodeString[2];
                index+=4;
                *output = '\0';
            } else if ((byte & 0xF0) == 0xE0) { // 3 bytes //byte <= 0xEF
                byte2 = *input++;
                byte3 = *input++;

                // make sure the utf8 is valid         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                if (byte2 < 0x80 || byte2 > 0xC0 || byte3 < 0x80 || byte3 > 0xC0) {
                    // RESET output to "\\u0000"
                    for (int i = 0; i < 4; i++)
                        *output++ = '0';
                    *output = '\0';
                    return 0;
                }// end if valid

                unicode = (byte & 0x0F) << 12 | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
                sprintf(unicodeString, "%X", unicode);
                if (my_strlen(unicodeString) == 3)
                    *output++ = 0x30; // ASCII character 0
                *output++ = unicodeString[0];
                *output++ = unicodeString[1];
                *output++ = unicodeString[2];
                if (my_strlen(unicodeString) == 4)
                    *output++ = unicodeString[3];
                index+=4;
                *output = '\0';
            } else {
                printf("Invalid hex byte x%X\n", byte);
                // RESET output to "\\u0000"
                for (int i = 0; i < 4; i++)
                    *output++ = '0';
                *output = '\0';
                return 0; //unsuccessful encode
            }//end if else
        }//end else lower bytes
    }//end while
    return 0;
}//end my_utf8_decode

// Validates that the input string is a valid UTF8 encoded string.
// A valid UTF8 encoded string is at least 0x0 and at most 0x10FFFF
// If invalid returns 0, if valid returns the number of bytes
int my_utf8_check(unsigned char *string) {
    unsigned int byte, byte2, byte3, byte4; //num = 1
    while (*string != '\0') {
        byte = *string++;
        if ((byte & 0xE0) == 0xC0) { //2 bytes
//            num = 2;
            byte2 = *string;
            if (byte2 < 0x80 || byte2 > 0xBF)
                return 0;
        }
        if ((byte & 0xF0) == 0xE0) { // 3 bytes
//            num = 3;
            byte2 = *string++;
            byte3 = *string;
            if (byte2 < 0x80 || byte2 > 0xBF || byte3 < 0x80 || byte3 > 0xBF)
                return 0;
        }
        if ((byte & 0xF8) == 0xF0) { //4 bytes
//            num = 4;
            byte2 = *string++;
            byte3 = *string++;
            byte4 = *string;
            if (byte2 < 0x80 || byte2 > 0xBF || byte3 < 0x80 || byte3 > 0xBF || byte4 < 0x80 || byte4 > 0xBF){
                return 0;
            }
        }
        if (byte > 0xF7) {
            return 0;
        }
        string++;
    }//end while
    return 1; //if we made it here, the string is valid
}//end my_utf8_check

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        if (*string <= 0x7F) {                 // 1 byte
            string++;
        } else if ((*string & 0xE0) == 0xC0) { //2 bytes
            string+=2;
        } else if ((*string & 0xF0) == 0xE0) { // 3 bytes
            string+=3;
        } else if ((*string & 0xF8) == 0xF0) { // 4 bytes
            string+=4;
        }//end if else
        count++;
    }//end while
    return count;
}

// Returns the UTF8 encoded character at the location specified.
// If the input string is improperly encoded, this function should return NULL to indicate an error.
unsigned char *my_utf8_charat(unsigned char *string, int index) {
    if ((index >= my_strlen(string)) || (index < 0))
        return NULL;
//    printf("START input %s, index %d\n", string, index);
    unsigned char output[7];
    string += index;
    if (*string <= 0x7F) {                 // 1 byte
//        printf("1 byte\n");
        output[0] = *string;
        output[1] = '\0';
//        printf("END input %s, index %d\n", string, index);
        unsigned char *ptr = output;
        return ptr;
    }
    if ((*string & 0xE0) == 0xC0) { //2 bytes
        output[0] = *string++;
        output[1] = *string;
        output[2] = '\0';
        unsigned char *ptr = output;
        return ptr;
    }
    if ((*string & 0xF0) == 0xE0) { // 3 bytes
        output[0] = *string++;
        output[1] = *string++;
        output[3] = *string;
        output[4] = '\0';
        unsigned char *ptr = output;
        return ptr;
    }
    if ((*string & 0xF8) == 0xF0) { // 4 bytes
        output[0] = *string++;
        output[1] = *string++;
        output[2] = *string++;
        output[3] = *string;
        output[4] = '\0';
        unsigned char *ptr = output;
        return ptr;
    }//end if else
//    printf("END input %s, index %d\n", string, index);
    return NULL;
}

// Returns whether the two strings are the same (similar result set to strcmp() )
// Returns 0 if they are equal, 1 if string1 > string2, and -1 if str1 < str2
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    int numbytes1, numbytes2;
//    printf("Before while %s, %s\n", string1, string2);
    while (*string1 != '\0' && *string2 != '\0') {
        numbytes1 = my_utf8_numbytes(string1);
        numbytes2 = my_utf8_numbytes(string2);
//        printf("%c %d byte, %c %d byte\n", *string1, numbytes1, *string2, numbytes2);
        if (numbytes1 > numbytes2)
            return 1;
        if (numbytes1 < numbytes2)
            return -1;
        if (numbytes1 == numbytes2) {
            for (int i = 1; i <= numbytes1; i++) {
//                printf("--> loop %d/%d: %c %x, %c %x\n", i, numbytes1, *string1, *string1, *string2, *string2);
                if (*string1 > *string2) {
//                    printf("----> return %d: %c > %c \n\n", *string1 > *string2, *string1, *string2);
                    return 1;
                }
                if (*string1 < *string2){
//                    printf("----> return %d: %c < %c \n\n", *string1 < *string2, *string1, *string2);
                    return -1;
                }
                string1++;
                string2++;
//                printf("----> aftr: %c, %c\n", *string1, *string2);
            }//end for
        }//end if ==
    }//end while
//    printf("After while %c, %c\n", *string1, *string2);

    // if we fall out of the loop it means all characters checked are the same, and either;
    // the strings are equal length, return 0
    // string1 is longer than string2, return 1
    // string1 is shorter than string2, return -1
    if (*string1 == '\0' && *string2 == '\0')
        return 0;
    if (*string2 == '\0')
        return 1;
    if (*string1 == '\0')
        return -1;
    return 2; //function should never return 2, unless input is invalid !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}//end my_utf8_strcmp

// Given an int, return a UTF8 encoded Hebrew character that corresponds to that int (i.e. its gematria)
int my_utf8_gematria(int g, unsigned char *output){
//    printf();
    int base = 0x8F;
    // Make sure g is a valid gematria
    if (g < 1 || g > 27) {
        output = NULL;
        return 0; //invalid input
    }//end if invalid
    // The first byte will always be 0xD7
    output[0] = 0xD7;
    // We need to account for final letters, which should be skipped over
    if (g > 10) base++;
    if (g > 13) base++;
    if (g > 15) base++;
    if (g > 19) base++;
    if (g > 21) base++;
    output[1] = (base + g);
    output[2] = '\0';
    return 1;
}// end my_utf8_hebrew

// Takes in a UTF-8 encoded character pointer and returns the number of bytes in that character
int my_utf8_numbytes(unsigned char *c) {
    if (my_utf8_check(c) == 0) { //invalid utf8 character
        return 0;
    }
    if (*c <= 0x7F)          // 1 byte
        return 1;
    if ((*c & 0xE0) == 0xC0) //2 bytes
        return 2;
    if ((*c & 0xF0) == 0xE0) // 3 bytes
        return 3;
    if ((*c & 0xF8) == 0xF0) // 4 bytes
        return 4;
    return -1; //function fail
}//end my_utf8_numbytes

// Returns 1 if the character is valid for hex, i.e. [0-9] or [A-F] or [a-f]
int my_utf8_validHex(unsigned char c) {
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
} //end my_utf8_validHex

int my_utf8_printHex(unsigned char *string, int rep) {
    int length = my_strlen(string);
    // include string representation
    if (rep)
        printf("%s ", string);
    printf("{");
    for (int i = 0; i < length; i++) {
        printf("0x%X", (unsigned char) string[i]);
        if (i < length-1) // fencepost to avoid trailing ", "
            printf(", ");
    }//end for
    printf("} ");
    return 0;
}//end my_utf8_printHex


// Do you hate always having to look up the codepoint-UTF8 conversion table? Me too! Here's a helpful function
// that prints it out for you
int my_utf8_print_conversion_table() {
    printf("%-12s  %-12s  ", "First CP", "Last CP");
    for (int i = 1; i <=4; i++) printf("%s %-9d", "Byte", i);
    printf("\n");
    char* data[18] = {"\\u0000", "\\u007F","0xxxxxxx",
                      "\\u0080", "\\u07FF", "110xxxxx", "10xxxxxx",
                      "\\u0800", "\\uFFFF", "1110xxxx", "10xxxxxx", "10xxxxxx",
                      "\\U00010000","\\U0010FFFF", "11110xxx", "10xxxxxx", "10xxxxxx", "10xxxxxx"};
    int rep = 0, watch = 3;
    for (int i = 0; i < 18 ; i++) {
        printf("%-12s  ", data[i]);
        rep++;
        if (rep == watch) {
            printf("\n");
            rep = 0;
            watch++;
        }//end if
    }//end for
    return 0;
}//end my_utf8_print_conversion_table

// Returns the amount of characters in an ASCII string
int my_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen


 /////////  TEST FUNCTIONS   ///////

 // Given a string function name, print a header for that function
int test_header(char *func) {
    printf("\n#########################################\n");  // MAKE THIS INTO A FUNCTION
    printf("          Test %s\n", func);                       //printf("\t\t\t");    test_pass_fail();
    printf("#########################################\n\n");
}//end test_header
// This function prints the number of tests passed/failed
int test_pass_fail(void) {
    if (NUM_FAIL == 0)
        printf("ALL TESTS PASSED 🥳🥳🥳🥳\n");
    else // if NUM_FAIL > 0
        printf("TESTS PASSED: %d  TESTS FAILED: %d 😭😭😭\n", NUM_PASS, NUM_FAIL);
    return 0;
}//end test_pass_fail

int test_my_utf8_encode(unsigned char *input, unsigned char *expected) { //, int verbose
    // Each character of the input ASCII can be at most 4 bytes in UTF-8
    // i.e. the encoded UTF-8 can be at most 4 times the length of the original ASCII string +1 for the terminating '00'
    int max_len = 4*my_strlen(input)+1;
    // initializing output string with max len
    unsigned char output[max_len];
    my_utf8_encode(input, output);
//    if (verbose) {
    // print input, output, and hex output after calling my_utf8_encode
    printf("Input: %s \nOutput: %s\nHex Output: ", input, output);
    my_utf8_printHex(output, 0);
    printf("\n");
//    }//end if verbose
    // print a line of '-' and assert if output matched expected output
    int match = my_utf8_strcmp(expected, output);
    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_encode
int my_utf8_encode_tests() {
    test_header("my_utf8_encode");
    unsigned char *input, *expected;
    input = "Aleph \\u05D0"; expected = "Aleph \u05D0";
    test_my_utf8_encode(input, expected);
    test_my_utf8_encode("Happy \\U0001F600 YAY! \\u066D", "Happy \U0001F600 YAY! \u066D");
    test_my_utf8_encode("\\U0001F44B\\U0001F30E Hello, World!", "\U0001F44B\U0001F30E Hello, World!");
    test_my_utf8_encode("umm\\U0001F914umm\\U0001F928umm", "umm\U0001F914umm\U0001F928umm");
    test_my_utf8_encode("\\Unicorn\\U0001F984\\unicorn", "\\Unicorn\U0001F984\\unicorn");
    test_my_utf8_encode("Trying to be sneaky are we? \\U1111FFFF", "Trying to be sneaky are we? \\U1111FFFF");
    test_my_utf8_encode("This is how to write \"Hello\" in Khmer: \\u1787\\u17C6\\u179A\\u17B6\\u1794\\u179F\\u17BD\\u179A",
                        "This is how to write \"Hello\" in Khmer: \u1787\u17C6\u179A\u17B6\u1794\u179F\u17BD\u179A");
    test_my_utf8_encode("\\U0010FFFF", "\U0010FFFF");
    test_my_utf8_encode("\\uber\\\\U0001F695\\Uber\\", "\\uber\\\U0001F695\\Uber\\");
    test_my_utf8_encode("Sarale \\u05E9\\u05E8\\u05D4\\u05DC\\u05D4", "Sarale \u05E9\u05E8\u05D4\u05DC\u05D4");
    test_my_utf8_encode("\\u00af\\u005c\\u005f\\u0028\\u30c4\\u0029\\u005f\\u002f\\u00af \\u0028\\u25D5\\u203f\\u25D5\\u0029",
                        "¯\\_(ツ)_/¯ (◕‿◕)");
}//end my_utf8_encode_tests

int test_my_utf8_decode(unsigned char *input, unsigned char *expected) {
    // Each bytes of UTF-8 encoded string can be at most
    // i.e. the decoded ASCII can be at most 6 times the number of characters in the input string +1 for the terminating '00'
    int max_len = 6*my_strlen(input)+1;
    // initializing output string with max len
    unsigned char output[max_len];
    my_utf8_decode(input, output);
//    if (verbose) {
    printf("Input: ");
    my_utf8_printHex(input, 1);
    printf("\nOutput:   %s\nExpected: %s\n", output, expected);
    // print a line of '-' and assert if output matched expected output
    int match = my_utf8_strcmp(expected, output);
    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
    return 0;
}// end test_my_utf8_decode
int my_utf8_decode_tests() {
    test_header("my_utf8_decode");
    unsigned char A[] = {0x41, 0x0};
    test_my_utf8_decode(A, "\\u0041");
    unsigned char Hello[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x0};
    test_my_utf8_decode(Hello, "\\u0048\\u0065\\u006C\\u006C\\u006F");
    unsigned char aleph[] = {0xd7, 0x90, 0x0};
    test_my_utf8_decode(aleph, "\\u05D0");
    unsigned char temp3[] = {0xE0, 0xA0, 0x80, 0x0};
    test_my_utf8_decode(temp3, "\\u0800");
    unsigned char cent[] = {0xEF, 0xBF, 0xA0, 0x0};
    test_my_utf8_decode(cent, "\\uFFE0");
    test_my_utf8_decode("\uFFFE", "\\uFFFE");
    unsigned char antenna[] = {0xF0, 0x90, 0x80, 0x80, 0x0};
    test_my_utf8_decode(antenna, "\\U00010000");
    unsigned char temp4[] = {0xF0, 0x9F, 0xA8, 0x82, 0x0};
    test_my_utf8_decode(temp4, "\\U0001FA02");
    unsigned char unicorn[] = {0xF0, 0x9F, 0xA6, 0x84, 0x0};
    test_my_utf8_decode(unicorn, "\\U0001F984");
    test_my_utf8_decode("\U00010000", "\\U00010000");
    test_my_utf8_decode("\U00010348", "\\U00010348");
    test_my_utf8_decode("\U00100000", "\\U00100000");
    test_my_utf8_decode("\U0010FFFF", "\\U0010FFFF");
    unsigned char invalid2[] = {0xC0, 0x05, 0x0};
    test_my_utf8_decode(invalid2, "\\u0000");
    unsigned char invalid3[] = {0xE0, 0x05, 0x02, 0x0};
    test_my_utf8_decode(invalid3, "\\u0000");
    unsigned char invalid3a[] = {0xE0, 0xA0, 0x02, 0x0};
    test_my_utf8_decode(invalid3a, "\\u0000");
    unsigned char invalid4[] = {0xF0, 0xA0, 0xC7, 0xA0, 0x0};
    test_my_utf8_decode(invalid4, "\\u0000");
    unsigned char invalid4a[] = {0xF0, 0xA0, 0x95, 0x02, 0x0};
    test_my_utf8_decode(invalid4a, "\\u0000");
    unsigned char invalid4b[] = {0xF0, 0xC7, 0x95, 0x02, 0x0};
    test_my_utf8_decode(invalid4b, "\\u0000");
    unsigned char invalidX[] = {0xFF, 0x05, 0x0};
    test_my_utf8_decode(invalidX, "\\u0000");
}//end my_utf8_decode_tests

int test_my_utf8_check(unsigned char *string, int expected) {
    int output = my_utf8_check(string);
    my_utf8_printHex(string, 1);
    printf("%s a valid UTF8 string\n", output == 1 ? "is" : "is NOT");
    // print a line of '-' and assert if output matched expected output
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    output == expected ? NUM_PASS++ : NUM_FAIL++; // increment pass/fail counters
    printf(" %s\n", output == expected ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_check
int my_utf8_check_tests() {
    test_header("my_utf8_check");
    test_my_utf8_check("\u0040", 1);
    test_my_utf8_check("\u0060", 1);
    test_my_utf8_check("Apple", 1);
    test_my_utf8_check("\u00A5", 1);
    test_my_utf8_check("\u0613", 1);
    test_my_utf8_check("\u0770", 1);
    test_my_utf8_check("\u10FF", 1);
    test_my_utf8_check("\uFFFE", 1);
    test_my_utf8_check("\U00010000", 1);
    test_my_utf8_check("\U0001ABCD", 1);
    test_my_utf8_check("\U0001FFFF", 1);

    unsigned char invalid2[] = {0xC0, 0x05, 0x0};
    unsigned char invalid3[] = {0xE0, 0x05, 0x02, 0x0};
    unsigned char invalid3a[] = {0xE0, 0xA0, 0x02, 0x0};
    unsigned char invalid4[] = {0xF0, 0xA0, 0xC7, 0xA0, 0x0};
    unsigned char invalid4a[] = {0xF0, 0xA0, 0x95, 0x02, 0x0};
    unsigned char invalid4b[] = {0xF0, 0xC7, 0x95, 0x02, 0x0};
    unsigned char invalidX[] = {0xFF, 0x05, 0x0};
    unsigned char *invalid_strings[] = {invalid2, invalid3, invalid3a,
                                        invalid4, invalid4a, invalid4b, invalidX};
    for (int i = 0; i < 7; i++)
        test_my_utf8_check(invalid_strings[i], 0);
}//end my_utf8_check_tests

int test_my_utf8_strlen(unsigned char *string, int expected) {
    int output = my_utf8_strlen(string);
    printf("Input: %s \t len: %d   expected: %d\n", string, output, expected);
    // print a line of '-' and assert if output matched expected output
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    output == expected ? NUM_PASS++ : NUM_FAIL++; // increment pass/fail counters
    printf(" %s\n", output == expected ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_strlen
int my_utf8_strlen_tests() {
    test_header("my_utf8_strlen");
    test_my_utf8_strlen("!", 1);
    test_my_utf8_strlen("A", 1);
    test_my_utf8_strlen("€", 1);
    test_my_utf8_strlen("א", 1);
    test_my_utf8_strlen("\U0001F984", 1);
    test_my_utf8_strlen("\u07FF", 1);
    test_my_utf8_strlen("\u0800", 1);
    test_my_utf8_strlen("\uFFFF", 1);
    test_my_utf8_strlen("\U00010000", 1);
    test_my_utf8_strlen("\U0010FFFF", 1);
    test_my_utf8_strlen("$€¥₡£€¢₪", 8);
    test_my_utf8_strlen("אני אוהבת מדעי המחשב!", 21);
    test_my_utf8_strlen("This is \"Hello\" in Tamil: வணக்கம்", 33);
    test_my_utf8_strlen("\U0010FFFF \uFFFF", 3);
    test_my_utf8_strlen("שרהלה", 5);
    test_my_utf8_strlen("(◕‿◕) (◕‿◕)", 11);
}//end my_utf8_strlen_tests

int test_my_utf8_charat(unsigned char *string, int index, unsigned char *expected) {
    unsigned char *output = my_utf8_charat(string, index);
    int match;
    printf("Input: %s\nIndex: %d\nOutput:   %s\nExpected: %s\n", string, index, output, expected);
    // print a line of '-' and assert if output matched expected output
    if (output == NULL)  match = (expected == NULL ? 0 : 1);       // if output is NULL don't enter my_utf8_strcmp
    else                 match = my_utf8_strcmp(expected, output);
    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_charat
int my_utf8_charat_tests(){
    test_header("my_utf8_charat");
    unsigned char *in;
    unsigned char *exp;
    in = "Aleph \u05D0"; exp = "\u05D0";
    test_my_utf8_charat(in, 6, exp);
    in = "Unicorn"; exp = "i";
    test_my_utf8_charat(in, 2, exp);
    in = "Unicorn \U0001F984 yay"; exp = "🦄";
    test_my_utf8_charat(in, 8, exp);
    in = "Unicorn"; exp = NULL;
    test_my_utf8_charat(in, 8, exp);
    in = "Unicorn \U0001F984 yay"; exp = NULL;
    test_my_utf8_charat(in, 18, exp);
    in = "Hello World"; exp = NULL;
    test_my_utf8_charat(in, -1, exp);
}//end my_utf8_charat_tests

int test_my_utf8_strcmp(unsigned char *string1, unsigned char *string2, int expected) {
    int output = my_utf8_strcmp(string1, string2);
    printf("String1: %s\nString2: %s\nOutput: %d   Expected: %d\n", string1, string2, output, expected);
    // print a line of '-' and assert if output matched expected output
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    output == expected ? NUM_PASS++ : NUM_FAIL++; // increment pass/fail counters
    printf(" %s\n", output == expected ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_strcmp
int my_utf8_strcmp_tests(){
    test_header("my_utf8_strcmp");
    test_my_utf8_strcmp("aaa", "aaa", 0);       // 1 byte comparison
    test_my_utf8_strcmp("aaa", "bbb", -1);
    test_my_utf8_strcmp("bbb", "aaa", 1);
    test_my_utf8_strcmp("app", "apple", -1);
    test_my_utf8_strcmp("apple", "app", 1);
    test_my_utf8_strcmp("א", "א", 0);            // 2 byte comparisons
    test_my_utf8_strcmp("א", "ת", -1);
    test_my_utf8_strcmp("ת", "א", 1);
    test_my_utf8_strcmp("שרהלה", "שרהלה", 0);
    test_my_utf8_strcmp("שרהלא", "שרהלה", -1);
    test_my_utf8_strcmp("שרהלה", "שרהלא", 1);
    test_my_utf8_strcmp("שרה", "שרהלה", -1);
    test_my_utf8_strcmp("שרהלה", "שרה", 1);
    unsigned char *theta = "\u2C90";
    unsigned char *rho = "\u2CA3";
    test_my_utf8_strcmp(theta, theta, 0);       // 3 byte comparisons
    test_my_utf8_strcmp(theta, rho, -1);
    test_my_utf8_strcmp(rho, theta, 1);
    unsigned char *antennas = "\U00010000";
    unsigned char *torch = "\U00010006";
    test_my_utf8_strcmp(torch, torch, 0);       // 4 byte comparisons
    test_my_utf8_strcmp(antennas, torch, -1);
    test_my_utf8_strcmp(torch, antennas, 1);
    test_my_utf8_strcmp("\U0001F984Unicorn", "\U0001F984Unicorn", 0);   // Mixed byte comparisons
    test_my_utf8_strcmp("\U0001F984Unicorn", "\U0001F984Unicorns are cool!", -1);
    test_my_utf8_strcmp("\U0001F984Unicorns are cool!", "\U0001F984Unicorn", 1);
    test_my_utf8_strcmp("A \u05D0 \u0840 \uFFF0 \u1200", "A \u05D0 \u0840 \uFFF0 \u1200", 0);
    test_my_utf8_strcmp("A \u05D0 \u0840 \u1200", "A \u05D0 \u0840 \uFFF0 \u1200", -1);
    test_my_utf8_strcmp("A \u05D0 \u0840 \uFFF0 \u1240", "A \u05D0 \u0840 \uFFF0 \u1200", 1);
}//end my_utf8_strcmp_tests

int test_my_utf8_gematria(int g, unsigned char *expected) {
    // Initialize an output string and invoke the function
    unsigned char output[3];
    my_utf8_gematria(g, output);
    // Print the input, output, and expected output
    printf("Gematria: %d\nOutput:   ", g);
    my_utf8_printHex(output, 1);
    printf("\nExpected: ");
    my_utf8_printHex(expected, 1);
    printf("\n");
    // print a line of '-' and assert if output matched expected output
    int match = my_utf8_strcmp(expected, output);
    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_gematria
int my_utf8_gematria_tests(void) {
    test_header("my_utf8_gematria_tests");
    // Initialize the base UTF8 character, Aleph
    char base[3] = {0xD7, 0x90, 0x0};
    // Test for the first ten
    for (int i = 1; i < 28; i++) {
        test_my_utf8_gematria(i, base);
        base[1]++;
        if (i == 10 || i == 13|| i == 15 || i == 19 || i == 21) base[1]++;
    }//end for 10
//    // Increment to account for the final Hebrew letters
//    base[1]++;
//    test_my_utf8_gematria(11, base);
//    base[1]++;
//    test_my_utf8_gematria(12, base);
//    for (int i = 11; i < 10; i++) {
//        test_my_utf8_gematria(i, base);
//        base[1]++;
//    }//end for
}//end my_utf8_gematria_tests

int test_my_utf8_numbytes(unsigned char *c, int expected) {
    // Invoke the function and save the output
    int output = my_utf8_numbytes(c);
    // Print the input, output, and expected output
    printf("Character: ");
    my_utf8_printHex(c, 1);
    printf("\nBytes: %d   Expected: %d\n", output, expected);
    // print a line of '-' and assert if output matched expected output
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    output == expected ? NUM_PASS++ : NUM_FAIL++; // increment pass/fail counters
    printf(" %s\n", output == expected ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_numbytes
int my_utf8_numbytes_tests(void) {
    test_header("my_utf8_numbytes");
    test_my_utf8_numbytes("\u0040", 1);
    test_my_utf8_numbytes("\u0060", 1);
    test_my_utf8_numbytes("\u00A0", 2);
    test_my_utf8_numbytes("\u0770", 2);
    test_my_utf8_numbytes("\u0800", 3);
    test_my_utf8_numbytes("\uABCD", 3);
    test_my_utf8_numbytes("\U00010000", 4);
    test_my_utf8_numbytes("\U0010ABCD", 4);

    unsigned char invalidA[] = {0xF0, 0xA0, 0x95, 0x02, 0x0};
    unsigned char invalidB[] = {0xF0, 0xC7, 0x95, 0x02, 0x0};
    unsigned char invalidD[] = {0xFF, 0xA0, 0xC7, 0xA0, 0x0};
    unsigned char invalidC[] = {0xFF, 0x05, 0x0};
    unsigned char *invalid_strings[] = {invalidA, invalidB, invalidC, invalidD};
    for (int i = 0; i < 4; i++)
        test_my_utf8_numbytes(invalid_strings[i], 0);
}//end my_utf8_numbytes_tests