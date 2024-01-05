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
int my_utf8_gematria_decode(int g, unsigned char *output);             // extra function
int my_utf8_gematria_encode(unsigned char *input);                     // extra function
int my_utf8_numbytes(unsigned char *c);                                // extra function
int my_utf8_validHex(unsigned char c);                                 // extra function, demonstrated use in encode
int my_utf8_printHex(unsigned char *string, int rep);                  // extra function, demonstrated use in test funcs
int my_utf8_print_conversion_table();                                  // extra function, printed at the top of main
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
int my_utf8_gematria_decode_tests(void);
int my_utf8_gematria_encode_tests(void);
int my_utf8_numbytes_tests(void);

int main() {
    my_utf8_print_conversion_table();

    my_utf8_encode_tests();              // Test encode
    my_utf8_decode_tests();              // Test decode
    my_utf8_check_tests();               // Test check
    my_utf8_strlen_tests();              // Test strlen
    my_utf8_charat_tests();              // Test charat
    my_utf8_strcmp_tests();              // Test strcmp
    my_utf8_gematria_decode_tests();     // Test gematria decode
    my_utf8_gematria_encode_tests();     // Test gematria encode
    my_utf8_numbytes_tests();            // Test numbytes

    printf("\n");                // PASS/FAIL message
    test_pass_fail();

} //end main

