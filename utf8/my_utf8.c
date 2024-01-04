// UTF-8 Endcoding in C
// By Sarale Goldberger

#include <stdio.h>

int my_strlen(unsigned char *string);
int my_utf8_encode(unsigned char *input, unsigned char *output);
int my_utf8_decode(unsigned char *input, unsigned char *output);
int my_utf8_check(unsigned char *string);
int my_utf8_currency(char *cur, char *utf8, char *unicode);

int test_my_utf8_encode(unsigned char *input);
int test_my_utf8_decode(unsigned char *string);
int test_my_utf8_check(unsigned char *string);
int test_my_utf8_currency(char *input);

int main() {

    // Test my_utf8_encode
    printf("----- Test my_utf8_encode -----\n");
    test_my_utf8_encode("Aleph \\u05D0");
    test_my_utf8_encode("Sarale \\u05E9\\u05E8\\u05D4\\u05DC\\u05D4");
    test_my_utf8_encode("Happy \\U0001F600 YAY! \\u066D");
    test_my_utf8_encode("\\u00af\\u005c\\u005f\\u0028\\u30c4\\u0029\\u005f\\u002f\\u00af \\u0028\\u25D5\\u203f\\u25D5\\u0029");
    test_my_utf8_encode("\\U0001F44B\\U0001F30E Hello, World!");
    test_my_utf8_encode("umm\\U0001F914umm\\U0001F928umm");
    test_my_utf8_encode("\\Unicorn\\U0001F984\\unicorn");
    test_my_utf8_encode("\\uber\\\\U0001F695\\Uber\\");
    test_my_utf8_encode("Trying to be sneaky are we? \\U1111FFFF");
    test_my_utf8_encode("This is how to write \"Hello\" in Khmer: \\u1787\\u17C6\\u179A\\u17B6\\u1794\\u179F\\u17BD\\u179A");

    // Test my_utf8_decode
    printf("\n----- Test my_utf8_decode -----\n");
    unsigned char temp1[] = {0x41, 0x0};
    test_my_utf8_decode(temp1);
    unsigned char temp1a[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x0};
    test_my_utf8_decode(temp1a);
    unsigned char temp2[] = {0xd7, 0x90, 0x0};
    test_my_utf8_decode(temp2);
    test_my_utf8_decode("א");
    unsigned char temp3[] = {0xE0, 0xA0, 0x80, 0x0};
    test_my_utf8_decode(temp3);
    unsigned char temp3a[] = {0xEF, 0xBF, 0xA0, 0x0};
    test_my_utf8_decode(temp3a);
    unsigned char temp4[] = {0xF0, 0x90, 0x80, 0x80, 0x0};
    test_my_utf8_decode(temp4);
    unsigned char temp4a[] = {0xF0, 0x9F, 0xA8, 0x82, 0x0};
    test_my_utf8_decode(temp4a);
//    test_my_utf8_decode("\U00010000");
//    unsigned char temp3[] = {0xF0, 0x9F, 0xA6, 0x84, 0x0};
//    test_my_utf8_decode(temp3);
//    test_my_utf8_decode("שרהלה");

    // Test my_utf8_check
//    printf("\n----- Test my_utf8_check -----\n");
//    test_my_utf8_check("\\u0010");
//    test_my_utf8_check("\\u10FF");
//    test_my_utf8_check("\\U0001FFFF");
//    test_my_utf8_check("\\U00FFFFFF");

    // Test my_utf8_currency
//    printf("\n----- Test my_utf8_currency -----\n");
//    test_my_utf8_currency("USD");
//    test_my_utf8_currency("Usd");
//    test_my_utf8_currency("usd");

} //end main


int test_my_utf8_check(unsigned char *string) {
    printf("%-8s is a valid UTF8 string: %s\n", string, my_utf8_check(string) == 1 ? "True" : "False");
    printf("\n--\n");
    return 0;
}//end test_my_utf8_check

int test_my_utf8_encode(unsigned char *input) {
    // Each character of the input ASCII can be at most 4 bytes in UTF-8
    // i.e. the encoded UTF-8 can be at most 4 times the length of the original ASCII string +1 for the terminating '00'
    int max_len = 4*my_strlen(input)+1;
    // initializing output string with max len
    unsigned char output[max_len];
    my_utf8_encode(input, output);
    // print input, output, and hex output after calling my_utf8_encode
    printf("Input: %s \nOutput: %s\nHex Output: ", input, output);
    for (int i = 0; i < my_strlen(output); i++)
        printf("%X ", (unsigned char)output[i]);
    printf("\n--\n");
    return 0;
}//end test_my_utf8_encode

int test_my_utf8_decode(unsigned char *input) {
    // Each bytes of UTF-8 encoded string can be at most
    // i.e. the decoded ASCII can be at most 6 times the number of characters in the input string +1 for the terminating '00'
    int max_len = 6*my_strlen(input)+1;
    // initializing output string with max len
    unsigned char output[max_len];
    my_utf8_decode(input, output);
    printf("Input: %s \nHex Input: ", input);
    for (int i = 0; i < my_strlen(input); i++)
        printf("%X ", (unsigned char)input[i]);
    printf("\nOutput: %s\n", output);
    printf("--\n");
    return 0;
}// end test_my_utf8_decode

