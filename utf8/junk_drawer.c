//
// Created by Sarale Goldberger on 12/31/2023.
//

#include <stdio.h>

int main() {
    printf("\n");
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