// Encoding a UTF8 string, taking as input an ASCII string, with UTF8 characters encoded using the Codepoint numbering
// scheme notation, and returns a UTF8 encoded string.
int my_utf8_encode(unsigned char *input, unsigned char *output) {
    int index = 0, limit = 0, i = 0;
    unsigned char next;
    unsigned char unicodeString[9];
    unsigned int unicode;
    // Loop through the string
    while (*input != '\0') {
        // Save a pointer to the next byte
        next = *(input+1);
        // if we are encountering unicode notation
        if (*input == '\\' && (next == 'u' || next == 'U')) {
            if (next == 'u') // will need to extract 4 digits i.e. till index 3
                limit = 3;
            if (next == 'U') // will need to extract 8 digits i.e. till index 7
                limit = 7;
            // increment the pointer to slice of the "\u" or "\U"
            input += 2;
            // We've encountered codepoint notation, so we must extract it
            // The unicode must only contain valid hex digits, and otherwise throw an error
            // It must contain exactly either 4 or 8 digits
            i = 0;
            while (i <= limit && my_utf8_validHex(*input)) {
                unicodeString[i++] = *input++;
            }//end while
            unicodeString[i] = '\0';

            // If we have not filled the limit this was not unicode
            // append the (tricky) "\\u" or "\\U" to output
            // as well as the characters mistaken for hex,
            // and move on to the next character
            if ((i - 1) < limit) {
                output[index++] = '\\';
                output[index++] = next;  // 'u' or 'U'
                for (int j = 0; j < i; j++)
                    output[index++] = unicodeString[j];
            }
            // if this is unicode, we can convert the string to hex and convert it to UTF8
            else {
                if (sscanf(unicodeString, "%x", &unicode) != 1) {
                    printf("ERROR: converting unicode string to unicode hex failed\n");
                    return 0; // function failsafe
                }//end if failsafe
                // Depending on which range the unicode falls tells us how many bytes we will need
                // Follow the conversion formula (invoke my_utf8_print_conversion_table() to see the conversion table)
                if (unicode <= 0x7F) {                // 1 byte
                    output[index++] = (char) unicode;
                    output[index] = '\0';
                } else if (unicode <= 0x7FF) {        // 2 byte
                    output[index++] = (char) (0xC0 | (unicode >> 6));
                    output[index++] = (char) (0x80 | (unicode & 0x3F));
                    output[index] = '\0';
                } else if (unicode <= 0xFFFF) {       // 3 byte
                    output[index++] = (char) (0xE0 | (unicode >> 12));
                    output[index++] = (char) (0x80 | ((unicode >> 6) & 0x3F));
                    output[index++] = (char) (0x80 | (unicode & 0x3F));
                    output[index] = '\0';
                } else if (unicode <= 0x10FFFF) {     // 4 byte
                    output[index++] = (char) (0xF0 | (unicode >> 18));
                    output[index++] = (char) (0x80 | ((unicode >> 12) & 0x3F));
                    output[index++] = (char) (0x80 | ((unicode >> 6) & 0x3F));
                    output[index++] = (char) (0x80 | (unicode & 0x3F));
                    output[index] = '\0';
                } else {
                    // If we've fallen here, it means the unicode code point is invalid
                    // We must treat it as ASCII
                    output[index++] = '\\';        // Append the tricky "\\U"
                    output[index++] = 'U';
                    for (int j = 0; j < i; j++) {  // Append the unicode as regular ASCII characters
                        output[index++] = unicodeString[j];
                    }//end for
                }//end if else
            }//end if else
        }//end if '\u'
        // If we've fallen here it means the character is just normal ASCII, so append it
        else {
            output[index] = *input;
            index++;
            input++;
        } //end else
    } //end while
    // Terminate the string
    output[index] = '\0';
    return 1; // function ran
} //end my_utf8_encode

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    unsigned int byte = 0, byte2 = 0, byte3 = 0, byte4 = 0, unicode;
    unsigned char unicodeString[8], utf8[5];
    int index = 0, valid = 1;
    // Loop through the string
    while (*input != '\0') {
        *output++ = 0x5C;    // ASCII character backslash
        index++;
        byte = *input++;

        // Check if we are in the 4 byte range
        // Isolate bits [7-3] and confirm whether they match 11110
        if ((byte & 0xF8) == 0xF0) {
            byte2 = *input++;
            byte3 = *input++;
            byte4 = *input++;

            // If the UTF8 is invalid, break and reset output
            if (byte2 < 0x80 || byte2 > 0xC0 || byte3 < 0x80 || byte3 > 0xC0 || byte4 < 0x80 || byte4 > 0xC0) {
                *output++ = 0x75;  // ASCII character u
                valid = 0;
                break;
            }// end if invalid

            // Follow the formula to convert the UTF8 to Unicode
            unicode = (byte & 0x07) << 18 | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
            sprintf(unicodeString, "%X", unicode);
            // The unicode in this range will always start with 'U' and at least two 0's
            *output++ = 0x55;  // ASCII character U
            *output++ = 0x30;  // ASCII character 0
            *output++ = 0x30;  // ASCII character 0
            // Check if we need to add a leading 0
            if (my_strlen(unicodeString) == 5) {
                *output++ = 0x30; // ASCII character 0
                index++;
            }//end add 0
            // Append the rest of the unicode string to output
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
            *output++ = unicodeString[3];
            *output++ = unicodeString[4];
            *output++ = unicodeString[5];
            // Increment the index counter to account for the unicode
            index+=9;
            *output = '\0';
        }

        // If we are NOT in the 4 byte range, the unicode point will start with lowercase 'u'
        else {
            *output++ = 0x75;  // ASCII character u
            index++;

            // Check if we are in the 1 byte range and append accordingly
            // Confirm whether the byte is equal to or below the last codepoint of the 1 byte range
            if (byte <= 0x7F) {
                sprintf(unicodeString, "%X", byte);
                // The unicode in this range will always start with at least two 0's
                *output++ = 0x30; // ASCII character 0
                *output++ = 0x30; // ASCII character 0
                *output++ = unicodeString[0];
                *output++ = unicodeString[1];
                index+=4;
                *output = '\0';

            // Check if we are in the 2 byte range and append accordingly
            // Isolate bits [7-5] and confirm whether they match 110
            } else if ((byte & 0xE0) == 0xC0) {
                byte2 = *input++;
                // If the UTF8 is invalid, break and reset output
                if (byte2 < 0x80 || byte2 > 0xC0) {
                    valid = 0;
                    break;
                }// end if invalid
                // Follow the formula to convert the UTF8 to Unicode
                unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
                sprintf(unicodeString, "%X", unicode);
                // The unicode in this range will always start with at least one 0
                *output++ = 0x30; // ASCII character 0
                *output++ = unicodeString[0];
                *output++ = unicodeString[1];
                *output++ = unicodeString[2];
                index+=4;
                *output = '\0';
            }
            // Check if we are in the 3 byte range
            // Isolate bits [7-4] and confirm whether they match 1110
            else if ((byte & 0xF0) == 0xE0) {
                byte2 = *input++;
                byte3 = *input++;
                // If the UTF8 is invalid, break and reset output
                if (byte2 < 0x80 || byte2 > 0xC0 || byte3 < 0x80 || byte3 > 0xC0) {
                    valid = 0;
                    break;
                }//end if invalid
                // Follow the formula to convert the UTF8 to Unicode
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
                valid = 0;
                break;
            }//end if else
        }//end else lower bytes
    }//end while

    // If the input was invalid we must reset output to "\\u0000"
    if (!valid) {
        for (int i = 0; i < 4; i++)
            *output++ = '0';
        *output = '\0';
    }//end reset

    return 0;
}//end my_utf8_decode