int test_my_utf8_currency(char *input) {
    char utf8[5];
    char unicode[8];
    my_utf8_currency(input, utf8, unicode);
    printf("Currency: %s\tUTF-8: %s\tUnicode: %s\n", input, utf8, unicode);
    return 0;
}//end test_my_utf8_currency


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
// Invalid if it is out of range. (negative? or greater than \\U0010FFFF)
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
    int index = 0, limit = 0, i = 0;
    unsigned char temp;
    unsigned char unicodeString[9];
    unsigned int unicode;
    while (*input != '\0') {
//        printf("c: %c\n", *input);
        // if we are encountering unicode notation
        temp = *(input + 1);
        if (*input == '\\' && (temp == 'u' || temp == 'U')) {
            if (temp == 'u') // will need to extract 4 digits i.e. till index 3
                limit = 3;
            if (temp == 'U') // will need to extract 8 digits i.e. till index 7
                limit = 7;
//            printf("input: %s\n", input);
            input += 2;
//            printf("input: %s\n", input);
            // We've encountered codepoint notation, so we must extract it
            // If the unicode starts with a 0, it will have 4 digits i.e. go till index 3
            // If the unicode starts with a 1, it will have 5 digits i.e. go till index 4
            // The unicode must only contain valid hex digits, and otherwise throw an error
            i = 0;
            while (i <= limit && validHex(*input)) { //CLEAN THIS UP *input != '\\' &&  && !EOS(*input)
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
                output[index++] = temp;  // or 'U'
                for (int j = 0; j < i; j++) {
                    output[index++] = unicodeString[j];
                }//end for
//                output[index++] = *input;
            } else {
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
                } else { // Invalid unicode code point, treat as ASCII
                    output[index++] = '\\';
                    output[index++] = 'U';
                    for (int j = 0; j < i; j++) {
                        output[index++] = unicodeString[j];
                    }//end for
                }//end if else
//            printf("utf8: %X %X  output: %s \n", (unsigned char)output[index-2], (unsigned char)output[index-1], output);
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
    return 1; //successful encode!
} //end my_utf8_encode

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.
int my_utf8_decode(unsigned char *input, unsigned char *output) {
    unsigned int byte, byte2, byte3, byte4, unicode;
    unsigned char byteString[7], unicodeString[5];
    while (*input != '\0') {
        printf("%c  x%X  next: x%X\n", *input, *input, *(input+1));
        byte = *input++;
        *output++ = 0x5C;    // ASCII character backslash
        sprintf(byteString, "%X", byte);
//        printf("byte: x%X  byteString: %s\n", byte, byteString);
        if (byte <= 0x7F) { // Expect 1 byte
            *output++ = 0x75;  // ASCII character u
            *output++ = 0x30; // ASCII character 0
            *output++ = 0x30;
            *output++ = byteString[0];
            *output++ = byteString[1];
            *output = '\0';
        } else if (byte <= 0xDF) {
            byte2 = *input++;
            *output++ = 0x75;  // ASCII character u
            *output++ = 0x30;
            unicode = (((byte & 0x1F) << 6) | (byte2 & 0x3F));
            sprintf(unicodeString, "%X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
            *output = '\0';
        } else if (byte <= 0xEF) {
            byte2 = *input++;
            byte3 = *input++;
            *output++ = 0x75;  // ASCII character u
            unicode = (byte & 0x0F) << 12 | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
            sprintf(unicodeString, "%X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
            *output = '\0';
        } else if (byte <= 0xF4) {
            byte2 = *input++;
            byte3 = *input++;
            byte4 = *input++;
            *output++ = 0x55;  // ASCII character U
            unicode = (byte & 0x0F) << 18 | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
            sprintf(unicodeString, "%X", unicode);
            *output++ = unicodeString[0];
            *output++ = unicodeString[1];
            *output++ = unicodeString[2];
            *output = '\0';
        } else {
            printf("Invalid hex byte %X\n", byte);
            return 0; //unsuccessful encode
        }//end if else
    }//end while
    return 0;
}//end my_utf8_decode

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


// Takes in a string corresponding to a language and returns a string which is hello in that language
int my_utf8_hello(){
    return 0;
}

int my_strcpy_upper(char *input, char *output) {
    while (*input != '\0') {
        // if character is lowercase convert to uppercase and append
        if (*input >= 'a' && *input <= 'z')
            *output = (*input - 'a' + 'A');
        // if it's already uppercase just append
        else
            *output = *input;
        output++;
        input++;
    }// end while
    *output++ = '\0';
    return 0;
}//end my_strcpy_upper

// Takes in a string corresponding to a currency and returns the UTF-8 encoded string and the Unicode code point in \uXXX notation
int my_utf8_currency(char *cur, char *utf8, char *unicode){
    char cur_upper[my_strlen(cur)+1];
    my_strcpy_upper(cur, cur_upper);
    if (cur == "USD") {
         utf8[0] = 0x24;
         utf8[1] = '\0';
         my_utf8_decode(utf8, unicode);
         return 1;
    } //else if () {
//    }
    return 0;
}//end my_utf8_currency

// Takes in a string corresponding to a language and returns a string which is hello in that language
int my_utf8_hebrew(){
    return 0;
}// end my_utf8_hebrew