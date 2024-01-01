// UTF-8 Endcoding in C
// By Sarale Goldberger

#include <stdio.h>

int my_utf8_encode(char *input, char *output);
int my_utf8_check(char *string);

int test_my_utf8_check(char *string) {
    printf("%-8s is a valid UTF8 string: %s\n", string, my_utf8_check(string) == 1 ? "True" : "False");
}

int main() {
    // Junk tests
    printf("Hello, World!\U0001F984 \n");
    printf("Aleph \u05D0 %c%c \n", 0xD7, 0x90);
    printf("\xD7\x90 \n");


    // Test my_utf8_encode
    printf("----- Test my_utf8_encode -----\n");
    char *input = "Aleph \u05D0 Yay!";
    char *output;
    printf("input[6] %x, input[7] %x\n", input[6], input[7]);
    my_utf8_encode(input, output);
//    printf("Input: %s \nOutput: %s\n", input, output); //Correct: %s , (output == "\xD7\x90") == 1 ? "True" : "False"


    // Test my_utf8_check
//    printf("\n----- Test my_utf8_check -----\n");
//    test_my_utf8_check("U+0000");
//    test_my_utf8_check("U+10FF");
//    test_my_utf8_check("U+11FFFF");
//    test_my_utf8_check("U+FFFFFF");
} //end main

int my_strlen(char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.

// Input: "Aleph \u05D0"
// Output: xD7x90
int encode_1byte(char *unicode, char *utf8) {
    printf("1\n");
}

int encode_2byte(char *unicode, char *utf8) {
    printf("2\n");
}

int encode_3byte(char *unicode, char *utf8) {
    printf("3\n");
}

int encode_4byte(char *unicode, char *utf8) {
    printf("4\n");
}

int my_utf8_encode(char *input, char *output) {
    printf("input %s\n", input);
    char c;
    char *c_next;
    char unicode[5];
    char *utf8;
    while (*input != '\0') {
        c = *input;
        printf("c: %c\n", c);
        switch (c) {
            // if we are encountering unicode
            case 'U':
                c_next = ++input;
                printf("c_next: %s\n", c_next);
                if (*c_next == '+') {
                    c_next++;
                    printf("c_next: %s\n", c_next);
                    int i = 0;
                    while (*c_next != ' ' && *c_next != 'U') {
                        unicode[i] = *c_next;
                        c_next++;
                        i++;
                    }
//                    unicode[i] = '\0';
                    printf("unicode: %s\n", unicode);
                    if (i == 4)
                        encode_4byte(unicode, utf8);
                    else if (unicode[1] >= '8')
                        encode_3byte(unicode, utf8);
                    else if (unicode[2] >= '8')
                        encode_2byte(unicode, utf8);
                    else
                        encode_1byte(unicode, utf8);
                }//end if

//            default:

        } //end switch
        input++;
    } //end while
//    for (int i = 0; i < length; i++)
//        printf("%c", input[i]);
    return 0;
} //end my_utf8_encode

// Takes a UTF8 encoded string, and returns a string, with ASCII representation where possible, and UTF8 character
// representation for non-ASCII characters.
int my_utf8_decode(char *input, char *output) {
    return 0;
}
// Validates that the input string is a valid UTF8 encoded string.
// Invalid if it is out of range. (negative? or greater than U+10FFFF)
int my_utf8_check(char *string) {
    string += 2;
//    return ((int)string <= 0x10FFFF);
    return 0;
}

// Returns the number of characters in the string.
int my_utf8_strlen(char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
    }
    return count;
}

// Returns the UTF8 encoded character at the location specified.
char *my_utf8_charat(char *string, int index) {
    return 0;
}

// Returns whether the two strings are the same (similar result set to strcmp() )
int my_utf8_strcmp(char *string1, char *string2) {
    return 0;
}


// Each student should add 2 functions that they believe are useful.  We will grade based on the complexity and
// creativity of these functions.