// Validates that the input string is a valid UTF8 encoded string.
// A valid UTF8 encoded string is at least 0x0 and at most 0x10FFFF
// If invalid returns 0, if valid returns the number of bytes
int my_utf8_check(unsigned char *string) {
    unsigned int byte, byte2, byte3, byte4;
    int num = 1;
    // Loop through the string
    // Depending on the number of bytes
    // Follow the formula for conversion
    while (*string != '\0') {
        byte = *string++;
        // Isolating
        if ((byte & 0xE0) == 0xC0) { //2 bytes
            num = 2;
            byte2 = *string;
            if (byte2 < 0x80 || byte2 > 0xBF)
                return 0;
        }
        if ((byte & 0xF0) == 0xE0) { // 3 bytes
            num = 3;
            byte2 = *string++;
            byte3 = *string;
            if (byte2 < 0x80 || byte2 > 0xBF || byte3 < 0x80 || byte3 > 0xBF)
                return 0;
        }
        if ((byte & 0xF8) == 0xF0) { //4 bytes
            num = 4;
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
    return num; //if we made it here, the string is valid
}//end my_utf8_check

// Returns the number of characters in the string.
int my_utf8_strlen(unsigned char *string) {
    int count = 0;
    // Loop through the string
    // If we've encountered a character larger than 1 byte
    // we must increment the counter accordingly to accomodate
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
        count++; // the counter only gets incremented once per character
    }//end while
    return count;
}//end my_utf8_strlen

// Returns the UTF8 encoded character at the location specified.
// If the input string is improperly encoded, this function returns NULL to indicate an error.
unsigned char *my_utf8_charat(unsigned char *string, int index) {
    // If index is out of range return NULL
    if ((index >= my_utf8_strlen(string)) || (index < 0))
        return NULL;

    unsigned char output[7];
    // increment string to point to the index position of the character we want to return
    string += index;
    // Determine the number of bytes and append accordingly
    int bytes = my_utf8_numbytes(string);  // What a helpful function!!
    unsigned char *ptr;
    switch (bytes) {
        case 1:                      // 1 byte
            output[0] = *string;
            output[1] = '\0';
            ptr = output;
            return ptr;
        case 2:                      //2 bytes
            output[0] = *string++;
            output[1] = *string;
            output[2] = '\0';
            ptr = output;
            return ptr;
        case 3:                      // 3 bytes
            output[0] = *string++;
            output[1] = *string++;
            output[3] = *string;
            output[4] = '\0';
            ptr = output;
            return ptr;
        case 4:                      // 4 bytes
            output[0] = *string++;
            output[1] = *string++;
            output[2] = *string++;
            output[3] = *string;
            output[4] = '\0';
            ptr = output;
            return ptr;
    }//end switch
    return NULL;
}

// Returns whether the two strings are the same (similar result set to strcmp() )
// Returns 0 if they are equal, 1 if string1 > string2, and -1 if str1 < str2
int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
    int numbytes1, numbytes2;
    // Loop through the string
    while (*string1 != '\0' && *string2 != '\0') {
        numbytes1 = my_utf8_numbytes(string1);
        numbytes2 = my_utf8_numbytes(string2);
        // If string1's current character has more bytes than string2's, string1 is  lexicographically larger
        if (numbytes1 > numbytes2)
            return 1;
        // If string1's current character has less bytes than string2's, string1 is  lexicographically smaller
        if (numbytes1 < numbytes2)
            return -1;
        // If string1 and string2's current characters have the same number of bytes
        // compare the character per byte
        if (numbytes1 == numbytes2) {
            for (int i = 1; i <= numbytes1; i++) {
                if (*string1 > *string2)
                    return 1;
                if (*string1 < *string2)
                    return -1;
                string1++;
                string2++;
            }//end for
        }//end if ==
    }//end while

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
    return 2; //function should never return 2
}//end my_utf8_strcmp

