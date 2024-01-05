//
// Created by Sarale Goldberger on 12/31/2023.
//

#include <stdio.h>

int main() {
    unsigned char aleph[3] = {0xD7, 0x90, 0x0};
    unsigned char *ptr = aleph;
    unsigned int next = *(ptr+1);
    printf("%s %X, %d\n", aleph, next, next < 0x90 || next > 0xAF);
    return 0;
}

// Returns the amount of characters in an ASCII string
int my_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen

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
//        my_utf8_decode(utf8, unicode);
        return 1;
    } //else if () {
//    }
    return 0;
}//end my_utf8_currency


int test_my_utf8_currency(char *input) {
    char utf8[5];
    char unicode[8];
    my_utf8_currency(input, utf8, unicode);
    printf("Currency: %s\tUTF-8: %s\tUnicode: %s\n", input, utf8, unicode);
    return 0;
}//end test_my_utf8_currency
int my_utf8_currency_tests() {
//    test_header("my_utf8_currency");
    test_my_utf8_currency("USD");
    test_my_utf8_currency("Usd");
    test_my_utf8_currency("usd");
}//end my_utf8_currency_tests


// Takes in a string corresponding to a language and returns a string which is hello in that language
int my_utf8_hello(){
    return 0;
}

// Returns 1 if the character indicates the end of the word/string
int EOS(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
} //end EOS

//unsigned char *my_utf8_charat(unsigned char *string, int index) {
//    // If index is out of range return NULL
//    if ((index >= my_utf8_strlen(string)) || (index < 0))
//        return NULL;
//
//    unsigned char output[7];
//    // increment string to point to the index position of the character we want to return
//    string += index;
//    // Determine the number of bytes and append the correct amount of bytes to output
//    int bytes = my_utf8_numbytes(string);  // What a helpful function!!
//    printf("bytes %d\n", bytes);
//    int i = 0;
//    while (i <= bytes) {
//        output[i++] = *string++;
//        printf("output %s  c %c 0x%X\n", output, *string, *string);
//    }
//    // Terminate output string and return a pointer to it
//    printf("i %d output %s\n", i, output);
//    output[i] = '\0';
//    unsigned char *ptr = output;
//    return ptr;
//}

//
//// Given a UTF8 encoded Hebrew string, return the corresponding gematria
//int my_utf8_gematria_encode(unsigned char *input){
//    int byte2, base = 0x8F, ch, count = 1;
//    static int offset = 0;
//    // Loop through the string
//    while (*input != '\0') {
//        // Make sure input is within the range {0xD7, 0x90} - {0xD7, 0xAF}
//        if (my_utf8_check(input) == 0) return 0;
//        if (my_utf8_numbytes(input) != 2) return 0;
//        byte2 = *(input+1);
//        printf("0x%x\n", byte2);
//        if ((*input < 0xD7) && (byte2 < 0x90 || byte2 > 0xAF)) return 0;
//
//        ch = byte2 - base - offset;
//        for (int i = 1; i < ch; i++) {
//            count++;
//            if (i == 11 || i == 14 || i == 16 || i == 20 || i == 22) count-=1;
//        }
//        input += 2;
//    }//end while
//    return count;
//}// end my_utf8_hebrew