// Given an int, return a UTF8 encoded Hebrew character that corresponds to that int (i.e. its gematria)
int my_utf8_gematria_decode(int g, unsigned char *output){
    unsigned int base = 0x8F;
    // Make sure g is a valid gematria
    if ((g < 1) || (g > 27)) {
        // If invalid, function returns 0x0
        output[0] = '0';
        output[1] = '\0';
        output[2] = '\0';
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

// Given a UTF8 encoded Hebrew string, return the corresponding gematria
int my_utf8_gematria_encode(unsigned char *input){
    int byte2, base = 0x8F, ch, count = 1, incr = 1;
    static int offset = 0;
    // Loop through the string
    if (*input != '\0') {
        // Make sure input is within the range {0xD7, 0x90} - {0xD7, 0xAF}
        if (my_utf8_check(input) == 0) return 0;
        if (my_utf8_numbytes(input) != 2) return 0;
        byte2 = *(input+1);
        printf("0x%x\n", byte2);
        if ((*input < 0xD7) && (byte2 < 0x90 || byte2 > 0xAF)) return 0;

        ch = byte2 - base - offset;
        for (int i = 1; i < ch; i++) {
            count += incr;
            if (i == 11 || i == 14 || i == 16 || i == 20 || i == 22) count-=incr;
            if (count == 11) {
                count = 20;
                incr = 10;
            }
            if (count == 100) {
                count = 200;
                incr = 100;
            }
        }//end for

    }//end if
    return count;
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

// Given a UTF8 encoded string print the hexidecimal values in an organized way
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
    // Loop through the string and increment the counter for each character
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen


 /////////  TEST FUNCTIONS   ///////

 // Given a string function name, print a header for that function
int test_header(char *func) {
    printf("\n#########################################\n");
    printf("          Test %s\n", func);
    printf("#########################################\n\n");
}//end test_header
// This function prints the number of tests passed/failed
int test_pass_fail(void) {
    // If we have not failed
    if (NUM_FAIL == 0)
        printf("ALL TESTS PASSED 🥳🥳🥳🥳\n");
    // If we have had fails
    else // if NUM_FAIL > 0
        printf("TESTS PASSED: %d  TESTS FAILED: %d 😭😭😭\n", NUM_PASS, NUM_FAIL);
    return 0;
}//end test_pass_fail

int test_my_utf8_encode(unsigned char *input, unsigned char *expected) { //, int verbose
    // Each character of the input ASCII can be at most 4 bytes in UTF-8
    // i.e. the encoded UTF-8 can be at most 4 times the length of the original ASCII string +1 for the terminating '00'
    int max_len = 4*my_strlen(input)+1;
    // Initialize output string with max len and invoke the function
    unsigned char output[max_len];
    my_utf8_encode(input, output);
    // print input, output, and hex output after calling my_utf8_encode
    printf("Input: %s \nOutput:   %s\nExpected: %s\nHex Output:   ", input, output, expected);
    my_utf8_printHex(output, 0);
    printf("\nHex Expected: ");
    my_utf8_printHex(expected, 0);
    printf("\n");
    // print a line of '-' and assert if output matched expected output
    int match = my_utf8_strcmp(expected, output);
//    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_encode
int my_utf8_encode_tests() {
    test_header("my_utf8_encode");
    test_my_utf8_encode("Aleph \\u05D0", "Aleph \u05D0");
    test_my_utf8_encode("Sarale \\u05E9\\u05E8\\u05D4\\u05DC\\u05D4", "Sarale \u05E9\u05E8\u05D4\u05DC\u05D4");
    test_my_utf8_encode("Happy \\U0001F600 YAY! \\u066D", "Happy \U0001F600 YAY! \u066D");
    test_my_utf8_encode("\\u00af\\u005c\\u005f\\u0028\\u30c4\\u0029\\u005f\\u002f\\u00af \\u0028\\u25D5\\u203f\\u25D5\\u0029",
                        "¯\\_(ツ)_/¯ (◕‿◕)");
    test_my_utf8_encode("\\U0001F44B\\U0001F30E Hello, World!", "\U0001F44B\U0001F30E Hello, World!");
    test_my_utf8_encode("umm\\U0001F914umm\\U0001F928umm", "umm\U0001F914umm\U0001F928umm");
    test_my_utf8_encode("\\Unicorn\\U0001F984\\unicorn", "\\Unicorn\U0001F984\\unicorn");
    test_my_utf8_encode("\\uber\\\\U0001F695\\Uber\\", "\\uber\\\U0001F695\\Uber\\");
    test_my_utf8_encode("Trying to be sneaky are we? \\U1111FFFF", "Trying to be sneaky are we? \\U1111FFFF");
    test_my_utf8_encode("This is how to write \"Hello\" in Khmer: \\u1787\\u17C6\\u179A\\u17B6\\u1794\\u179F\\u17BD\\u179A",
                        "This is how to write \"Hello\" in Khmer: \u1787\u17C6\u179A\u17B6\u1794\u179F\u17BD\u179A");
    test_my_utf8_encode("\\U0010FFFF", "\U0010FFFF");
}//end my_utf8_encode_tests

int test_my_utf8_decode(unsigned char *input, unsigned char *expected) {
    // Each bytes of UTF-8 encoded string can be at most
    // i.e. the decoded ASCII can be at most 6 times the number of characters in the input string +1 for the terminating '00'
    int max_len = 6*my_strlen(input)+1;
    // Initialize output string with max len and invoke the function
    unsigned char output[max_len];
    my_utf8_decode(input, output);
    // Print the results
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
    // Initialize an output int and invoke the function
    int output = my_utf8_check(string);
    // to simplify the test cases, we'll treat any non-zero output as 1
    if (output != 0) output = 1;
    my_utf8_printHex(string, 1);
    printf("%s a valid UTF8 string\n", output == 0 ? "is NOT" : "is");
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
    // Initialize an output int and invoke the function
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
    // Initialize an output string and invoke the function
    unsigned char *output;
    output = my_utf8_charat(string, index);
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
    test_my_utf8_charat("Unicorn", 8, NULL);
    test_my_utf8_charat("Unicorn \U0001F984 ", 16, NULL);
    test_my_utf8_charat("Hello World", -1, NULL);
    test_my_utf8_charat("שרהלה", 4, "ה");
    test_my_utf8_charat("Aleph \u05D0 ", 6, "\u05D0");
    test_my_utf8_charat("Money \u0024 ", 6, "\u0024");
    test_my_utf8_charat("\u0024 Money", 0, "\u0024");
    test_my_utf8_charat("Money \u0024 Money", 15, NULL);
}//end my_utf8_charat_tests

int test_my_utf8_strcmp(unsigned char *string1, unsigned char *string2, int expected) {
    // Invoke the function and save the output
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
    test_my_utf8_strcmp("🦄", "🦄", 0);            // 2 byte comparisons
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

int test_my_utf8_gematria_decode(int g, unsigned char *expected) {
    // Initialize an output string and invoke the function
    unsigned char output[3];
    my_utf8_gematria_decode(g, output);
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
}//end test_my_utf8_gematria_decode
int my_utf8_gematria_decode_tests(void) {
    test_header("my_utf8_gematria_tests");
    // Initialize the base UTF8 character, Aleph
    char base[3] = {0xD7, 0x90, 0x0};
    // Test for the aleph-bet
    for (int i = 1; i < 28; i++) {
        test_my_utf8_gematria_decode(i, base);
        base[1]++;
        if (i == 10 || i == 13|| i == 15 || i == 19 || i == 21) base[1]++;
    }//end test
    // Test for invalid index
    test_my_utf8_gematria_decode(28, "0");
    test_my_utf8_gematria_decode(770, "0");
}//end my_utf8_gematria_decode_tests

int test_my_utf8_gematria_encode(unsigned char *input, int expected) {
    // Initialize an output string and invoke the function
    int output = my_utf8_gematria_encode(input);
    // Print the input, output, and expected output
    printf("Input: %s\nGematria: %d\nExpected: %d\n", input, output, expected);
    // print a line of '-' and assert if output matched expected output
    expected == output ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
    for (int i = 0; i < BUFFER; i++)
        printf("-");
    printf(" %s\n", expected == output ? "PASSED" : "FAILED");
    return 0;
}//end test_my_utf8_gematria_encode
int my_utf8_gematria_encode_tests(void) {
    test_header("my_utf8_gematria_tests");
    // Initialize the base UTF8 character, Aleph
    char base[3] = {0xD7, 0x90, 0x0};
    // Test for the aleph-bet
    int g = 1, incr = 1;
    for (int i = 1; i < 28; i++) {
        test_my_utf8_gematria_encode(base, g);
        base[1]++; g += incr;
        if (i == 11 || i == 14 || i == 16 || i == 20 || i == 22) g-=incr;
        if (g == 11) {
            g = 20;
            incr = 10;
        }
        if (g == 100) {
            g = 200;
            incr = 100;
        }
    }//end for

    // Test for invalid index
    test_my_utf8_gematria_encode("Hello", 0);
    test_my_utf8_gematria_encode("A", 0);
    test_my_utf8_gematria_encode("ஃபூபா", 0);
}//end my_utf8_gematria_encode_tests

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